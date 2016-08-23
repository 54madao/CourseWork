package JSpace;
import java.io.*;
import java.net.*;
import java.util.*;
import java.util.Hashtable;

public class JSpace {
	
	private Hashtable<String, Entry> ht; // local hashtalbe to store entry
	private String command; //command from terminal
	private Thread th;
	private InetAddress group = null;
	
    /* This constructor instantiates a master JSpace server, which will then
     * spawn a SpaceThread, and read command from terminal
     *
     * @param connections JSCH connections
     * @param group multicast group
     * @param port socket port
     */
	public JSpace(Connection[] connections, String group, int port) {
		ht = new Hashtable<String, Entry>();
		try{
			this.group = InetAddress.getByName( group );		
			
            //send group, port, index, and total number of servers
			for(int i = 0; i < connections.length; i++){
                connections[i].out.writeObject(group);
                connections[i].out.writeObject(port);
                connections[i].out.writeObject(i + 1);
                connections[i].out.writeObject(connections.length + 1);
                connections[i].out.flush();
			}
			
            //distributed hash
			int l = 0;
			int r = 26 / (connections.length + 1);
            
            //new SpaceThread
			th = new Thread(new SpaceThread(this.group, port, l, r));
			th.start();
			
            //keep reading command from terminal
			while(true){
				command = new String( System.console( ).readLine( "$" ));
				if(command.equals("show")){
                    // send "show" command
					for(int i = 0; i < connections.length; i++){
						connections[i].out.writeObject("show");
						connections[i].out.flush();
					}

                    // print all hashtalbes to the terminal
					for(int i = 0; i < connections.length + 1; i++){
                        System.out.println("Slave-" + i + ":");
                        if(i == 0){
                            System.out.println(htToString());
                        }
						else{
                            System.out.println((String)(connections[i - 1].in.readObject()));
                        }
					}
				}
				else if(command.equals("quit")){
                    
                    // send "quit" command
					for(int i = 0; i < connections.length; i++){
						connections[i].out.writeObject("quit");
						connections[i].out.flush();
					}
                    
                    // count if every slave server quits
					int count = 0;
					for(int i = 0; i < connections.length; i++){
						String respone = (String)connections[i].in.readObject();
						if(respone.equals("yes")){
							count++;
						}
					}

					if(count == connections.length){
                        System.out.println("Let's quit");
					}
					System.out.println("Exit");
					break;
				}
                else{
                    System.err.println( "usage: show\n" + "\t" + "quit" );
                }
			}
		}catch(Exception e){
            e.printStackTrace( );
		}
	}

    /* This constructor instantiates a slave JSpace server, which will then
     * spawn a SpaceThread, and keep listening commands from the master
     *
     * @param connections JSCH connections
     */
	public JSpace(Connection connection) {
		ht = new Hashtable<String, Entry>();
		try{

            //receive group, port, index, and total number of servers
			String group = (String) connection.in.readObject();
			int port = (Integer) connection.in.readObject();
			int index = (Integer) connection.in.readObject();
			int num = (Integer) connection.in.readObject();
			
            // distributed hash
			int l = 26 / num * index;
			int r = index == num - 1 ? 26 : 26 / num * (index + 1);
			
			debug("gets group: " + group + "\n");
			debug("gets port: " + port + "\n");
			debug("My keys: from " + l +" to " + r + "\n");
			
			this.group = InetAddress.getByName( group );
            
            // new SpaceThread
			th = new Thread(new SpaceThread(this.group, port, l, r));
			th.start();

            // keep listenning commands from master server
			while(true){
				command = (String) connection.in.readObject();
				if(command.equals("show")){
                    // send local hashtable
					connection.out.writeObject(htToString());
				}
				else if(command.equals("quit")){
					debug("%quit%\n");
                    // send commitment to quit
					connection.out.writeObject("yes");
					connection.out.flush();
					System.exit( -1 );
				}
			}

		}catch(Exception e){
				e.printStackTrace( );
		}
	}

    
	public class SpaceThread implements Runnable {
		
		private int port;
		private InetAddress group = null;
		private MulticastSocket mSocket = null; // a socket to multicast a packet
    	private DatagramSocket dSocket = null;  // a socket to receive a UDP packet
		private Thread th;
		private int low; // lowerbound
		private int high; // upperbound

        /*
         * This constructor instantiates a SpaceThread to keep listenning UDP
         * messages from clients
         *
         * @param group multicast group
         * @param port socket port
         * @param low lowerbound of the collection of characters
         * @param low upperbound of the collection of characters
         */
		public SpaceThread(InetAddress group, int port, int low, int high) {
			debug("New SpaceThread\n");
			this.port = port;
			this.group = group;
			this.low = low;
			this.high = high;
		}
        
		public void run() {
			debug("SpaceThread is  runing!\n");
			while(true){
				try{
					mSocket = new MulticastSocket(port);
					mSocket.joinGroup(group);
					byte[] buf = new byte[1024];
					DatagramPacket packet = new DatagramPacket( buf, buf.length );
                    mSocket.receive( packet ); // receive an entry from a server
                    Entry tmp = Entry.deserialize( buf ); // deserialize it
					
					debug(tmp.getName() + " " + tmp.getType() + "\n");
			
					int initial = tmp.getName().charAt(0) - 'a';
                    debug(initial + "; " + low + "; " + high + "\n");
                    
                    //if this server should respond this packet, start a SessionThread
					if( initial >= low && initial < high ) {
						th = new Thread(new SessionThread(tmp, packet.getAddress( ), port));
						th.start();				
					}
				}catch(Exception e){
					e.printStackTrace( );
				}
			}
		}
	}

    
	public class SessionThread implements Runnable {
		private Entry entry;
		private InetAddress srcAddr;
		private int port;
        
        /*
         * This construtor instantiates a SessionThread to process entries
         *
         * @param entry the entry for processing
         * @param srcAddr source address of the packet
         * @param port socket port
         */
		public SessionThread(Entry entry, InetAddress srcAddr, int port) {
			this.entry = entry;
			this.srcAddr = srcAddr;
			this.port = port;
		}
        
		public void run() {
            debug("SessionThread is  runing!\n");
			
            //write operation
			if(entry.getOperation() == 1){
				ht.put(entry.getType() + "\t" + entry.getName(), entry);
                debug("Write: " + entry.getName() + "\n");
			}
			//read operation
			else{
				Entry tmp = null;
				while(!ht.containsKey(entry.getType() + "\t" + entry.getName()));

				tmp = ht.get(entry.getType() + "\t" + entry.getName());
                debug("Read: " + entry.getName() + "\n");
					
                //take operation
			    if(entry.getOperation() == 2){
				    ht.remove(entry.getType() + "\t" + entry.getName());
                    debug("Take: " + entry.getName() + "\n");
			    }
				
                
				try{
					byte[] buf = Entry.serialize( tmp ); // serialize and send back
                    DatagramPacket packet = new DatagramPacket( buf, buf.length, srcAddr, port );
                    DatagramSocket socket = new DatagramSocket( );
					socket.send( packet );
                    debug("Send back\n");
				}catch(Exception e){
                    e.printStackTrace( );
				}
			}				
		}
	}

    /*
     * This functions is for output debug information to the local log file
     *
     * @param str debug information
     */
	public static void debug(String str){
        try{
            File file = new File("/net/metis/home/wenbog/css533/prog2/" + InetAddress.getLocalHost( ).getHostName( ) + "_log.txt");
            FileOutputStream fos = new FileOutputStream(file,true);
            BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(fos));
            bw.write(str);
            bw.flush();
            bw.close();
        }catch(Exception e){
                e.printStackTrace( );
        }
	}
  
    /*
     * This function is to format the content of the local hashtable
     */
    public String htToString(){
        String str = "";
			if(ht.isEmpty()){
				return str;
			}
        Set<String> keys = ht.keySet();
        int count = 0;
        for(String k: keys){
            str += ("\t" + count++ + ": " + k + "\t" + ht.get(k).getValue() + "\n");
        }
        debug(str);
        return str;
    }
  
}
