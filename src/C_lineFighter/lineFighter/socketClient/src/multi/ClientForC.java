package multi;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.List;

public class ClientForC implements Runnable{
	/**
	 * 本地C++程序
	 */
	private Socket serverLocal;
	/**
	 * 游戏server
	 */
	private Socket serverGame;
	/**
	 * 游戏主机IP
	 */
	private String serverGameIP;
	
	public ClientForC(Socket server){
		this.serverLocal = server;
	}
	/**
	 * join game server
	 * @param serverIP
	 * @throws Exception
	 */
	public void joinServer(String serverIP) throws Exception{
		serverGameIP = serverIP;
		//连接主机
		serverGame=new Socket(serverIP,56789);
		//接收线程
		ClientReceive cr = new ClientReceive(serverGame,serverLocal);
		new Thread(cr).start();
		//发送线程
		ClientSend cs = new ClientSend(serverGame,serverLocal);
		new Thread(cs).start();
	}
	/* (non-Javadoc)
	 * @see java.lang.Runnable#run()
	 */
	/* (non-Javadoc)
	 * @see java.lang.Runnable#run()
	 */
	public void run() {
		try {
			while(true){
				//本地C++程序通讯
				System.out.println("开始接收报文");
				BufferedReader inFromC = new BufferedReader(new InputStreamReader(serverLocal.getInputStream()));
				
				String commondFromC = inFromC.readLine();//阻塞方法
				String[] temp = commondFromC.split("="); 
				System.out.println("收到报文：" + commondFromC);
				
				//获取本地命令
				//接收到创建server的命令，创建本地server
		 		if("createServer".equals(temp[0])){
		 			ServerSocket createGameServer=new ServerSocket(56789);
		 			//记录所有的client
		 			List<Socket> clientList = new ArrayList<Socket>();
		 			String resultString = "";
		 			//现将自己连接上server ??这段不清楚是否有问题
		 			joinServer(InetAddress.getLocalHost().getHostAddress());
		 			while (true){
		 		        Socket client=createGameServer.accept();
		 		        System.out.println(client.getInetAddress()+","+client.getLocalSocketAddress());
		 		        System.out.println(client);
		 		        clientList.add(client);
		 		        ChildTh child=new ChildTh(client,clientList,resultString);//用socket实例初始化具体的处理线程对象
		 		        new Thread(child).start();
		 		        if(clientList.size()==4){
		 		        	//返回C++客户端 ready  广播
		 		        	for(Socket s:clientList){
		 		        		PrintWriter out=new PrintWriter(s.getOutputStream());
		 		        		out.println("ready");
		 		        		out.flush();
		 		        	}
		 		        	break;
		 		        }
		 			}
		 		}
		 		//连接已存在的主机 joinServer=192.1.1.77
		 		if("joinServer".equals(temp[0])){
		 			System.out.println("连接主机：" + temp[1]);
		 			joinServer(temp[1]);
		 		}else{
		 			PrintWriter outToServerGame=new PrintWriter(serverGame.getOutputStream());
		 			outToServerGame.println(commondFromC);
		 			outToServerGame.flush();
		 		}
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
