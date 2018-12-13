import java.net.*;
import java.io.*;

public class ClientC{
 static Socket server;
 
 public static void main(String[] args)throws Exception{
  server=new Socket(InetAddress.getLocalHost(),54321);
  System.out.println(server.getPort());
  BufferedReader in=new BufferedReader(new InputStreamReader(server.getInputStream()));
  PrintWriter out=new PrintWriter(server.getOutputStream());
         //标准输入
  BufferedReader wt=new BufferedReader(new InputStreamReader(System.in));
  
  while(true){
           String str=wt.readLine();
           out.println(str);
           out.flush();
           if(str.equals("end")){
        	   break;
           }
  }
  server.close();
 }
}
