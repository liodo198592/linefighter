package LineFighter.Core;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import LineFighter.Core.Const.EACTION;
import LineFighter.Core.Const.ELIFESTAT;

import android.util.Log;

import com.example.linefighter2.MainActivity.HandlerSet;
import com.example.linefighter2.MainActivity.TextShowHandler;

public class Engine implements Runnable {
	Long m_port;
	
	HandlerSet m_TextShowHandler;
	public Engine(HandlerSet mh){
		m_TextShowHandler = mh;
	}
	
	public void run(){
		//netReady();//�ȴ���javaͨѶ�㽨������
		Const.statusFlag = 1L; //��ʼ��״̬����
		
		List<Player> vplayers = new ArrayList<Player>();
		List<List<Action>> vActions = new ArrayList<List<Action>>();
		Map<Long,List<String>> vdiscreption = new LinkedHashMap<Long,List<String>>();
		for(Long iActionIndex = 0L ; iActionIndex < Const.MAX_ACTNUM; ++iActionIndex){
			List<String> vString = new ArrayList<String>();
			vdiscreption.put(iActionIndex, vString);
		}
		
		Show sw = new Show(m_TextShowHandler);
		while(true)
		{
			
			Long RoundIndex = 0L;
			//��ʼ���ĸ���
			initGame(vplayers);
			
			while(isGameover == -1)
			{
				RoundIndex++;
				sw.showPlayer(vplayers);
				sw.showRoundIndex(RoundIndex);
				vActions.clear();
				
				for(Player pl: vplayers){
					List<Action> subVec = new ArrayList<Action>();
					subVec = pl.getAction(sw);
					Log.d("LineFighter", "Engine :: run() ��subVec.size = " + subVec.size());
					vActions.add(subVec);
				}
				sw.showComand(vActions,vplayers);
				//�ȴ��û�ȷ�Ϻ�ʼ����
				sw.waitAkey();
				
				for(Long iActionIndex = 0L ; iActionIndex < Const.MAX_ACTNUM; ++iActionIndex){
					vdiscreption.get(iActionIndex).clear();
				}
				calcARound(vplayers,vActions,vdiscreption);
				sw.showDetail(vdiscreption);
				sw.waitAkey();
//				try {
//					Thread.sleep(5000);
//				} catch (InterruptedException e) {
//					// TODO Auto-generated catch block
//					e.printStackTrace();
//				}
			}
			sw.showGameOver(vplayers.get(isGameover.intValue()),RoundIndex);
			sw.waitAkey();
			continue;
		}
	}
	
	Long waitFor(Long dest){
		return 0L;
	}
	
	Long uiReady(String name,String ip, Long isServer){
		return null;
	}
	
	Long netReady(){
		return null;
	}
	
	Long sendChat(String content){
		String sendValue = "chat=" + name + " : " + content + "\n";
	
		ptcp.send(sendValue,sendValue.length());
		return 0L;
	}
	//���ݵ�ǰ��player״̬�͵�ǰ��Action״̬������������player
	Long calcARound(List<Player> players , List<List<Action>> actions, Map<Long,List<String>> description)
	{
		Log.d("LineFighter", "calcARound :: players size = ��" + players.size());
		EACTION eActionType = EACTION.EACTION_Invalid;
		for(Long iActionIndex = 0L ; iActionIndex < Const.MAX_ACTNUM; ++iActionIndex)
		{
			for(Long iPlayerIndex = 0L ; iPlayerIndex < Const.MAX_PLAYER; ++iPlayerIndex)
			{
				Log.d("LineFighter", "calcARound :: iPlayerIndex = " + iPlayerIndex.toString());
				//�ȴ����ƶ��¼�
				if(players.get(iPlayerIndex.intValue()).getLifeStat() != ELIFESTAT.ELIFESTAT_ALIFE)
				{
					continue;
				}

			
				Long tempX = null,tempY = null,newX = null,newY = null;
				String strDescript = "";
				String startPosition; 
				String endPosition;
				eActionType = actions.get(iPlayerIndex.intValue()).get(iActionIndex.intValue()).getType();
				if(eActionType == EACTION.EACTION_Move || eActionType == EACTION.EACTION_ChangeLine)
				{
					tempX = players.get(iPlayerIndex.intValue()).getPositionX();
					tempY = players.get(iPlayerIndex.intValue()).getPositionY();
					
					newX = actions.get(iPlayerIndex.intValue()).get(iActionIndex.intValue()).movetoX(tempX);
					newY = actions.get(iPlayerIndex.intValue()).get(iActionIndex.intValue()).movetoY(tempY);
					
					strDescript = "" + players.get(iPlayerIndex.intValue()).getName() +
							"��" + tempX.toString() + "," + tempY.toString() + " " + 
							"�ƶ���" + newX.toString() + "," + newY.toString();
					Log.d("LineFighter-Show", "strDescript = " + strDescript);
					Log.d("LineFighter", "iActionIndex = " + iActionIndex.toString());
					Log.d("LineFighter", "description size = " + description.size());
					description.get(iActionIndex).add(strDescript);
					players.get(iPlayerIndex.intValue()).setPosition(newX, newY);
				}
			}
			//�������¼�
			for(Long iPlayerIndex = 0L ; iPlayerIndex < Const.MAX_PLAYER ; ++ iPlayerIndex)
			{
				//�ȴ����ƶ��¼�
				if(players.get(iPlayerIndex.intValue()).getLifeStat() != Const.ELIFESTAT.ELIFESTAT_ALIFE)
				{
					continue;
				}
				Long selfX = 0L,selfY = 0L,yourX = 0L,yourY = 0L;
				selfX = players.get(iPlayerIndex.intValue()).getPositionX();
				selfY = players.get(iPlayerIndex.intValue()).getPositionY();
				
				//��ȡ�����¼������ж϶�Ӧλ�õ���Ա
				eActionType = actions.get(iPlayerIndex.intValue()).get(iActionIndex.intValue()).getType();
				if(eActionType == EACTION.EACTION_Hand || eActionType == EACTION.EACTION_Air || 
						eActionType == EACTION.EACTION_Fall || eActionType ==EACTION.EACTION_Kill)
				{
					//ѭ����taget
					Map<Long,List<Long>> mIDs = new LinkedHashMap<Long,List<Long>>();
					Long ret = 0L;
					for(Long iSubIndex = 0L ; iSubIndex < Const.MAX_PLAYER; ++iSubIndex)
					{
						if(iSubIndex == iPlayerIndex || 
							players.get(iSubIndex.intValue()).getLifeStat() != ELIFESTAT.ELIFESTAT_ALIFE)//��ʾ���Լ�
						{
							continue;
						}
						yourX = players.get(iSubIndex.intValue()).getPositionX();
						yourY = players.get(iSubIndex.intValue()).getPositionY();
						ret = actions.get(iPlayerIndex.intValue()).get(iActionIndex.intValue()).rangeto(selfX,selfY,yourX,yourY);
						if(ret >= 0L)
						{
							if(mIDs.isEmpty())
							{
								List<Long> subList = new ArrayList<Long>();
								subList.add(iSubIndex);
								mIDs.put(ret, subList);
							}
							else
							{
								Iterator<Long> itset = (Iterator<Long>) mIDs.keySet().iterator();
								Long firstValue  = itset.next();
								
								Log.d("LineFighter", "ret = " + ret.toString() + " firstValue= " + firstValue.intValue()
										 + " mIDs.size =" + mIDs.size());
								
								if(ret < firstValue)
								{
									mIDs.clear();
									List<Long> subList = new ArrayList<Long>();
									subList.add(iSubIndex);
									mIDs.put(ret, subList);
								}
								else if(ret == firstValue)
								{
									if(mIDs.get(ret) == null){
										List<Long> subList = new ArrayList<Long>();
										subList.add(iSubIndex);
										mIDs.put(ret, subList);
									}
								}
							}
						}
					}

					Long isEmpty = 1L;
					
					Iterator<Long> itset = (Iterator<Long>) mIDs.keySet().iterator();
					while(itset.hasNext()){
						List<Long> itM = mIDs.get(itset.next());
						for(Long itV : itM){
							Long iDamege = 0l;
							iDamege = actions.get(iPlayerIndex.intValue()).get(iActionIndex.intValue())
									.damageto(actions.get(itV.intValue()).get(iActionIndex.intValue()));
							Log.d("LineFighter", "demage  = " + iDamege);
							if(iDamege == 0)
							{
								String strDescript = 
									players.get(iPlayerIndex.intValue()).getName() + "��" + 
									actions.get(iPlayerIndex.intValue()).get(iActionIndex.intValue()).getDescription() +
									 "��" + players.get(itV.intValue()).getName() +
									 actions.get(itV.intValue()).get(iActionIndex.intValue()).getDescription() + "��";
								Log.d("LineFighter-Show", "description  = " + description);
								description.get(iActionIndex).add(strDescript);
							}
							else if(iDamege > 0)
							{
								isEmpty = 0L;
								String dmg = "�˺���" + iDamege.toString();
								String strDescript =  
										players.get(iPlayerIndex.intValue()).getName() + "��" 
									+ actions.get(iPlayerIndex.intValue()).get(iActionIndex.intValue()).getDescription() +
									 "������" + players.get(itV.intValue()).getName() + dmg;
								players.get(itV.intValue()).damageHp(iDamege);
								Log.d("LineFighter-Show", "description  = " + description);
								description.get(iActionIndex).add(strDescript);
							}	
							else //-1�����
							{
								;
							}
						}
					}
					if(isEmpty == 1)//��յ����
					{	
						String strDescript =  players.get(iPlayerIndex.intValue()).getName() + "��" + 
								actions.get(iPlayerIndex.intValue()).get(iActionIndex.intValue()).getDescription() 
								+ "����ˣ�"; 
						description.get(iActionIndex).add(strDescript);
					}
				}
			}
			
			//�ж�����״̬
			for(Long iPlayerIndex = 0L ; iPlayerIndex < Const.MAX_PLAYER; ++iPlayerIndex)
			{
				if(players.get(iPlayerIndex.intValue()).checkLifeStat() == ELIFESTAT.ELIFESTAT_DEAD)
				{
					lifenum--;
					String strDescript = players.get(iPlayerIndex.intValue()).getName() + "��������";
					description.get(iActionIndex).add(strDescript);
				}
				else if(players.get(iPlayerIndex.intValue()).checkLifeStat() == ELIFESTAT.ELIFESTAT_ALIFE)
				{
					isGameover = iPlayerIndex;
				}
			}
			if(lifenum >= 2)
			{
				isGameover = -1L;//�����������δDEAD����Ϊ��δ����
			}
		}
		
		return 0L;
	}
	
	Long initGame(List<Player> players){
		
		players.clear();

		Player  p1 = new Player("PLAYER");
		p1.setPosition(1L,1L);
		Player  p2 = new Player("COM1");
		p2.setPosition(2L,1L);
		Player  p3 = new Player("COM2");
		p3.setPosition(1L,2L);
		Player  p4 = new Player("COM3");
		p4.setPosition(2L,2L);
		
		players.add(p1);
		players.add(p2);
		players.add(p3);
		players.add(p4);
		
		isGameover = -1l;
		lifenum = Const.MAX_PLAYER;
		
		return null;
	}
	
	public String serverIp;
	public String name;
	CTcpclient ptcp;
	//-1��ʾ��Ϸ��ʼ ��Ӧ��PLAYER ID��ʾ���� 
	Long isGameover;
	//���������
	Long lifenum;
	
}
