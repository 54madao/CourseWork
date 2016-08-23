// header files
#include <iostream>
#include <math.h>                // for pow( )
#include <stdlib.h>              // for rand( )
#include "mpi.h"
#include "omp.h"
#include "Timer.h"

// constants
#define Nmax 1000                // maximum number of molecules
#define pixelsPerUnit 2          // space needed for each molecule
#define dt 0.02                  // time increment in natural units
#define wallStiffness 50.0       // "spring constant" for bouncing off wall
#define forceCutoff 3.0          // distance beyond which we don't bother to compute the force

using namespace std;

// global variables
int    N  = 0;                                   // currrent number of molecules
double t  = 0.0;                                 // clock time in natural units
double forceCutoff2 = forceCutoff * forceCutoff; // a squar of force cut off 
double pEatCutoff = 4 * ( pow( forceCutoff, -12.0 ) - pow( forceCutoff, -6.0 ) );

double boxWidth = 500 * 1.0 / pixelsPerUnit; // simulation space in a square 250 x 250
double gravity = 0;             // local gravitational constant in natural units

double x[Nmax];                 // x[i] = molecular i's location in x coordinate
double y[Nmax];                 // y[i] = molecular i's location in y coordinate
double vx[Nmax];                // vx[i] = molecular i's velocity in x coordinate
double vy[Nmax];                // xy[i] = molecular i's velocity in y coordinate
double ax[Nmax];                // ax[i] = molecular i's acceleration in x coordinate
double ay[Nmax];                // ax[i] = molecular i's acceleration in x coordinate

int my_rank = 0;	// used by MPI
int mpi_size = 1;	// used by MPI
int stripe = 0;		// used by MPI
int nThreads = 1;	// used by OpenMP

/* 
 * Add a molecule at the first available space (if any)
 * @return true if a new molecule was added successfully, otherwise false.
 */
bool addMolecule( ) {
  double buffer = 1.3;          // minimum space required between molecule centers
  double epsilon = 0.01;        // small distance
  if ( N == Nmax )              // quit if max number of molecules is already reached
    return false;
  double xTest = buffer / 2;    // start looking for space at lower-left corner
  double yTest = buffer / 2;

  while (true) {  
    // this loop actually does terminate, when one or the other "return" statement is reached
    bool spaceOpen = true;          // temporarily assume this space is available

    for ( int i = 0; i < N; i++ ) { // check all other molecules to see if any are too close
      if ( ( fabs( x[i] - xTest ) < buffer - epsilon ) && 
	   		( fabs( y[i] - yTest ) < buffer-epsilon ) ) {
				spaceOpen = false;
				break;
      }
    }
    if ( spaceOpen ) {
      // random nudge is to avoid too much symmetry
      x[N] = xTest + ( double( rand( ) ) / double( RAND_MAX ) - 0.5 ) * epsilon; 
      y[N] = yTest + ( double( rand( ) ) / double( RAND_MAX ) - 0.5 ) * epsilon;
      vx[N] = 0; vy[N] = 0;
      ax[N] = 0; ay[N] = 0;
      N++;
      return true;
    } else {                // if this space isn't open, try the next...
      xTest += buffer;
      if ( xTest + buffer / 2 > boxWidth ) { // reached maximum X
				xTest = buffer / 2;
				yTest += buffer;
				if ( yTest + buffer/2 > boxWidth )   // reached maximum Y
	  			return false;                      // no more space
      }
    }
  }
}

/*
 * Computes accelerations of all atoms from current positions
 */
void computeAccelerations( ) {
  
  // change each atom's acceleration when it bounce against a wall
  
  #pragma omp parallel for
  for (int i = my_rank * stripe; i < ( my_rank + 1) * stripe; i++) {
    // checking if an atom is bouncing against the left or right boundary
    if ( x[i] < 0.5 ) {                       // bouncing against the left boudary
      ax[i] = wallStiffness * ( 0.5 - x[i] );
    } else if ( x[i] > ( boxWidth - 0.5 ) ) { // bouncing against the right boundary
      ax[i] = wallStiffness * ( boxWidth - 0.5 - x[i] );
    } else                                    // in a middle of the simulation box
      ax[i] = 0.0;

    // checking if an atom is bouncing against the upper or lower boundary
    if ( y[i] < 0.5 ) {                       // bouncing againt the lower boundary
      ay[i] = wallStiffness * ( 0.5 - y[i] );
    } else if ( y[i] > ( boxWidth - 0.5 ) ) { // bouncing againt the upper boundary
      ay[i] = wallStiffness * ( boxWidth - 0.5 - y[i] );
    } else                                    // in a middle of the simulation box
      ay[i] = 0;

    // add gravity
    ay[i] -= gravity;
    
  }

  MPI_Allgather(MPI_IN_PLACE, stripe, MPI_DOUBLE, ax, stripe, MPI_DOUBLE, MPI_COMM_WORLD);
 	MPI_Allgather(MPI_IN_PLACE, stripe, MPI_DOUBLE, ay, stripe, MPI_DOUBLE, MPI_COMM_WORLD);

  // Now compute interaction forces (Lennard-Jones potential).
  // This is where the program spends most of its time (when N is reasonably large),
  // so we carefully avoid unnecessary calculations and array lookups.

  double dx, dy;  // separations in x and y directions
  double dx2, dy2, rSquared, rSquaredInv, attract, repel, fOverR, fx, fy;
	
  #pragma omp parallel for private(dx, dx2, dy, dy2, rSquared, rSquaredInv, attract, repel, fOverR, fx, fy)
  for ( int i = my_rank * stripe; i < ( my_rank + 1 ) * stripe; i++ ) {           // for each molecule
    for (int j = 0; j < i; j++) {           // loop over all distinct pairs
      dx = x[i] - x[j];
      dx2 = dx * dx;
      if ( dx2 < forceCutoff2 ) {           // make sure they're close enough to bother    	
				dy = y[i] - y[j];
				dy2 = dy * dy;
				if ( dy2 < forceCutoff2 ) {         
	  			rSquared = dx2 + dy2;
	  			if ( rSquared < forceCutoff2 ) {  // yes, these two molecules are very close!
            // now compute their collision 
	    			rSquaredInv = 1.0 / rSquared;
	    			attract = rSquaredInv * rSquaredInv * rSquaredInv;
	  			  repel = attract * attract;
	  			  fOverR = 24.0 * ( ( 2.0 * repel ) - attract ) * rSquaredInv;
	  			  fx = fOverR * dx;
	  			  fy = fOverR * dy;

	  			  #pragma omp critical
           	{
		 				  ax[i] += fx;  // add this force on to i's acceleration ( mass = 1 )
		 				  ay[i] += fy;
	    				ax[j] -= fx;  // Newton's 3rd law
	    				ay[j] -= fy;
	    			}
	  			}
				}
      }
    }
  }  
  MPI_Allreduce( MPI_IN_PLACE, ax, N, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );
 	MPI_Allreduce( MPI_IN_PLACE, ay, N, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );	
}   // end of method computeAccelerations

void initialize( int n ) {
  for ( int i = 0; i < n; i++) 
    if ( addMolecule( ) == false )
      break;
      
	stripe = N / mpi_size; // partitioned stripe 
  computeAccelerations( );
}

/*
 * Executes one time step using the Verlet algorithm (from Gould and Tobochnik);
 * The physics is all in this method and computeAccelerations( ).
 */
void singleStep( ) {
  double dtOver2 = 0.5 * dt;
  double dtSquaredOver2 = 0.5 * dt * dt;

	//test 2
	
  #pragma omp parallel 
  {
 	#pragma omp for nowait
  for ( int i = 0; i < N; i++ ) { // Compute each molecule's new position and velocity
    x[i] += ( vx[i] * dt ) + ( ax[i] * dtSquaredOver2 );  // update position
    y[i] += ( vy[i] * dt ) + ( ay[i] * dtSquaredOver2 );
    vx[i] += ( ax[i] * dtOver2 );                         // update velocity halfway
    vy[i] += ( ay[i] * dtOver2 );
  }
  }
  computeAccelerations( );        // Compute each molecule's new acceleration
  
  #pragma omp parallel for
  for ( int i = 0; i < N; i++ ) {
    vx[i] += ( ax[i] * dtOver2 ); // finish updating velocity with new acceleration
    vy[i] += ( ay[i] * dtOver2 );
  }
  t += dt;
}  // end of method singleStep

/*
 * Prints each molecule's position.
 */
void printMolecules( ) {
  cout << t << endl;
  for ( int i = 0; i < Nmax; i++ )
    cout << x[i] << "\t" << y[i] << endl;
  cout << endl;
}

/*
 * Simultes moledular dynamics with "Nrequested" atoms for "maxSteps" and shows their
 * motions every "frequency". 
 */
int main( int argc, char *argv[] ) {
  int Nrequested = 1000;          // number of molecules requested by a user
  int maxSteps   = 10000;         // simulation lasts from 0 to maxTime - 1
  int frequency  = 100;           // frequency to show results

  // verify arguments
  if ( argc == 5 ) {
    Nrequested = atoi( argv[1] );
    maxSteps = atoi( argv[2] );
    frequency = atoi( argv[3] );
    nThreads = atoi( argv[4] );
  }
  else if ( argc != 1 ) {
    cerr << "usage: MD #molecules maxSteps frequency" << endl;
    return -1;
  }
  if( my_rank == 0 )
  cerr << "#molecules = " << Nrequested 
       << ", maxSteps = " << maxSteps 
       << ", frequency = " << frequency
       << ", nThreads = " << nThreads << endl;
  
  // change # of threads
  omp_set_num_threads( nThreads );
  
  MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &my_rank );
	MPI_Comm_size( MPI_COMM_WORLD, &mpi_size );
	
  initialize( Nrequested );

  // start a timer
  Timer timer;
  timer.start( );
	
  // the main body of computation
  for ( int stepCount = 0; stepCount < maxSteps; stepCount++ ) {
    if ( frequency > 0 && stepCount % frequency == 0 && my_rank == 0)
      printMolecules( );

    gravity = ( stepCount > maxSteps * 0.3 && stepCount < maxSteps * 0.4 ) ? 0.1 : 0.0;
    singleStep( );
  }

  // finished the timer
  if( my_rank == 0)
  	cerr << "elapsed time = " << timer.lap( ) << endl;

	MPI_Finalize();
  return 0;
}
