package multi;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.Socket;

public class ClientReceive implements Runnable{
	/**
	 * 游戏网络socket
	 */
	private Socket serverGame;
	/**
	 * 本地socket
	 */
	private Socket serverLocal;
	
	public ClientReceive(Socket server){
		this.serverGame=server;
	}
	public ClientReceive(Socket serverGame,Socket serverLocal){
		this.serverGame=serverGame;
		this.serverLocal=serverLocal;
	}
	public void run() {
		//游戏网络层的输入
		BufferedReader in;
		//本地C++通信
		BufferedWriter out;
		try {
			in = new BufferedReader(new InputStreamReader(serverGame.getInputStream()));
			out = new BufferedWriter(new OutputStreamWriter(serverLocal.getOutputStream()));
			while(true){
//				System.out.println("ready to receive");
			    String strGame = in.readLine();
			    System.out.println(strGame);
			    //接收到游戏 命令后，转发到C++
			    out.write(strGame);
			    out.flush();
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
