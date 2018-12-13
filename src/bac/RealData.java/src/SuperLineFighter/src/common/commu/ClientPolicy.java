package common.commu;

import java.util.List;
import java.util.Map;
import java.util.Queue;

import client.badlogic.androidgames.sperLineFigher.view.WorderRenderer.StepActioin;

import common.commu.handler.ClientHandler;
import common.message.AbstractMessage;
import common.model.Action;
import common.model.LFUser;
import common.model.Player;
import common.model.Room;

/**
 * 代理客户端对于服务器的请求
 * 响应回调
 * @author Administrator
 *
 */
public class ClientPolicy  implements Runnable {
	
	ClientHandler messageHandler = null;
	
	ClientPolicy(){}
	
	/**
	 * 初始化连接使用
	 * @return
	 */
	public boolean initConnect(){
		messageHandler = new ClientHandler();
		messageHandler.setIp("127.0.0.1");
		messageHandler.setPort(10000);
		
		return messageHandler.initConnect();
	}
	
	/**
	 * 检查连接状态，如果连接断开则重新连接，需要手动检查，检查方式是发送一个带有自身ID的确认包，由服务器端返回确认信息
	 * 第一次连接时服务端会返回确认帧
	 * @return
	 */
	public boolean CheckConnect(){
		//
		return true;
	}
	
	/**
	 * 返回SessionID
	 * @return
	 */
	LFUser logIn(LFUser user){
		
		return user;
	}
	
	/**
	 * 实时返回： 单人游戏创建房间，返回房间对象
	 * 房间对象包含玩家人数，玩家名称，
	 * 返回为空说明房间创建失败
	 */
	Room createSignleRom(LFUser user , int StageNum){
		
		//组建创建房间消息
		
		AbstractMessage abstractMessage = null;
		
		//发送消息
		messageHandler.sendMessage(abstractMessage);
		
		//超时1秒获取消息
		AbstractMessage recvedMessage = messageHandler.getMessage(1000);
		
		if(recvedMessage != null){
			//解析消息
		}
		return null;
	}
	
	/**
	 * 开始游戏，如果返回null表示开始失败
	 * 开始成功则返回与ROOM匹配的玩家位置和血量信息
	 * 每一关结束后由由界面请求下一关
	 */
	List<Player> startGame(LFUser user , int StageNum , Room thisRoom){
		return null;
	}
	
	
	
	
	/**
	 * 提交自己的动作，,提交错误返回 -1 ; 
	 * 否则返回round+1, 返回时表示服务器已经运算完毕本轮，可以请求STEP action 和  各个玩家的ACTIONLIST和对应的 计算完毕以后的PLAYER 状态
	 * 
	 */
	int startRound(LFUser user , Room thisRoom , List<Action> actions){
		return thisRoom.getRoundNum() + 1;
	}
	
	/**
	 * 请求服务器中本轮计算过后的StepAction;
	 * 
	 */
	Queue<StepActioin> askStepAction(LFUser user,Room thisRoom){
		return null;
	}
	
	/**
	 * 请求服务器中本轮计算过后的StepAction;
	 * 返回的Key是PlayerID， 返回的value 是对应人物的3步动作
	 * 
	 */
	Map<Long,List<Action>> askActionList(LFUser user,Room thisRoom){
		return null;
	}
	
	/**
	 * 询问房间中ROOM中当前的PLAYER信息
	 */
	List<Player> askPlayer(LFUser user,Room thisRoom){
		return null;
	}
	
	/**
	 * 根据用户信息向服务器请求Room对象，如果Room不存在则返回null，请求到room以后可以根据Room请求player等信息（上一轮的）
	 */
	Room askRoom(LFUser myUser){
		return null;
	}
	

	@Override
	public void run() {
		// TODO Auto-generated method stub
		
	}
	
	//维持客户端连接,接收报文并调用对应的回调函数
	
	
}
