package server.core;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Queue;

import server.core.AIBoots.AIFactory;
import server.core.AIBoots.LFAI;

import common.model.Action;
import common.model.ModelConst;
import common.model.ModelConst.EACTION;
import common.model.ModelConst.ELIFESTAT;
import common.model.Player;


import android.os.Handler;
import android.util.Log;
import client.badlogic.androidgames.sperLineFigher.view.WorderRenderer.SHOWACTION;
import client.badlogic.androidgames.sperLineFigher.view.WorderRenderer.StepActioin;

public class Engine implements Runnable {
	Long m_port;
	
	Handler handler;
	public Show sw;
	public Engine(Handler handler){
		this.handler = handler;
	}
	
	public void run(){
		//netReady();//等待与java通讯层建立连接
		ModelConst.statusFlag = 1L; //初始化状态变量
		
		List<Player> vplayers = new ArrayList<Player>();
		List<List<Action>> vActions = new ArrayList<List<Action>>();
		Map<Long,List<String>> vdiscreption = new LinkedHashMap<Long,List<String>>();
		for(Long iActionIndex = 0L ; iActionIndex < ModelConst.MAX_ACTNUM; ++iActionIndex){
			List<String> vString = new ArrayList<String>();
			vdiscreption.put(iActionIndex, vString);
		}
		
		sw = new Show(handler);
		while(true)
		{
			
			
			Long RoundIndex = 0L;
			//初始化四个人
			initGame(vplayers);
			sw.showPlayer(vplayers);

			while(isGameover == -1)
			{
				RoundIndex++;
				vActions.clear();
				
				for(Player pl: vplayers){
					List<Action> subVec = new ArrayList<Action>();
					subVec = pl.getAction(sw);
					Log.d("LineFighter", "Engine :: run() ！subVec.size = " + subVec.size());
					vActions.add(subVec);
				}
				
				sw.waitAkey();
				
				for(Long iActionIndex = 0L ; iActionIndex < ModelConst.MAX_ACTNUM; ++iActionIndex){
					vdiscreption.get(iActionIndex).clear();
				}
				Queue<StepActioin> stepActioins = new LinkedList<StepActioin>();
				calcARound(vplayers,vActions,vdiscreption,stepActioins);
				
				sw.showAction(stepActioins);
				sw.waitAkey();
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
	
		//ptcp.send(sendValue,sendValue.length());
		return 0L;
	}
	//根据当前的player状态和当前的Action状态来决定后续的player
	Long calcARound(List<Player> players , List<List<Action>> actions, 
			Map<Long,List<String>> description,Queue<StepActioin> stepActioins)
	{
		Log.d("LineFighter", "calcARound :: players size = ！" + players.size());
		EACTION eActionType = EACTION.EACTION_Invalid;
		for(Long iActionIndex = 0L ; iActionIndex < ModelConst.MAX_ACTNUM; ++iActionIndex)
		{
			for(Long iPlayerIndex = 0L ; iPlayerIndex < ModelConst.MAX_PLAYER; ++iPlayerIndex)
			{
				Log.d("LineFighter", "calcARound :: iPlayerIndex = " + iPlayerIndex.toString());
				//先处理移动事件
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
							"从" + tempX.toString() + "," + tempY.toString() + " " + 
							"移动到" + newX.toString() + "," + newY.toString();
					Log.d("LineFighter-Show", "strDescript = " + strDescript);
					Log.d("LineFighter", "iActionIndex = " + iActionIndex.toString());
					Log.d("LineFighter", "description size = " + description.size());
					description.get(iActionIndex).add(strDescript);
					players.get(iPlayerIndex.intValue()).setPosition(newX, newY);
					
					StepActioin stepActioin = new StepActioin();
					stepActioin.showAction = SHOWACTION.MOVE;
					stepActioin.name = players.get(iPlayerIndex.intValue()).getName();
					stepActioin.positon.set(newX, newY);
					stepActioins.offer(stepActioin);
				}
			}
			//处理攻击事件
			for(Long iPlayerIndex = 0L ; iPlayerIndex < ModelConst.MAX_PLAYER ; ++ iPlayerIndex)
			{
				//先处理移动事件
				if(players.get(iPlayerIndex.intValue()).getLifeStat() != ModelConst.ELIFESTAT.ELIFESTAT_ALIFE)
				{
					continue;
				}
				Long selfX = 0L,selfY = 0L,yourX = 0L,yourY = 0L;
				selfX = players.get(iPlayerIndex.intValue()).getPositionX();
				selfY = players.get(iPlayerIndex.intValue()).getPositionY();
				
				//获取攻击事件，并判断对应位置的人员
				eActionType = actions.get(iPlayerIndex.intValue()).get(iActionIndex.intValue()).getType();
				if(eActionType == EACTION.EACTION_Hand || eActionType == EACTION.EACTION_Air || 
						eActionType == EACTION.EACTION_Fall || eActionType ==EACTION.EACTION_Kill)
				{
					//添加攻击入stepActioins
					insertAttackStep(players.get(iPlayerIndex.intValue()), 
							actions.get(iPlayerIndex.intValue()).get(iActionIndex.intValue()),
							stepActioins);
					
					//循环找taget
					Map<Long,List<Long>> mIDs = new LinkedHashMap<Long,List<Long>>();
					Long ret = 0L;
					for(Long iSubIndex = 0L ; iSubIndex < ModelConst.MAX_PLAYER; ++iSubIndex)
					{
						if(iSubIndex == iPlayerIndex || 
							players.get(iSubIndex.intValue()).getLifeStat() != ELIFESTAT.ELIFESTAT_ALIFE)//表示是自己
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
									players.get(iPlayerIndex.intValue()).getName() + "的" + 
									actions.get(iPlayerIndex.intValue()).get(iActionIndex.intValue()).getDescription() +
									 "被" + players.get(itV.intValue()).getName() +
									 actions.get(itV.intValue()).get(iActionIndex.intValue()).getDescription() + "了";
								Log.d("LineFighter-Show", "description  = " + description);
								description.get(iActionIndex).add(strDescript);
								
								//添加闪避step
								
								Log.d("avoid", "111111111111:" + actions.get(itV.intValue()).get(iActionIndex.intValue()).etype);
								insertAvoidStep(players.get(iPlayerIndex.intValue()), 
										actions.get(itV.intValue()).get(iActionIndex.intValue()),
										stepActioins);
								
							}
							else if(iDamege > 0)
							{
								isEmpty = 0L;
								String dmg = "伤害：" + iDamege.toString();
								String strDescript =  
										players.get(iPlayerIndex.intValue()).getName() + "的" 
									+ actions.get(iPlayerIndex.intValue()).get(iActionIndex.intValue()).getDescription() +
									 "击中了" + players.get(itV.intValue()).getName() + dmg;
								players.get(itV.intValue()).damageHp(iDamege);
								Log.d("LineFighter-Show", "description  = " + description);
								description.get(iActionIndex).add(strDescript);
								
								//添加受伤step
								insertDamagedStep(players.get(itV.intValue()),
										actions.get(iPlayerIndex.intValue()).get(iActionIndex.intValue()),
										stepActioins);
							}	
							else //-1的情况
							{
								;
							}
						}
					}
					if(isEmpty == 1)//打空的情况
					{	
						String strDescript =  players.get(iPlayerIndex.intValue()).getName() + "的" + 
								actions.get(iPlayerIndex.intValue()).get(iActionIndex.intValue()).getDescription() 
								+ "打空了！"; 
						description.get(iActionIndex).add(strDescript);
					}
				}
			}
			
			//判断死亡状态
			for(Long iPlayerIndex = 0L ; iPlayerIndex < ModelConst.MAX_PLAYER; ++iPlayerIndex)
			{
				if(players.get(iPlayerIndex.intValue()).checkLifeStat() == ELIFESTAT.ELIFESTAT_DEAD)
				{
					lifenum--;
					String strDescript = players.get(iPlayerIndex.intValue()).getName() + "被击败了";
					description.get(iActionIndex).add(strDescript);
				}
				else if(players.get(iPlayerIndex.intValue()).checkLifeStat() == ELIFESTAT.ELIFESTAT_ALIFE)
				{
					isGameover = iPlayerIndex;
				}
			}
			if(lifenum >= 2)
			{
				isGameover = -1L;//两个以上玩家未DEAD则认为尚未结束
			}
		}
		
		return 0L;
	}
	
	private void insertAttackStep(Player player,Action action,Queue<StepActioin> stepActioins){
		StepActioin stepActioin = new StepActioin();
		if(action.etype == EACTION.EACTION_Hand){
			stepActioin.showAction = SHOWACTION.QUAN;
		}else if(action.etype == EACTION.EACTION_Fall){
			stepActioin.showAction = SHOWACTION.SHUAI;
		}else if(action.etype == EACTION.EACTION_Air){
			stepActioin.showAction = SHOWACTION.QIGONG;
			stepActioin.direction = action.getDerection().intValue();
		}else if(action.etype == EACTION.EACTION_Kill){
			stepActioin.showAction = SHOWACTION.BISHA;
			stepActioin.direction = action.getDerection().intValue();
		}
		
		stepActioin.name = player.getName();
		stepActioins.offer(stepActioin);
	}
	
	private void insertDamagedStep(Player player,Action action,Queue<StepActioin> stepActioins){
		StepActioin stepActioin = new StepActioin();
		stepActioin.showAction = SHOWACTION.DAMAGED;
		stepActioin.name = player.getName();
		stepActioin.blood = player.getHp().intValue();
		stepActioins.offer(stepActioin);
	}
	
	private void insertAvoidStep(Player player,Action action,Queue<StepActioin> stepActioins){
		StepActioin stepActioin = new StepActioin();
		if(action.etype == EACTION.EACTION_Avoid){
			Log.d("avoid", "222222222222222");
			stepActioin.showAction = SHOWACTION.AVOID;
			stepActioin.name = player.getName();
			stepActioins.offer(stepActioin);
		}
	}
	
	Long initGame(List<Player> players){
		
		players.clear();

		Player  p1 = new Player("PLAYER",AIFactory.getInstance().createAI("LFAILV0"));
		p1.setPosition(1L,1L);
		Player  p2 = new Player("COM1",AIFactory.getInstance().createAI("LFAILV0"));
		p2.setPosition(2L,1L);
		Player  p3 = new Player("COM2",AIFactory.getInstance().createAI("LFAILV0"));
		p3.setPosition(1L,2L);
		Player  p4 = new Player("COM3",AIFactory.getInstance().createAI("LFAILV0"));
		p4.setPosition(2L,2L);
		
		players.add(p1);
		players.add(p2);
		players.add(p3);
		players.add(p4);
		
		isGameover = -1l;
		lifenum = ModelConst.MAX_PLAYER;
		
		return null;
	}
	
	public String serverIp;
	public String name;
	//-1表示游戏开始 对应的PLAYER ID表示结束 
	Long isGameover;
	//存活的玩家数
	Long lifenum;
	
}
