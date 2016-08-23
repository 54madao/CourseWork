package Mobile;

import java.io.*;
import java.util.*;
import java.net.*;
import java.rmi.*;
import java.rmi.server.*;
import java.rmi.registry.*;

/**
 * Mobile.Place is the our mobile-agent execution platform that accepts an
 * agent transferred by Mobile.Agent.hop( ), deserializes it, and resumes it
 * as an independent thread.
 *
 * @author  Munehiro Fukuda
 * @version %I% %G$
 * @since   1.0
 */
public class Place extends UnicastRemoteObject implements PlaceInterface {
    private AgentLoader loader = null;  // a loader to define a new agent class
    private int agentSequencer = 0;     // a sequencer to give a unique agentId
    private Queue<String> msg; // save messages sent to this place
    /**
     * This constructor instantiates a Mobiel.AgentLoader object that
     * is used to define a new agen class coming from remotely.
     */
    public Place( ) throws RemoteException {
	super( );
	loader = new AgentLoader( );
    msg = new LinkedList<String>();
    }

    /**
     * deserialize( ) deserializes a given byte array into a new agent.
     *
     * @param buf a byte array to be deserialized into a new Agent object.
     * @return a deserialized Agent object
     */
    private Agent deserialize( byte[] buf ) 
	throws IOException, ClassNotFoundException {
	// converts buf into an input stream
        ByteArrayInputStream in = new ByteArrayInputStream( buf );

	// AgentInputStream identify a new agent class and deserialize
	// a ByteArrayInputStream into a new object
        AgentInputStream input = new AgentInputStream( in, loader );
        return ( Agent )input.readObject();
    }

    /**
     * transfer( ) accepts an incoming agent and launches it as an independent
     * thread.
     *
     * @param classname The class name of an agent to be transferred.
     * @param bytecode  The byte code of  an agent to be transferred.
     * @param entity    The serialized object of an agent to be transferred.
     * @return true if an agent was accepted in success, otherwise false.
     */
    public boolean transfer( String classname, byte[] bytecode, byte[] entity )
	throws RemoteException {
        try{
            // Register this calling agent’s classname and bytecode into AgentLoader
            Class agentClass = loader.loadClass(classname, bytecode);

            // Deserialize this agent’s entity
            Agent agent = deserialize(entity);

            //Set this agent’s identifier with a combination of the Place Hostname and a sequence number
            if(agent.getId() == -1){
                String[] addr = (InetAddress.getLocalHost( ).getHostAddress()).split("\\.");
                int id  = 0;
                for(String s : addr){
                    id += Integer.parseInt(s);
                }
                id *= 10;
                id += agentSequencer++;
                agent.setId(id);
            }

            // Instantiate a Thread Object as passing the deserialized agent
            // Invoke this Thread's start() method
            new Thread(agent).start();

            return true;
        } catch ( Exception e ) {
            e.printStackTrace( );
            return false;
        }

    }

    public boolean saveMsg(String msg){
        this.msg.offer(msg);
        return true;
    }

    public String pickMsg(){
        return msg.poll();
    }

    /**
     * main( ) starts an RMI registry in local, instantiates a Mobile.Place
     * agent execution platform, and registers it into the registry.
     *
     * @param args receives a port, (i.e., 5001-65535).
     */
    public static void main( String args[] ) {
	
	// check args validity
	int port = 0;
	try {
	    if ( args.length == 1 ) {
		port = Integer.parseInt( args[0] );
		if ( port < 5001 || port > 65535 )
		    throw new Exception( );
	    }
	    else 
		throw new Exception( );
	} catch ( Exception e ) {
	    System.err.println( "usage: java Server port" );
	    System.exit( -1 );
	}

	// invoke startRegistry(int port)
	try {     
	    startRegistry( port );
	    Place place = new Place( );
	    Naming.rebind( "rmi://localhost:" + port + "/server", 
			   place );
	    System.out.println( "Place ready." );
	} catch ( Exception e ) {
	    e.printStackTrace( );
	    System.exit( -1 );
	}
    }
    
    /**
     * startRegistry( ) starts an RMI registry process in local to this Place.
     * 
     * @param port the port to which this RMI should listen.
     */
    private static void startRegistry( int port ) throws RemoteException {
        try {
            Registry registry =
                LocateRegistry.getRegistry( port );
            registry.list( );
        }
        catch ( RemoteException e ) {
            Registry registry =
                LocateRegistry.createRegistry( port );
        }
    }
}
