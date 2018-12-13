package multi;

import java.io.*;
import java.net.*;
import java.util.List;
public class ChildTh implements Runnable{
        private Socket client;
        private List<Socket> clientList;
        private String resultString;
        /**
         * 广播flag 1：广播 0：不广播
         */
        private Integer broadcastFlag = 0;
        /**
         * 客户端名称
         */
        private String clientName;
        
        public ChildTh(Socket client){
            this.client=client;
        }
        public ChildTh(Socket client,List<Socket> clientList,String resultString){
        	this.client=client;
        	this.clientList=clientList;
        	this.resultString=resultString;
        }
        
        public void run(){        
           try{        
             BufferedReader in=new BufferedReader(new InputStreamReader(client.getInputStream()));
             PrintWriter out=new PrintWriter(client.getOutputStream());
             if(!client.isClosed()){
            	 while(true){
            		 String str=in.readLine();
            		 String temp[] = str.split("=");
            		 System.out.println("child===>"+str);
            		 //out.println("has receive...."+str);
            		 //out.flush();
            		 if(str.equals("end")){
            			 break;
            		 }
            		 //发送玩家名称
            		 if("name".equals(temp[0])){
            			 clientName = temp[1];
            			 int size = resultString.split(",").length;
            			 if(size==0){
            				 resultString = resultString+"player"+(size+1)+"name";
            			 }else{
            				 resultString = resultString+","+"player"+(size+1)+"name";
            			 }
            			 broadcastFlag = 1;
            		 }
            		 //发送消息 群聊
            		 if("chat".equals(temp[0])){
            			 resultString = str;
            			 broadcastFlag = 1;
            		 }
//            		 PrintWriter out3=new PrintWriter(client.getOutputStream());
//            		 System.out.println(out3);
//            		 out3.println("has receive33333333333...."+str);
//            		 out3.flush();
            		 //广播到所有的client
                     if(broadcastFlag==1){
                    	 for(Socket s:clientList){
                    		 if(!s.isClosed()){
//                  		 BufferedReader in2=new BufferedReader(new InputStreamReader(s.getInputStream()));
                    			 PrintWriter out2=new PrintWriter(s.getOutputStream());
//                         String str2=in2.readLine();
                    			 System.out.println("guangbo===>"+resultString);
                    			 out2.println(resultString);
                    			 out2.flush();
                    		 }else{
                    			 clientList.remove(s);
                    		 }
                    	 }
                     }
            	 }        
            	 client.close();
             }else{
            	 clientList.remove(client);
             }
            }catch(Exception e){
                  System.out.println("child error in the close the socket!");                        
                  e.printStackTrace();
              }
              finally{
                                                                
              }
        }
        
}
