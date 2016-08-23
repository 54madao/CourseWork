import Mobile.*;
import java.net.*;
/**
 * MyAgent is a test mobile agent that is injected to the 1st Mobile.Place
 * platform to print the breath message, migrates to the 2nd platform to
 * say "Hello!", and even moves to the 3rd platform to say "Oi!".
 * 
 * @author  Munehiro Fukuda
 * @version %I% %G%
 * @since   1.0
 */
public class MyTestAgent extends Agent {
    public int hopCount = 0;
    public String[] destination = null;
    
    /**
     * The consturctor receives a String array as an argument from 
     * Mobile.Inject.
     *
     * @param args arguments passed from Mobile.Inject to this constructor
     */
    public MyTestAgent( String[] args ) {
	   destination = args;
    }

    /**
     * init( ) is the default method called upon an agent inject.
     */
    public void init( ) {
	   System.out.println( "agent( " + agentId + ") invoked init: " +
			    "hop count = " + hopCount +
			    ", next dest = " + destination[hopCount] );
	   String[] args = new String[1];
	   args[0] = "Hello from init()!";
	   hop( destination[hopCount++], "step", args );
    }
    
    /**
     * step( ) is invoked upon an agent migration to destination[0] after 
     * init( ) calls hop( ).
     * 
     * @param args arguments passed from init( ).
     */
    public void step( String[] args ) {
        System.out.println( "agent( " + agentId + ") invoked step: " +
			    "hop count = " + hopCount +
			    ", next dest = " + destination[hopCount] + 
			    ", message = " + args[0] );
        args[0] = "Hello from step()!";

        //pick up a message from the local message queue
        String msg = pickFromLocal();
        if(msg != null){
            System.out.println(this.agentId + " received: \"" + msg + "\" at the local place");
        }
        else{

            //pick up a message from the message queue at destination[1]
            msg = pick(destination[1]);
            if(msg != null){
                System.out.println(this.agentId + " received: \"" + msg + "\" from " + destination[1]);
            }
        }  
        hop( destination[hopCount++], "jump", args );

    }

    /**
     * jump( ) is invoked upon an agent migration to destination[1] after
     * step( ) calls hop( ).
     *
     * @param args arguments passed from step( ).
     */
    public void jump( String[] args ) {
        System.out.println( "agent( " + agentId + ") invoked step: " +
                "hop count = " + hopCount +
                ", next dest = " + destination[hopCount] + 
                ", message = " + args[0] );
        args[0] = "Hello from jump()!";

        //pick up a message from the local message queue
        String msg = pickFromLocal();
        if(msg != null){
            System.out.println(this.agentId + " received: \"" + msg + "\" at the local place");
        }  
        hop( destination[hopCount++], "messaging", args );

    }

    /**
     * messaging( ) is invoked upon an agent migration to destination[2] after
     * jump( ) calls hop( ).
     *
     * @param args arguments passed from jump( ).
     */
    public void messaging( String[] args ) {
	   System.out.println( "agent( " + agentId + ") invoked jump: " +
			    "hop count = " + hopCount +
			    ", message = " + args[0] );

        //pick up a message from the local message queue
        String msg = pickFromLocal();
        if(msg != null){
            System.out.println(this.agentId + " received: \"" + msg + "\" at the local place");
        
            // send a message to destination[1]
            String tmp = "Hi from " + agentId;
            System.out.println(this.agentId + " send: \"" + tmp + "\" to " + destination[1]); 
            send(destination[1], tmp);
        }
        else{

            // send a message to all destinations
            String tmp = "Hello from " + agentId;
            System.out.println(this.agentId + " send: \"" + tmp + "\" to all destination nodes"); 
            sendAll(destination, tmp);
        }
    }
}