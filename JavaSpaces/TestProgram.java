import java.io.*;
import java.net.*;
import java.lang.reflect.*;
import java.util.*;
import java.lang.IllegalArgumentException;

public class TestProgram{
    private String multicast_group = "239.255.255.255";
    private int port = 62955;
    private JSpace.Entry entry;
    private InetAddress group = null;
    private final int READ = 0;
    private final int WRITE = 1;
    private final int TAKE = 2;
    private MulticastSocket mSocket = null; // a socket to multicast a packet
    private DatagramSocket dSocket = null;  // a socket to receive a UDP packet

    /**
     * This constructor instantiates a testprogram, and extracts arguments from
     * terminal to form an entry.
     */
    public TestProgram(String[] args){
        try {
            group = InetAddress.getByName( multicast_group );
                
            //create sockets
            mSocket = new MulticastSocket( );
            dSocket = new DatagramSocket( port );
        } catch( IOException e ) {
            e.printStackTrace( );
            System.exit( -1 );
        }
        
        if(args.length == 3 && args[0].equals("read")){
            Reader(args[0], args[1], args[2], "0");
        }
        else if(args.length == 3 && args[0].equals("take")){
            Taker(args[0], args[1], args[2], "0");
        }
        else if(args.length == 4 && args[0].equals("write")){
            Writer(args[0], args[1], args[2], args[3]);
        }
        else{
            usage();
			System.exit( -1 );
        }
    }



    /**
     * This function create an entry as the client specifies,
     * thereafter access the space to read the variable data into
     * the entry.
     *
     * @param op operation type
     * @param type variable type
     * @param name variable name
     * @param value variable value
     */
    public void Reader(String op, String type, String name, String value) {
        
        //create an entry
        setEntry(op, type, name, value);
        entry.setOperation(READ);
        
        try {
            byte[] buf = JSpace.Entry.serialize( entry ); // serialize and multicast
            DatagramPacket packet = new DatagramPacket( buf, buf.length, group, port );
            mSocket.send( packet );
				
            buf = new byte[1024];
            packet = new DatagramPacket( buf, buf.length );
            dSocket.receive( packet ); // receive an entry from a server
            entry = JSpace.Entry.deserialize( buf ); // deserialize it
            System.out.println(entry.getValue());
        } catch( Exception e ) {
            e.printStackTrace( );
	    	System.exit( -1 );
        }
    }
		
    /**
     * This function create an entry as the client specifies,
     * thereafter access the space to write an entry into the space.
     *
     * @param op operation type
     * @param type variable type
     * @param name variable name
     * @param value variable value
     */
    public void Writer(String op, String type, String name, String value) {

        //create an entry
        setEntry(op, type, name, value);
        entry.setOperation(WRITE);

        try {
	    	byte[] buf = JSpace.Entry.serialize( entry ); // serialize and multicast
	    	DatagramPacket packet = new DatagramPacket( buf, buf.length, group, port );
	   	 	mSocket.send( packet );
        } catch( IOException e ) {
	    	e.printStackTrace( );
	    	System.exit( -1 );
        }
    }
    
    /**
     * This function create an entry as the client specifies,
     * thereafter access the space to take out an entry
     *
     * @param op operation type
     * @param type variable type
     * @param name variable name
     * @param value variable value
     */
    public void Taker(String op, String type, String name, String value ) {

        //create an entry
        setEntry(op, type, name, value);
        entry.setOperation(TAKE);
        
        try {
	    	byte[] buf = JSpace.Entry.serialize( entry ); // serialize and multicast
	    	DatagramPacket packet = new DatagramPacket( buf, buf.length, group, port );
	   	 	mSocket.send( packet );
				
            buf = new byte[1024];
	    	packet = new DatagramPacket( buf, buf.length );
	    	dSocket.receive( packet ); // receive an entry from a server
	    	entry = JSpace.Entry.deserialize( buf ); // deserialize it
            System.out.println(entry.getValue());
        } catch( Exception e ) {
	    	e.printStackTrace( );
	    	System.exit( -1 );
        }
    }

    /**
     * This function create an entry as the client specifies
     *
     * @param op operation type
     * @param type variable type
     * @param name variable name
     * @param value variable value
     */
    public void setEntry(String op, String type, String name, String value){
        type = type.toLowerCase();
        entry = new JSpace.Entry( );
        
        Class enClass = entry.getClass();
        Field[] enFields = enClass.getDeclaredFields( );// extract a data field
        for ( int i = 0; i < enFields.length; i++ ) {
            enFields[i].setAccessible(true);
            String fname = enFields[i].getName( ); // get variable name
            try{
                if(fname.equals("varName")){
                    enFields[i].set(entry, name); // set the value of varName
                }
                else if(fname.equals("varType")){
                    enFields[i].set(entry, type); // set the value of varType
                }
                else if(fname.equals("value")){ // set the value of value
                    if ( type.equals( "byte" ) )
                    enFields[i].set( entry, Byte.parseByte(value) );
                    else if ( type.equals( "char" ) )
                    enFields[i].set( entry, value.charAt(0) );
                    else if ( type.equals( "short" ) )
                    enFields[i].set( entry, Short.parseShort(value) );
                    else if ( type.equals( "int" ) )
                    enFields[i].set( entry, Integer.parseInt(value) );
                    else if ( type.equals( "long" ) )
                    enFields[i].set( entry, Long.parseLong(value) );
                    else if ( type.equals( "float" ) )
                    enFields[i].set( entry, Float.parseFloat(value) );
                    else if ( type.equals( "double" ) )
                    enFields[i].set( entry, new Double(value) );
                    else if ( type.equals( "string" ) )
                    enFields[i].set( entry, value );
                    else{
                        usage();
                    }
                }
            }catch(Exception e){
                e.printStackTrace();
                usage();
                System.exit( -1 );
            }
        }
    }

    public void usage(){
        System.err.println( "usage: write type name value" );
        System.err.println( "usage: read type entryName" );
        System.err.println( "usage: take type entryName" );
        System.err.println( "type = {byte, char, short, int, long, float, double, string}" );
        System.err.println( "value should be vaild" );
    }
    
    /**
     * The actual program starts from the main( ) and instantiates a TestProgram
     * object.
     *
     * @param args nothing to pass
     */
    public static void main( String[] args ) {
				new TestProgram(args);
    }
}
