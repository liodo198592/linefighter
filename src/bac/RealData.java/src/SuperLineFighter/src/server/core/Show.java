package server.core;

import java.io.Serializable;
import java.util.Iterator;
import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.Queue;

import common.model.Action;
import common.model.ModelConst;
import common.model.Player;

import client.badlogic.androidgames.sperLineFigher.view.WorderRenderer.StepActioin;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

/**
 * 原始的	Console展示类，现在已经废弃
 * @author liodo
 *
 */
public class Show {
	
	Handler myHandler;
	public List<Action> ActionListRet; 
	public Show(Handler mh){
		myHandler = mh;
		ActionListRet = new ArrayList<Action>();
	}
	
	public void showMessage(String strShow){
		
		Log.d("LineFighter", "Engine :: ShowMessage " + strShow);
		Message msg = myHandler.obtainMessage();
		Bundle b = new Bundle();
		
		b.putString("value", strShow);
		msg.setData(b);
		myHandler.sendMessage(msg);
	}
	
	public void startGetCommand(String strShow){
		Message msg = myHandler.obtainMessage();
		Bundle b = new Bundle();
		
		b.putString("value", strShow);
		msg.setData(b);
		myHandler.sendMessage(msg);
	}
	
	public void showPlayer(List<Player> players){
		
		Message msg = myHandler.obtainMessage();
		Bundle bundle = new Bundle();
		
		bundle.putSerializable("players", (Serializable)players);
		msg.setData(bundle);
		
		myHandler.sendMessage(msg);
		
		
	}

	public void LcmSetCursor(Long byRow, Long byCol){
		
	}

	public void showGameOver(Player  player,Long index){
		
		showMessage("clean");
		showMessage(player.getName() + "最终获胜！真NB 回合数:" + String.valueOf(index));
	}

	public void showRoundIndex(Long index){
		
		showMessage("linefighter 第" + String.valueOf(index) + "回合");
		
	}

	public void waitAkey(){
		
	}

	public void showDetail(Map<Long,List<String>> discreption){
		
		showMessage("==========过程演算===========");
		Iterator iterator = discreption.entrySet().iterator();
		//showMessage("clean");
		while(iterator.hasNext()){
			Map.Entry entry = (Map.Entry)iterator.next();
			Long ilLong = (Long)entry.getKey();
			List<String> list = (List<String>)entry.getValue();
			String totalString = new String();
			totalString += "第" + String.valueOf(ilLong + 1) + "步  ";
			
			for(String str:list){
				totalString += " " + str;
			}
			showMessage(totalString);
		}
	}

	public void showAction(Queue<StepActioin> stepActioins){
		Message msg = myHandler.obtainMessage();
		Bundle bundle = new Bundle();
		
		bundle.putSerializable("showActions", (Serializable)stepActioins);
		msg.setData(bundle);
		
		myHandler.sendMessage(msg);
	}
	public void showComand(List<List<Action> > vActions,List<Player> vplayers){
		
		Log.d("LineFighter", "showComand start！");
		showMessage("==========AI随机命令============");
		
		String strCommand;
		for(int iPlayerIndex = 0;iPlayerIndex < ModelConst.MAX_PLAYER;iPlayerIndex++){
			if(vplayers.get(iPlayerIndex).checkLifeStat() == ModelConst.ELIFESTAT.ELIFESTAT_ALIFE){
				strCommand = "";
				strCommand += vplayers.get(iPlayerIndex).getName() + "===>";
				for(int iActionIndex = 0;iActionIndex < ModelConst.MAX_ACTNUM;++iActionIndex){
					strCommand += String.valueOf(iActionIndex + 1) + vActions.get(iPlayerIndex).get(iActionIndex).getDescription();
					
				}
				showMessage(strCommand);
			}
		}
		Log.d("LineFighter", "showComand leave！");
	}

	public List<Action> getActionFromConsole(Player  player){
		
		
		Message msg = myHandler.obtainMessage();
		Bundle b = new Bundle();
		
		b.putString("input", "command");
		msg.setData(b);
		myHandler.sendMessage(msg);
		
		while(ActionListRet.isEmpty()){
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		List<Action> list = new ArrayList<Action>(ActionListRet);
		ActionListRet.clear();
		//myHandler.getActionListRet().clear();
		
		Message msgClean = myHandler.obtainMessage();
		Bundle bClean = new Bundle();
		
		bClean.putString("input", "clean");
		msgClean.setData(bClean);
		myHandler.sendMessage(msgClean);
		while(!ActionListRet.isEmpty()){
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
		return list;
	}
}
