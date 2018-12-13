package common.commu;

import java.util.List;
import java.util.Map;
import java.util.Queue;

import client.badlogic.androidgames.sperLineFigher.view.WorderRenderer.StepActioin;

import common.commu.handler.ServerHandler;
import common.message.AbstractMessage;
import common.model.Action;
import common.model.LFUser;
import common.model.Player;
import common.model.Room;

 /**
 * 代理客户端对于服务器的请求
 * 此类中只代理通讯，和分流不进行业务逻辑的处理
 * @author Administrator
 *
 */
public class ServerPolicy implements Runnable{

	//记录服务端连接的所有客户端
	Map<Long,ClientInfo> listClients;
	ServerHandler serverHandler;
	
	ServerPolicy(){
		ServerHandler serverHandler = new ServerHandler();
	}
	
	/**
	 * 检查客户端连接状态，如果某客户端断开连接暂停，等待用户重连
	 * 默认启动监听
	 * @return
	 */
	public boolean CheckConnect(){
		//
		return true;
	}
	
	/**
	 * HasClient ，有连接过来
	 * @return
	 */
	public boolean AcceptClient(){
		//
		return true;
	}
	
	/**
	 * 启动监听
	 * @return
	 */
	public boolean Listen(){
		serverHandler.setPort(10000);
		serverHandler.startServer();
		return true;
	}
	/**
	 * 返回SessionID
	 * @return
	 */
	LFUser asw_logIn(LFUser user){
		//设置SessionID，返回线路
		return user;
	}
	
	/**
	 * 响应服务器请求创建对应的Room
	 */
	Room asw_createSignleRom(LFUser user , int StageNum){
		
		//发送消息
		AbstractMessage sendMessage = null;
		//sendMessage.setSessionID(sessionID);
		serverHandler.sendMessage(sendMessage);
				
		return null;
	}
	
	/**
	 * 对应创建各个Player和对应的信息
	 */
	List<Player> asw_startGame(LFUser user , int StageNum , Room thisRoom){
		return null;
	}
	
	
	/**
	 * 提交自己的动作，,提交错误返回 -1 ; 
	 * 否则返回round+1, 返回时表示服务器已经运算完毕本轮，可以请求STEP action 和  各个玩家的ACTIONLIST和对应的 计算完毕以后的PLAYER 状态
	 * 
	 */
	int asw_startRound(LFUser user , Room thisRoom , List<Action> actions){
		return thisRoom.getRoundNum() + 1;
	}
	
	/**
	 * 请求服务器中本轮计算过后的StepAction;
	 * 
	 */
	Queue<StepActioin> asw_askStepAction(LFUser user, Room thisRoom){
		return null;
	}
	
	/**
	 * 请求服务器中本轮计算过后的StepAction;
	 * 返回的Key是PlayerID， 返回的value 是对应人物的3步动作
	 * 
	 */
	Map<Long,List<Action>> asw_askActionList(LFUser user, Room thisRoom){
		return null;
	}
	
	/**
	 * 询问房间中ROOM中当前的PLAYER信息
	 */
	List<Player> asw_askPlayer(LFUser user , Room thisRoom){
		return null;
	}
	
	/**
	 * 根据用户信息向服务器请求Room对象，如果Room不存在则返回null，请求到room以后可以根据Room请求player等信息（上一轮的）
	 */
	Room asw_askRoom(LFUser myUser){
		return null;
	}

	/**
	 * 1. 等待新的连接请求，并维持
	 * 2. 获取客户端发过来的请求任务，解析并分派执行
	 */
	@Override
	public void run() {
		// TODO Auto-generated method stub
		
		//接收消息
		while(true){
			AbstractMessage message = serverHandler.getMessage(100);
			int sessionID = message.getSessionID();
		}
				
		
	}
}
