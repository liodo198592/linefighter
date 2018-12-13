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
 * ����ͻ��˶��ڷ�����������
 * ��Ӧ�ص�
 * @author Administrator
 *
 */
public class ClientPolicy  implements Runnable {
	
	ClientHandler messageHandler = null;
	
	ClientPolicy(){}
	
	/**
	 * ��ʼ������ʹ��
	 * @return
	 */
	public boolean initConnect(){
		messageHandler = new ClientHandler();
		messageHandler.setIp("127.0.0.1");
		messageHandler.setPort(10000);
		
		return messageHandler.initConnect();
	}
	
	/**
	 * �������״̬��������ӶϿ����������ӣ���Ҫ�ֶ���飬��鷽ʽ�Ƿ���һ����������ID��ȷ�ϰ����ɷ������˷���ȷ����Ϣ
	 * ��һ������ʱ����˻᷵��ȷ��֡
	 * @return
	 */
	public boolean CheckConnect(){
		//
		return true;
	}
	
	/**
	 * ����SessionID
	 * @return
	 */
	LFUser logIn(LFUser user){
		
		return user;
	}
	
	/**
	 * ʵʱ���أ� ������Ϸ�������䣬���ط������
	 * �������������������������ƣ�
	 * ����Ϊ��˵�����䴴��ʧ��
	 */
	Room createSignleRom(LFUser user , int StageNum){
		
		//�齨����������Ϣ
		
		AbstractMessage abstractMessage = null;
		
		//������Ϣ
		messageHandler.sendMessage(abstractMessage);
		
		//��ʱ1���ȡ��Ϣ
		AbstractMessage recvedMessage = messageHandler.getMessage(1000);
		
		if(recvedMessage != null){
			//������Ϣ
		}
		return null;
	}
	
	/**
	 * ��ʼ��Ϸ���������null��ʾ��ʼʧ��
	 * ��ʼ�ɹ��򷵻���ROOMƥ������λ�ú�Ѫ����Ϣ
	 * ÿһ�ؽ��������ɽ���������һ��
	 */
	List<Player> startGame(LFUser user , int StageNum , Room thisRoom){
		return null;
	}
	
	
	
	
	/**
	 * �ύ�Լ��Ķ�����,�ύ���󷵻� -1 ; 
	 * ���򷵻�round+1, ����ʱ��ʾ�������Ѿ�������ϱ��֣���������STEP action ��  ������ҵ�ACTIONLIST�Ͷ�Ӧ�� ��������Ժ��PLAYER ״̬
	 * 
	 */
	int startRound(LFUser user , Room thisRoom , List<Action> actions){
		return thisRoom.getRoundNum() + 1;
	}
	
	/**
	 * ����������б��ּ�������StepAction;
	 * 
	 */
	Queue<StepActioin> askStepAction(LFUser user,Room thisRoom){
		return null;
	}
	
	/**
	 * ����������б��ּ�������StepAction;
	 * ���ص�Key��PlayerID�� ���ص�value �Ƕ�Ӧ�����3������
	 * 
	 */
	Map<Long,List<Action>> askActionList(LFUser user,Room thisRoom){
		return null;
	}
	
	/**
	 * ѯ�ʷ�����ROOM�е�ǰ��PLAYER��Ϣ
	 */
	List<Player> askPlayer(LFUser user,Room thisRoom){
		return null;
	}
	
	/**
	 * �����û���Ϣ�����������Room�������Room�������򷵻�null������room�Ժ���Ը���Room����player����Ϣ����һ�ֵģ�
	 */
	Room askRoom(LFUser myUser){
		return null;
	}
	

	@Override
	public void run() {
		// TODO Auto-generated method stub
		
	}
	
	//ά�ֿͻ�������,���ձ��Ĳ����ö�Ӧ�Ļص�����
	
	
}
