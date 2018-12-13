package common.commu.handler;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.util.Date;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.Map;
import java.util.Queue;

import org.apache.mina.core.service.IoAcceptor;
import org.apache.mina.core.service.IoHandlerAdapter;
import org.apache.mina.core.session.IoSession;
import org.apache.mina.filter.codec.ProtocolCodecFilter;
import org.apache.mina.transport.socket.nio.NioSocketAcceptor;

import android.R.string;

import common.message.AbstractMessage;

public class ServerHandler extends IoHandlerAdapter {

	private int port;
	private IoAcceptor acceptor = null;


	/**
	 * sessionID计数器
	 */
	private int sessionIdInex = 1;
	
	
	/**
	 * key:sessionId 	value:session
	 */
	private Map<Integer, IoSession> sessionMap = new LinkedHashMap<Integer, IoSession>();
	
	private Queue<AbstractMessage> messageQueue = new LinkedList<AbstractMessage>();
	
	
	public boolean startServer(){
		acceptor = new NioSocketAcceptor();
		acceptor.getFilterChain().addLast("codec", new ProtocolCodecFilter(new MessageFactory()));
		acceptor.setHandler(this);
		acceptor.getSessionConfig().setReadBufferSize(2048);
		try {
			acceptor.bind(new InetSocketAddress(port));
		} catch (IOException e) {
			// TODO: handle exception
			e.printStackTrace();
			return false;
		}
		
		return true;
	}
	
	@Override
	public void sessionOpened(IoSession session) throws Exception {
		sessionMap.put(sessionIdInex++, session);
	}
	
	@Override
	public void sessionClosed(IoSession session) throws Exception {
		if(sessionMap.containsValue(session)){
			for(Map.Entry<Integer, IoSession> entry:sessionMap.entrySet()){
				if(entry.getValue().equals(session)){
					sessionMap.remove(entry.getKey());
					return;
				}
			}
		}
	}
	
	private Integer getSessionIdBySession(IoSession session){
		for(Map.Entry<Integer, IoSession> entry:sessionMap.entrySet()){
			if(entry.getValue().equals(session)){
				return entry.getKey();
			}
		}
		
		return null;
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
		
		AbstractMessage abstractMessage = (AbstractMessage)message;
		
		Integer sessionId = getSessionIdBySession(session);
		
		if(sessionId == null){
			return;
		}
		
		abstractMessage.setSessionID(sessionId);
		
		messageQueue.add(abstractMessage);
		
	}
	
	public void sendMessage(AbstractMessage message){
		
		int sessionId = message.getSessionID();
		
		if(sessionMap.containsKey(sessionId)){
			IoSession session = sessionMap.get(sessionId);
			
			session.write(message);
		}
	}
	
	
	/**
	 * 用sessionid获取ip地址
	 * @param sessionId
	 * @return
	 */
	public String getIpBySessionId(Integer sessionId){
		if(!sessionMap.containsKey(sessionId)){
			return null;
		}
		
		IoSession session = sessionMap.get(sessionId);
		
		String ip = session.getRemoteAddress().toString();
		
		String[] strings1 = ip.split("/");
		if(strings1.length != 2){
			return null;
		}
		
		String[] strings2 = strings1[1].split(":");
		if(strings2.length != 2){
			return null;
		}
		
		return strings2[0];
	}
	
	/**
	 * 用sessionId获取端口
	 * @param sessionId
	 * @return
	 */
	public Integer getPortBySessionId(Integer sessionId){
		if(!sessionMap.containsKey(sessionId)){
			return null;
		}
		
		IoSession session = sessionMap.get(sessionId);
		
		String addressDesc = session.getRemoteAddress().toString();
		
		String[] strings1 = addressDesc.split("/");
		if(strings1.length != 2){
			return null;
		}
		
		String[] strings2 = strings1[1].split(":");
		if(strings2.length != 2){
			return null;
		}
		return Integer.parseInt(strings2[1]);
		
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
