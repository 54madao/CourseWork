import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.EOFException;
import java.io.IOException;
import java.net.BindException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketTimeoutException;
import java.net.UnknownHostException;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JOptionPane;

/**
 *
 * @author Munehiro Fukuda
 */
public class OnlineTicTacToe implements ActionListener {

    private final int INTERVAL = 1000;         // 1 second
    private final int NBUTTONS = 9;            // #bottons
    private ObjectInputStream input = null;    // input from my counterpart
    private ObjectOutputStream output = null;  // output from my counterpart
    private JFrame window = null;              // the tic-tac-toe window
    private JButton[] button = new JButton[NBUTTONS]; // button[0] - button[9]
    private boolean[] myTurn = new boolean[1]; // T: my turn, F: your turn
    private String myMark = null;              // "O" or "X"
    private String yourMark = null;            // "X" or "O"
    private Socket client = null;
    private boolean won = false;				// T: counterpart is won
    private boolean tie = false;				// T: It's tie
    private int steps = 0;						// count the number of steps
    private boolean myNewGame = false;			// T: I agree to start a new game
    private boolean setFirst = true;			// T: set to play first
    /**
     * Prints out the usage.
     */
    private static void usage( ) {
        System.err.
	    println( "Usage: java OnlineTicTacToe ipAddr ipPort(>=5000)" );
        System.exit( -1 );
    }

    /**
     * Prints out the track trace upon a given error and quits the application.
     * @param an exception 
     */
    private static void error( Exception e ) {
        e.printStackTrace();
        System.exit(-1);
    }

    /**
     * Starts the online tic-tac-toe game.
     * @param args[0]: my counterpart's ip address, args[0]: his/her port
     */
    public static void main( String[] args ) {
        // verify the number of arguments
        if ( args.length != 2 ) {
            usage( );
        }

        // verify the correctness of my counterpart address
        InetAddress addr = null;
        try {
            addr = InetAddress.getByName( args[0] );
        } catch ( UnknownHostException e ) {
            error( e );
        }

        // verify the correctness of my counterpart port
        int port = 0;
        try {
            port = Integer.parseInt( args[1] );
        } catch (NumberFormatException e) {
            error( e );
        }
        if ( port < 5000 ) {
            usage( );
        }
		
        // now start the application
        OnlineTicTacToe game = new OnlineTicTacToe( addr, port );
		
    }

    /**
     * Is the constructor that sets up a TCP connection with my counterpart,
     * brings up a game window, and starts a slave thread for listenning to
     * my counterpart.
     * @param my counterpart's ip address
     * @param my counterpart's port
     */
    public OnlineTicTacToe( InetAddress addr, int port ) {

        // set up a TCP connection with my counterpart
		boolean islocal = false; // T: the game is played in local site
		myTurn = new boolean[1];
		myTurn[0] = true;

	    // Prepare a server socket and make it non-blocking
	    ServerSocket server = null;
	    try {
	        server = new ServerSocket( port );
			
            //If it's not a local game, then set the server non-blocking, (i.e. time out beyon 1000)
            if(!addr.equals(InetAddress.getByName( "localhost" )) && !addr.equals(InetAddress.getLocalHost())){
            	System.out.println("It's not a local game!");
            	server.setSoTimeout( INTERVAL );
            }         
	    } catch ( BindException e ) {
	    	islocal = true;
	        
	    } catch ( Exception e){
	    	error( e );
	    }

		// While accepting a remote request, try to send my connection request
		while ( true ) {
			if(!islocal){
	    		try {
					//Try to accept a connection as a server
					client = server.accept( );
	    		} catch ( SocketTimeoutException ste ) {
					// Couldn't receive a connection request withtin INTERVAL
	    		} catch ( IOException ioe ) {
					error( ioe );
	    		}
	    		// Check if a connection was estabmyTurnhed. If so, leave the loop
	    		if ( client != null ){
					setFirst = false;
					myTurn[0] = false;
					break;
				}
			}

	    	try {
				// Try to request a connection as a client
				client = new Socket(addr, port);
	    	} catch ( IOException ioe ) {
				// Connection refused
	    	}
	    		// Check if a connection was estabmyTurnhed, If so, leave the loop
	    	if ( client != null ){
		    	break;
        	}
		}
		
        // set up a window
        makeWindow( setFirst ); // or makeWindow( false );

        // start my counterpart thread
        Counterpart counterpart = new Counterpart( );
        counterpart.start();
    }

    /**
     * Creates a 3x3 window for the tic-tac-toe game
     * @param true if this window is created by the 1st player, false by
     *        the 2nd player
     */
    private void makeWindow( boolean amFormer ) {
        myTurn[0] = amFormer;
        myMark = ( amFormer ) ? "O" : "X";    // 1st person uses "O"
        yourMark = ( amFormer ) ? "X" : "O";  // 2nd person uses "X"

        // create a window
        window = new JFrame("OnlineTicTacToe(" +
                ((amFormer) ? "former)" : "latter)" ) + myMark );
        window.setSize(300, 300);
        window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        window.setLayout(new GridLayout(3, 3));

		// initialize all nine cells.
        for (int i = 0; i < NBUTTONS; i++) {
            button[i] = new JButton();
            window.add(button[i]);
            button[i].addActionListener(this);
        }

		// make it visible
        window.setVisible(true);
    }

    /**
     * Marks the i-th button with mark ("O" or "X")
     * @param the i-th button
     * @param a mark ( "O" or "X" )
     * @param true if it has been marked in success
     */
    private boolean markButton( int i, String mark ) {
	if ( button[i].getText( ).equals( "" ) ) {
	    button[i].setText( mark );
	    button[i].setEnabled( false );
		steps++;
	    return true;
	}
	return false;
    }

    /**
     * Checks which button has been clicked
     * @param an event passed from AWT 
     * @return an integer (0 through to 8) that shows which button has been 
     *         clicked. -1 upon an error. 
     */
    private int whichButtonClicked( ActionEvent event ) {
	for ( int i = 0; i < NBUTTONS; i++ ) {
	    if ( event.getSource( ) == button[i] )
		return i;
	}
	return -1;
    }

    /**
     * Checks if the i-th button has been marked with mark( "O" or "X" ).
     * @param the i-th button
     * @param a mark ( "O" or "X" )
     * @return true if the i-th button has been marked with mark.
     */
    private boolean buttonMarkedWith( int i, String mark ) {
	return button[i].getText( ).equals( mark );
    }

    /**
     * Pops out another small window indicating that mark("O" or "X") won!
	 * Confirm if players wants to start a new game
     * @param a mark ( "O" or "X" )
     */
    private void showWon( String mark ) {
		JOptionPane.showMessageDialog( null, mark + " won!" );
		int confirm = JOptionPane.showConfirmDialog( null, "Want new game?", "new game", JOptionPane.YES_NO_OPTION);
		
		try{
			output = new ObjectOutputStream(client.getOutputStream());
			if(confirm == JOptionPane.YES_OPTION){
				//send "n" to state a desire of a new game
				output.writeObject("n");						
				output.flush();
				myNewGame = true;		
			}
			else{
				//send "9" to end the game
				output.writeObject("" + 9);
				output.flush();
			}
		}catch(Exception e){
		
		}
    }

	/**
     * Pops out another small window indicating that it was a tie!
	 * Confirm if players wants to start a new game
     */
	private void showTie( ) {
		JOptionPane.showMessageDialog( null, "It's a tie!" );
		int confirm = JOptionPane.showConfirmDialog( null, "Want new game?", "new game", JOptionPane.YES_NO_OPTION);
		
		try{
			output = new ObjectOutputStream(client.getOutputStream());
			if(confirm == JOptionPane.YES_OPTION){
				System.out.println(confirm);
				output.writeObject("n");						
				output.flush();
				myNewGame = true;		
			}
			else{
				System.out.println(confirm);
				output.writeObject("" + 9);
				output.flush();
			}
		}catch(Exception e){
		
		}	
    }

	/**
     * Reset settings to start a new game
	 *
     */
	private void newGame( ) {
		System.out.println("New Game Starts!");
		setFirst = !setFirst;
		myTurn[0] = setFirst;
		won = false;
		tie = false;
		steps = 0;
		myNewGame = false;
		myMark = ( myTurn[0] ) ? "O" : "X";    // 1st person uses "O"
        yourMark = ( myTurn[0] ) ? "X" : "O";  // 2nd person uses "X"
		window.setTitle("OnlineTicTacToe(" +
                ((myTurn[0]) ? "former)" : "latter)" ) + myMark);
		for(int i = 0; i < NBUTTONS; i++){
			button[i].setText( "" );
	    	button[i].setEnabled( true );
		}
	}

    /**
     * Is called by AWT whenever any button has been clicked. You have to:
     * <ol>
     * <li> check if it is my turn,
     * <li> check which button was clicked with whichButtonClicked( event ),
     * <li> mark the corresponding button with markButton( buttonId, mark ),
     * <li> send this informatioin to my counterpart,
     * <li> checks if the game was completed with 
     *      buttonMarkedWith( buttonId, mark ) 
     * <li> shows a winning message with showWon( )
     */
    public void actionPerformed( ActionEvent event ) {

		System.out.println("myTurn:" + myTurn[0] + "  from action");
		
		//if it's not my turn to play
		if(!myTurn[0]){
			System.out.println("I'm listening");
			return;
		}

		synchronized(myTurn){
			int index = whichButtonClicked(event);
			
			//check if the button has been marked
			if(!markButton(index, myMark)){
				System.out.println("Been Marked!");
				return;
			}
			System.out.println("I marked: " + index);
			//check if the player won
			if(checkWon(index)){
			
				//send my winning;
				try{
					output = new ObjectOutputStream(client.getOutputStream());
					output.writeObject("w");
					output.flush();				
					output.writeObject("" + index);
					output.flush();			    
	    			showWon(myMark);
	    		} catch ( Exception e ) {
	    			error( e );
				}
			}
			else if(steps == 9){
				//send the information if it's a tie
				try{
					output = new ObjectOutputStream(client.getOutputStream());
					output.writeObject("t");
					output.flush();				
					output.writeObject("" + index);
					output.flush();			    
	    			showTie();
					//output.close();
	    		} catch ( Exception e ) {
	    			error( e );
				}
			}
			else{
				//send my action
				try{
					output = new ObjectOutputStream(client.getOutputStream());			    
	    			output.writeObject("" + index);
					output.flush();
	    		} catch ( Exception e ) {
	    			error( e );
				}
			}
			myTurn[0] = false;
			myTurn.notify(); // notify the counterpart thread		
		}
    }
	
	private boolean checkWon(int index){
		int i = index / 3;
		int j = index % 3;
		int index1;
		int index2;
		//check horizontal
		index1 = i * 3 + (j + 1) % 3;
		index2 = i * 3 + (j + 2) % 3;
		if(buttonMarkedWith(index1, myMark) && buttonMarkedWith(index2, myMark)){
			return true;
		}
		//check vertical
		index1 = ((i + 1) % 3) * 3 + j;
		index2 = ((i + 2) % 3) * 3 + j;
		if(buttonMarkedWith(index1, myMark) && buttonMarkedWith(index2, myMark)){
			return true;
		}
		//check from topleft to bottomright
		if(i == j){
			index1 = ((i + 1) % 3) * 3 + (j + 1) % 3;
			index2 = ((i + 2) % 3) * 3 + (j + 2) % 3;
			if(buttonMarkedWith(index1, myMark) && buttonMarkedWith(index2, myMark)){
				return true;
			}
		}
		//check from topright to bottomleft
		if(i + j == 2){
			index1 = ((i + 1) % 3) * 3 + 2 - ((i + 1) % 3);
			index2 = ((i + 2) % 3) * 3 + 2 - ((i + 2) % 3);
			if(buttonMarkedWith(index1, myMark) && buttonMarkedWith(index2, myMark)){
				return true;
			}
		}
		return false;
	}

    /**
     * This is a reader thread that keeps reading fomr and behaving as my
     * counterpart.
     */
    private class Counterpart extends Thread {

		/**
		* Is the body of the Counterpart thread.
	 	*/
    	@Override
    	public void run( ) {
			while(true){
				synchronized(myTurn){
					System.out.println("myTurn:" + myTurn[0] + "  from run");
				
					//wait here, if it's my turn
					while(myTurn[0]){
						System.out.println("I'm playing");
						try{					
							myTurn.wait();
						} catch(InterruptedException e){
					
						}
					}
					
	    			try{
	    				input = new ObjectInputStream(client.getInputStream());
	    				String msg = ( String )input.readObject( );
						int index = 0;
						//if the counterpart has won
						if(msg.equals("w")){
							won = true;
							String tmp = ( String )input.readObject( );
							index = tmp.charAt(0) - '0';
						}
						//if it's a tie
						else if(msg.equals("t")){
							tie = true;
							String tmp = ( String )input.readObject( );
							index = tmp.charAt(0) - '0';
						}
						//if the counterpart wants to start a new game
						else if(msg.equals("n")){
							//if I also agree, then start a new game
							if(myNewGame){
								newGame();
								continue;
							}
							System.exit( -1 );
						}
						else{		
	    					index = msg.charAt(0) - '0';
						}

						System.out.println("Your counterpart marked: "  +  index);
	    				if( index >= 0 && index < 9){
	    					markButton(index, yourMark);		
	    				}

						// "9" means the end of the game
	    				if(index == 9){
							System.exit( -1 );
						}
						if(won){
							showWon(yourMark);	
						}
						else if(tie){
							showTie();
						}
						else{
							myTurn[0] = true;
							myTurn.notify(); //notify myself
						}
				
					} catch ( Exception e ) {
	    				error( e );
					}
					
				}
			}
		}
    }
}
