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
	 * ����C++����
	 */
	private Socket serverLocal;
	/**
	 * ��Ϸserver
	 */
	private Socket serverGame;
	/**
	 * ��Ϸ����IP
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
		//��������
		serverGame=new Socket(serverIP,56789);
		//�����߳�
		ClientReceive cr = new ClientReceive(serverGame,serverLocal);
		new Thread(cr).start();
		//�����߳�
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
				//����C++����ͨѶ
				System.out.println("��ʼ���ձ���");
				BufferedReader inFromC = new BufferedReader(new InputStreamReader(serverLocal.getInputStream()));
				
				String commondFromC = inFromC.readLine();//��������
				String[] temp = commondFromC.split("="); 
				System.out.println("�յ����ģ�" + commondFromC);
				
				//��ȡ��������
				//���յ�����server�������������server
		 		if("createServer".equals(temp[0])){
		 			ServerSocket createGameServer=new ServerSocket(56789);
		 			//��¼���е�client
		 			List<Socket> clientList = new ArrayList<Socket>();
		 			String resultString = "";
		 			//�ֽ��Լ�������server ??��β�����Ƿ�������
		 			joinServer(InetAddress.getLocalHost().getHostAddress());
		 			while (true){
		 		        Socket client=createGameServer.accept();
		 		        System.out.println(client.getInetAddress()+","+client.getLocalSocketAddress());
		 		        System.out.println(client);
		 		        clientList.add(client);
		 		        ChildTh child=new ChildTh(client,clientList,resultString);//��socketʵ����ʼ������Ĵ����̶߳���
		 		        new Thread(child).start();
		 		        if(clientList.size()==4){
		 		        	//����C++�ͻ��� ready  �㲥
		 		        	for(Socket s:clientList){
		 		        		PrintWriter out=new PrintWriter(s.getOutputStream());
		 		        		out.println("ready");
		 		        		out.flush();
		 		        	}
		 		        	break;
		 		        }
		 			}
		 		}
		 		//�����Ѵ��ڵ����� joinServer=192.1.1.77
		 		if("joinServer".equals(temp[0])){
		 			System.out.println("����������" + temp[1]);
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
