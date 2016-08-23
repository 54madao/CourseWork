package Mobile;
import java.io.*;
import java.rmi.*;
import java.lang.reflect.*;

/**
 * Mobile.Agent is the base class of all user-define mobile agents. It carries
 * an agent identifier, the next host IP and port, the name of the function to
 * invoke at the next host, arguments passed to this function, its class name,
 * and its byte code. It runs as an independent thread that invokes a given
 * function upon migrating the next host.
 *
 * @author  Munehiro Fukuda
 * @version %I% %G%
 * @since   1.0
 */
public class Agent implements Serializable, Runnable {
    // live data to carry with the agent upon a migration
    protected int agentId        = -1;    // this agent's identifier
    private String _hostname     = null;  // the next host name to migrate
    private String _function     = null;  // the function to invoke upon a move
    private int _port            = 0;     // the next host port to migrate
    private String[] _arguments  = null;  // arguments pass to _function
    private String _classname    = null;  // this agent's class name
    private byte[] _bytecode     = null;  // this agent's byte code

    /**
     * setPort( ) sets a port that is used to contact a remote Mobile.Place.
     * 
     * @param port a port to be set.
     */
    public void setPort( int port ) {
	this._port = port;
    }

    /**
     * setId( ) sets this agent identifier: agentId.
     *
     * @param id an idnetifier to set to this agent.
     */
    public void setId( int id ) {
	this.agentId = id;
    }

    /**
     * getId( ) returns this agent identifier: agentId.
     *
     * @param this agent's identifier.
     */
    public int getId( ) {
	return agentId;
    }

    /**
     * getByteCode( ) reads a byte code from the file whosename is given in
     * "classname.class".
     *
     * @param classname the name of a class to read from local disk.
     * @return a byte code of a given class.
     */
    public static byte[] getByteCode( String classname ) {
	// create the file name
	String filename = classname + ".class";

	// allocate the buffer to read this agent's bytecode in
	File file = new File( filename );
	byte[] bytecode = new byte[( int )file.length( )];

	// read this agent's bytecode from the file.
	try {
	    BufferedInputStream bis =
		new BufferedInputStream( new FileInputStream( filename ) );
	    bis.read( bytecode, 0, bytecode.length );
	    bis.close( );
	} catch ( Exception e ) {
	    e.printStackTrace( );
	    return null;
	}

	// now you got a byte code from the file. just return it.
	return bytecode;	
    }

    /**
     * getByteCode( ) reads this agent's byte code from the corresponding file.
     *
     * @return a byte code of this agent.
     */
    public byte[] getByteCode( ) {
	if ( _bytecode != null ) // bytecode has been already read from a file
	    return _bytecode; 
	
	// obtain this agent's class name and file name
	_classname = this.getClass( ).getName( );
	_bytecode = getByteCode( _classname );

	return _bytecode;
    }

    /**
     * run( ) is the body of Mobile.Agent that is executed upon an injection
     * or a migration as an independent thread. run( ) identifies the method 
     * with a given function name and arguments and invokes it. The invoked
     * method may include hop( ) that transfers this agent to a remote host or
     * simply returns back to run( ) that termiantes the agent.
     */
    public void run( ) {
        
        Method func = null;
        // Find the method to invoke
        try{
            if(_arguments == null){
                func = this.getClass().getMethod(_function);
                func.invoke(this);
            }
            else{
                func = this.getClass().getMethod(_function, new Class[]{String[].class});
                // Invoke this method
                func.invoke(this, new Object[]{_arguments});
            }
        } catch(InvocationTargetException e){
            //System.out.println(e.getCause().getMessage());
        
        } catch (Exception e){
            e.printStackTrace( );
            System.exit( -1 );
        }

        
    }

    /**
     * hop( ) transfers this agent to a given host, and invoeks a given
     * function of this agent.
     *
     * @param hostname the IP name of the next host machine to migrate
     * @param function the name of a function to invoke upon a migration
     */    
    public void hop( String hostname, String function ) {
	   hop( hostname, function, null );
    }

    /**
     * hop( ) transfers this agent to a given host, and invoeks a given
     * function of this agent as passing given arguments to it.
     *
     * @param hostname the IP name of the next host machine to migrate
     * @param function the name of a function to invoke upon a migration
     * @param args     the arguments passed to a function called upon a 
     *                 migration.
     */
    @SuppressWarnings( "deprecation" )
    public void hop( String hostname, String function, String[] args ) {
        // set next host to migrate
        _hostname = hostname;
        _function = function;
        _arguments = args;

        //Load this agent’s byte code into the memory
        byte[] bytecode = getByteCode( );

        // Serialize this agent into a byte array
        byte[] entity = serialize();

        // Find a remote place
        try {
            PlaceInterface placeObject =  ( PlaceInterface )
            Naming.lookup( "rmi://" + hostname + ":" + _port + "/server" );
            //Invoke an RMI call.
            if(placeObject.transfer( _classname, bytecode, entity )){
                // kill this agent
                Thread.currentThread( ).stop();
            }
        }
        catch ( Exception e ) {
            e.printStackTrace( );
            System.exit( -1 );
        }
    }

    /**
     * serialize( ) serializes this agent into a byte array.
     *
     * @return a byte array to contain this serialized agent.
     */
    private byte[] serialize( ) {
	try {
	    // instantiate an object output stream.
	    ByteArrayOutputStream out = new ByteArrayOutputStream( );
	    ObjectOutputStream os = new ObjectOutputStream( out );
	    
	    // write myself to this object output stream
	    os.writeObject( this );

	    return out.toByteArray( ); // conver the stream to a byte array
	} catch ( IOException e ) {
	    e.printStackTrace( );
	    return null;
	}
    }

    /**
     * sendAll( ) send a message to all destination nodes
     *
     * @param des host names of all destination nodes
     * @param msg the message to be sent
     *
     */
    public void sendAll(String[] des, String msg){
        for(int i = 0; i < des.length; i++){
            try {
                //System.out.println("rmi://" + des[i] + ":" + _port + "/server");
                PlaceInterface placeObject =  ( PlaceInterface )
                Naming.lookup( "rmi://" + des[i] + ":" + _port + "/server" );
                //Invoke an RMI call.
                placeObject.saveMsg(msg);
            }
            catch ( Exception e ) {
                e.printStackTrace( );
                System.exit( -1 );
            }
        }
    }

    /**
     * pickFromLocal( ) pick up a message from the local message queue.
     *
     * @return a message in the front of the local message queue.
     */
    public String pickFromLocal(){
        String str = null;
        try {
            //System.out.println("rmi://" + des[i] + ":" + _port + "/server");
            PlaceInterface placeObject =  ( PlaceInterface )
            Naming.lookup( "rmi://localhost" + ":" + _port + "/server" );
            //Invoke an RMI call.
            str = placeObject.pickMsg();
        }
        catch ( Exception e ) {
            e.printStackTrace( );
            System.exit( -1 );
        }
        return str;
    }

    /**
     * sendA( ) send a message to a specific place
     *
     * @param des host name of the specific place
     * @param msg the message to be sent
     *
     */
    public void send(String des, String msg){
        try {
            //System.out.println("rmi://" + des[i] + ":" + _port + "/server");
            PlaceInterface placeObject =  ( PlaceInterface )
            Naming.lookup( "rmi://" + des + ":" + _port + "/server" );
            //Invoke an RMI call.
            placeObject.saveMsg(msg);
        }
        catch ( Exception e ) {
            e.printStackTrace( );
            System.exit( -1 );
        }
    }

    /**
     * pick( ) pick up a message from the message queue at a
     * specific place
     *
     * @return a message in the front of the message queue
     * at the specific place
     */
    public String pick(String des){
        String str = null;
        try {
            //System.out.println("rmi://" + des[i] + ":" + _port + "/server");
            PlaceInterface placeObject =  ( PlaceInterface )
            Naming.lookup( "rmi://" + des + ":" + _port + "/server" );
            //Invoke an RMI call.
            str = placeObject.pickMsg();
        }
        catch ( Exception e ) {
            e.printStackTrace( );
            System.exit( -1 );
        }
        return str;
    }
}