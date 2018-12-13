package LineFighter.Core;

import java.io.Serializable;
import java.util.Iterator;
import java.util.List;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Map;

import android.R.integer;
import android.graphics.Path.Direction;
import android.os.Bundle;
import android.os.Message;
import android.util.Log;

import com.example.linefighter2.MainActivity.HandlerSet;
import com.example.linefighter2.MainActivity.PlayerShowHandler;
import com.example.linefighter2.MainActivity.TextShowHandler;
public class Show {
	
	HandlerSet myHandler;
	
	public Show(HandlerSet mh){
		myHandler = mh;
	}
	
	public void showMessage(String strShow){
		Message msg = myHandler.getMyHandler().obtainMessage();
		Bundle b = new Bundle();
		
		b.putString("value", strShow);
		msg.setData(b);
		myHandler.getMyHandler().sendMessage(msg);
	}
	
	public void startGetCommand(String strShow){
		Message msg = myHandler.getM_inputHandler().obtainMessage();
		Bundle b = new Bundle();
		
		b.putString("value", strShow);
		msg.setData(b);
		myHandler.getMyHandler().sendMessage(msg);
	}
	
	public void showPlayer(List<Player> players){
		
		Message msg = myHandler.getPlayerShowHandler().obtainMessage();
		Bundle bundle = new Bundle();
		
		bundle.putSerializable("players", (Serializable)players);
		msg.setData(bundle);
		
		myHandler.getPlayerShowHandler().sendMessage(msg);
		
		
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

	public void showComand(List<List<Action> > vActions,List<Player> vplayers){
		
		Log.d("LineFighter", "showComand start！");
		showMessage("==========AI随机命令============");
		
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
		Log.d("LineFighter", "showComand leave！");
	}

	public List<Action> getActionFromConsole(Player  player){
		
		
		Message msg = myHandler.getM_inputHandler().obtainMessage();
		Bundle b = new Bundle();
		
		b.putString("input", "command");
		msg.setData(b);
		myHandler.getM_inputHandler().sendMessage(msg);
		
		while(myHandler.getActionListRet().isEmpty()){
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		List<Action> list = new ArrayList<Action>(myHandler.getActionListRet());
		//myHandler.getActionListRet().clear();
		
		Message msgClean = myHandler.getM_inputHandler().obtainMessage();
		Bundle bClean = new Bundle();
		
		bClean.putString("input", "clean");
		msgClean.setData(bClean);
		myHandler.getM_inputHandler().sendMessage(msgClean);
		while(!myHandler.getActionListRet().isEmpty()){
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
