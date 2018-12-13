package LineFighter.Core;

import java.io.Serializable;
import java.util.Iterator;
import java.util.List;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Map;
import java.util.Queue;

import com.badlogic.androidgames.SperLineFigher.WorderRenderer.StepActioin;

import android.R.integer;
import android.graphics.Path.Direction;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

//import com.example.linefighter2.MainActivity.HandlerSet;
//import com.example.linefighter2.MainActivity.PlayerShowHandler;
//import com.example.linefighter2.MainActivity.TextShowHandler;
public class Show {
	
	Handler myHandler;
	public List<Action> ActionListRet; 
	public Show(Handler mh){
		myHandler = mh;
		ActionListRet = new ArrayList<Action>();
	}
	
	public void showMessage(String strShow){
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
		showMessage(player.getName() + "���ջ�ʤ����NB �غ���:" + String.valueOf(index));
	}

	public void showRoundIndex(Long index){
		
		showMessage("linefighter ��" + String.valueOf(index) + "�غ�");
		
	}

	public void waitAkey(){
		
	}

	public void showDetail(Map<Long,List<String>> discreption){
		
		showMessage("==========��������===========");
		Iterator iterator = discreption.entrySet().iterator();
		//showMessage("clean");
		while(iterator.hasNext()){
			Map.Entry entry = (Map.Entry)iterator.next();
			Long ilLong = (Long)entry.getKey();
			List<String> list = (List<String>)entry.getValue();
			String totalString = new String();
			totalString += "��" + String.valueOf(ilLong + 1) + "��  ";
			
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
		
		Log.d("LineFighter", "showComand start��");
		showMessage("==========AI�������============");
		
		String strCommand;
		for(int iPlayerIndex = 0;iPlayerIndex < Const.MAX_PLAYER;iPlayerIndex++){
			if(vplayers.get(iPlayerIndex).checkLifeStat() == Const.ELIFESTAT.ELIFESTAT_ALIFE){
				strCommand = "";
				strCommand += vplayers.get(iPlayerIndex).getName() + "===>";
				for(int iActionIndex = 0;iActionIndex < Const.MAX_ACTNUM;++iActionIndex){
					strCommand += String.valueOf(iActionIndex + 1) + vActions.get(iPlayerIndex).get(iActionIndex).getDescription();
					
				}
				showMessage(strCommand);
			}
		}
		Log.d("LineFighter", "showComand leave��");
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
