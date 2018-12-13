package multi;

import java.net.*;
import java.io.*;

public class ClientSend implements Runnable{
 private Socket serverGame;
 private Socket serverLocal;
 public ClientSend(Socket server)throws Exception{
         this.serverGame= server;
 }
 public ClientSend(Socket server,Socket serverLocal)throws Exception{
	 this.serverGame= server;
	 this.serverLocal = serverLocal;
 }

	public void run() {
		try {
			//发送到其他客户端的信息
	        PrintWriter out=new PrintWriter(serverGame.getOutputStream());
	        //等待C++程序发送信息
	        BufferedReader wt=new BufferedReader(new InputStreamReader(serverLocal.getInputStream()));
	        while(true){
	           String str=wt.readLine();
	           out.println(str);
	           out.flush();
	           if(str.equals("end")){
	                    break;
	          }
	        }
	        serverGame.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
