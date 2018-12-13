import java.net.*;
import java.io.*;
import java.lang.*;

public class ClientApp {
	public static void main(String args[]) {
		try {
			// 创建通讯并且和主机Rock连接
			Socket cSocket = new Socket(InetAddress.getLocalHost(), 8019);
			// 打开这个Socket的输入/输出流
			OutputStream os = cSocket.getOutputStream();
			DataInputStream is = new DataInputStream(cSocket.getInputStream());

			int c;
			boolean flag = true;

			String responseline;

			while (flag) {
				// 从标准输入输出接受字符并且写如系统
				while ((c = System.in.read()) != -1) {
					os.write((byte) c);
					if (c == '\n') {
						os.flush();
						// 将程序阻塞，直到回答信息被收到后将他们在标准输出上显示出来
						responseline = is.readLine();
						System.out.println("Message is:" + responseline);
					}
				}
			}
			os.close();
			is.close();
			cSocket.close();

		} catch (Exception e) {
			System.out.println("Exception :" + e.getMessage());
		}
	}
}