package multi;

import java.net.*;
import java.io.*;

public class Client{
	static Socket server;
 
 	public static void main(String[] args)throws Exception{
 		//����һ������serverSocket�������ȴ�C++Client����
 		ServerSocket serverLocalhost = new ServerSocket(54321);
 		System.out.println("s/c==>"+serverLocalhost.getLocalSocketAddress());
 		Socket socketC = serverLocalhost.accept();
 		System.out.println("����C++������!");
 		ClientForC cfc = new ClientForC(socketC);
 		new Thread(cfc).start();
 	}
}
