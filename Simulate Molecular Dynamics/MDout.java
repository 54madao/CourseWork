/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Insets;
import javax.swing.JFrame;
import java.util.Scanner;

/**
 *
 * @author munehiro
 */
class MDout {
    public static int Nmax = 1000;                 // # molecules

    static final Color bgColor = new Color(255, 255, 255); //white background
    private JFrame gWin;                     // a graphics window
    private int boxWidth = 750;              // simulation space of 250.000 x 250.000 
    private int cellWidth = 3;               // each cell's width 0.01 in the window
    private Insets theInsets;                // the insets of the window
    private Color atomColor[];               // atom color [0]: no existance, [1]: existance

    public MDout( ) {
	startGraphics();
    }

    private void startGraphics() {

        // initialize window and graphics:
        gWin = new JFrame("Molecular Dynamics");
        gWin.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        gWin.setLocation(50, 50);  // screen coordinates of top left corner
        gWin.setResizable(false);
        gWin.setVisible(true);     // show it!
        theInsets = gWin.getInsets();
        gWin.setSize(boxWidth + theInsets.left + theInsets.right,
		     boxWidth + theInsets.top + theInsets.bottom);

        // wait for frame to get initialized
        long resumeTime = System.currentTimeMillis() + 1000;
        do {
        } while (System.currentTimeMillis() < resumeTime);

	// pop out the graphics
        Graphics g = gWin.getGraphics();
        g.setColor(bgColor);
        g.fillRect(theInsets.left,
		   theInsets.top,
		   boxWidth,
		   boxWidth);

	// set two colors: atomColor[0] = "white", atomColor[1] = "blue"
        atomColor = new Color[2];
        atomColor[0] = new Color(0xFFFFFF);
        atomColor[1] = new Color(0x0000FF);
    }

    private void writeToGraphics( double[] x, double[] y, int existance ) {
        Graphics g = gWin.getGraphics();
	g.setColor( atomColor[existance] );	

	for ( int i = 0; i < Nmax; i++ ) {
	    g.fillOval( theInsets.left + ( int )( x[i] * 4 ),
			  theInsets.top + boxWidth - ( int )( y[i] * 4 ),
			  cellWidth, cellWidth );
	}
    }

    public static void main(String args[]) {
	MDout box = new MDout( );  // create a graphics

	double time = 0.0;               // simulation time
	double[] x = new double[Nmax];   // x[i] = molecular i's location in x coordiante
	double[] y = new double[Nmax];   // y[i] = molecular i's location in y coordinate

	Scanner keyboard = new Scanner( System.in );
	
	while ( keyboard.hasNext( ) ) {
	    time = keyboard.nextDouble( );
	    System.out.println( "time = " + time );
	    box.writeToGraphics( x, y, 0 );
	    for ( int i = 0; i < Nmax; i++ ) {
		x[i] = keyboard.nextDouble( );
		y[i] = keyboard.nextDouble( );
	    }
	    box.writeToGraphics( x, y, 1 );
	}
	System.out.println( "Done..." );
    }
}
