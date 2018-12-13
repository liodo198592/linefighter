package multi;

import java.net.*;
import java.io.*;

public class Client{
	static Socket server;
 
 	public static void main(String[] args)throws Exception{
 		//创建一个本地serverSocket，用来等待C++Client连接
 		ServerSocket serverLocalhost = new ServerSocket(54321);
 		System.out.println("s/c==>"+serverLocalhost.getLocalSocketAddress());
 		Socket socketC = serverLocalhost.accept();
 		System.out.println("本地C++已连接!");
 		ClientForC cfc = new ClientForC(socketC);
 		new Thread(cfc).start();
 	}
}
