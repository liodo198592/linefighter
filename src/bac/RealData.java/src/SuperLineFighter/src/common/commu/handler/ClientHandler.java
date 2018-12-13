package common.commu.handler;

import java.net.InetSocketAddress;
import java.util.Date;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;

import org.apache.mina.core.future.ConnectFuture;
import org.apache.mina.core.service.IoConnector;
import org.apache.mina.core.service.IoHandlerAdapter;
import org.apache.mina.core.session.IoSession;
import org.apache.mina.filter.codec.ProtocolCodecFilter;

import common.message.AbstractMessage;


public class ClientHandler extends IoHandlerAdapter {

	/**
	 * 服务端IP
	 */
	private String ip;
	
	/**
	 * 服务端端口
	 */
	private int port;
	
	private IoSession session = null;
	private IoConnector connector = null;
	
	
	private Queue<AbstractMessage> messageQueue = new LinkedList<AbstractMessage>();
	
	
	public boolean initConnect(){
		connector.setConnectTimeoutMillis(30*1000);
		connector.getFilterChain().addLast("codec", new ProtocolCodecFilter(new MessageFactory()));
		connector.setHandler(this);
		connector.getSessionConfig().setReadBufferSize(4*1024);
		return connectToServer();
	}
	
	public boolean connectToServer(){
		ConnectFuture future = connector.connect(new InetSocketAddress(ip,port));
		future.awaitUninterruptibly();
		if(future.isDone()){
			if(!future.isConnected()){
				return false;
			}
		}else {
			return false;
		}
		
		session = future.getSession();
		if(session == null){
			return false;
		}
		
		return true;
	}
	
	@Override
    public void sessionOpened(IoSession session) throws Exception {
		this.session = session;
	}
	
	@Override
    public void sessionClosed(IoSession session) throws Exception {
		this.session = null;
		connectToServer();
	}
	
	public AbstractMessage getMessage(){
		return messageQueue.poll();
	}
	
	/**
	 * 超时获取消息，
	 * @param timeOutMilliSeconds（毫秒）
	 * @return
	 */
	public AbstractMessage getMessage(long timeOutMilliSeconds){
		
		long startTime = new Date().getTime();
		
		while(true){
			long nowTime = new Date().getTime();
			if((nowTime - startTime) > timeOutMilliSeconds){
				return null;
			}
			
			if(messageQueue.size() > 0){
				return messageQueue.poll();
			}
			
			try {
				Thread.sleep(10);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
	}
	
	@Override
    public void messageReceived(IoSession session, Object message)
            throws Exception {
		
		if(!(message instanceof AbstractMessage)){
			return;
		}
		
		messageQueue.add((AbstractMessage)message);
		
	}
	
	public void sendMessage(AbstractMessage message){
		
		if(message == null){
			return;
		}
		
		if(session != null){
			session.write(message);
		}
	}
	
	/**
	 * @return the ip
	 */
	public String getIp() {
		return ip;
	}

	/**
	 * @param ip the ip to set
	 */
	public void setIp(String ip) {
		this.ip = ip;
	}

	/**
	 * @return the port
	 */
	public int getPort() {
		return port;
	}

	/**
	 * @param port the port to set
	 */
	public void setPort(int port) {
		this.port = port;
	}
	
	
}
