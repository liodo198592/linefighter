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
 * ����ͻ��˶��ڷ�����������
 * ������ֻ����ͨѶ���ͷ���������ҵ���߼��Ĵ���
 * @author Administrator
 *
 */
public class ServerPolicy implements Runnable{

	//��¼��������ӵ����пͻ���
	Map<Long,ClientInfo> listClients;
	ServerHandler serverHandler;
	
	ServerPolicy(){
		ServerHandler serverHandler = new ServerHandler();
	}
	
	/**
	 * ���ͻ�������״̬�����ĳ�ͻ��˶Ͽ�������ͣ���ȴ��û�����
	 * Ĭ����������
	 * @return
	 */
	public boolean CheckConnect(){
		//
		return true;
	}
	
	/**
	 * HasClient �������ӹ���
	 * @return
	 */
	public boolean AcceptClient(){
		//
		return true;
	}
	
	/**
	 * ��������
	 * @return
	 */
	public boolean Listen(){
		serverHandler.setPort(10000);
		serverHandler.startServer();
		return true;
	}
	/**
	 * ����SessionID
	 * @return
	 */
	LFUser asw_logIn(LFUser user){
		//����SessionID��������·
		return user;
	}
	
	/**
	 * ��Ӧ���������󴴽���Ӧ��Room
	 */
	Room asw_createSignleRom(LFUser user , int StageNum){
		
		//������Ϣ
		AbstractMessage sendMessage = null;
		//sendMessage.setSessionID(sessionID);
		serverHandler.sendMessage(sendMessage);
				
		return null;
	}
	
	/**
	 * ��Ӧ��������Player�Ͷ�Ӧ����Ϣ
	 */
	List<Player> asw_startGame(LFUser user , int StageNum , Room thisRoom){
		return null;
	}
	
	
	/**
	 * �ύ�Լ��Ķ�����,�ύ���󷵻� -1 ; 
	 * ���򷵻�round+1, ����ʱ��ʾ�������Ѿ�������ϱ��֣���������STEP action ��  ������ҵ�ACTIONLIST�Ͷ�Ӧ�� ��������Ժ��PLAYER ״̬
	 * 
	 */
	int asw_startRound(LFUser user , Room thisRoom , List<Action> actions){
		return thisRoom.getRoundNum() + 1;
	}
	
	/**
	 * ����������б��ּ�������StepAction;
	 * 
	 */
	Queue<StepActioin> asw_askStepAction(LFUser user, Room thisRoom){
		return null;
	}
	
	/**
	 * ����������б��ּ�������StepAction;
	 * ���ص�Key��PlayerID�� ���ص�value �Ƕ�Ӧ�����3������
	 * 
	 */
	Map<Long,List<Action>> asw_askActionList(LFUser user, Room thisRoom){
		return null;
	}
	
	/**
	 * ѯ�ʷ�����ROOM�е�ǰ��PLAYER��Ϣ
	 */
	List<Player> asw_askPlayer(LFUser user , Room thisRoom){
		return null;
	}
	
	/**
	 * �����û���Ϣ�����������Room�������Room�������򷵻�null������room�Ժ���Ը���Room����player����Ϣ����һ�ֵģ�
	 */
	Room asw_askRoom(LFUser myUser){
		return null;
	}

	/**
	 * 1. �ȴ��µ��������󣬲�ά��
	 * 2. ��ȡ�ͻ��˷��������������񣬽���������ִ��
	 */
	@Override
	public void run() {
		// TODO Auto-generated method stub
		
		//������Ϣ
		while(true){
			AbstractMessage message = serverHandler.getMessage(100);
			int sessionID = message.getSessionID();
		}
				
		
	}
}
