package client.badlogic.androidgames.sperLineFigher.control;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Queue;

import common.model.Action;
import common.model.Player;

import server.core.Engine;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;

import client.badlogic.androidgames.sperLineFigher.control.BtnHandler.BTNTYPE;
import client.badlogic.androidgames.sperLineFigher.control.playeraction.PlayerAction;
import client.badlogic.androidgames.sperLineFigher.model.CoordinateManager;
import client.badlogic.androidgames.sperLineFigher.model.FighterObject;
import client.badlogic.androidgames.sperLineFigher.resource.Assests.PLAYERICON;
import client.badlogic.androidgames.sperLineFigher.view.WorderRenderer;
import client.badlogic.androidgames.sperLineFigher.view.WorderRenderer.SHOWACTION;
import client.badlogic.androidgames.sperLineFigher.view.WorderRenderer.StepActioin;

import client.badlogic.androidgames.framework.math.Vector2;


public class World extends Handler{

	public enum WordState{
		waitingNextRound,
		inputting,
		waitingActions,
		showing
	}

	WordState wordState = WordState.waitingNextRound;
	String myNameString;
	
	
	//HashMap<String, FighterObject> fighterHashMap;
	List<FighterObject> fighterObjects;
	public CoordinateManager coordinateManager;
	
	Queue<StepActioin> stepActioins = new LinkedList<StepActioin>();
	
	int showingRound = 0;
	boolean canChangeToInputing = false;
	
	Engine engine;
	public World(Looper mainLooper){
		super(mainLooper);
		init();
		engine = new Engine(this);
		new Thread(engine).start();

	}
	
	public void init(){
		fighterObjects = new ArrayList<FighterObject>();
		
		coordinateManager = new CoordinateManager();
	}
	
	private void initPlayers(List<Player> players){
		
		setMyName("PLAYER");
		Vector2 position = null;
		int positionIndex = 0;
		int playerIndex = 1;
		for(Player player:players){
			positionIndex = (player.getPositionX().intValue() - 1)*3 + (player.getPositionY().intValue() - 1);
			if(player.getName().equals(myNameString)){
				position = coordinateManager.centorPositonCordinates[positionIndex];
				FighterObject fighterObject = new FighterObject(player.getName(), position.x, position.y, 
						FighterObject.playerIconWidth, FighterObject.playerIconHeight);
				fighterObject.setZhiye(PLAYERICON.LUSANJIN);
				fighterObject.detaiPositionIndex = 0;
				addFighter(fighterObject);
			}else {
				
				if(playerIndex == 1){
					position = coordinateManager.topPositionCordinates[positionIndex];
					FighterObject fighterObject = new FighterObject(player.getName(), position.x, position.y, 
							FighterObject.playerIconWidth, FighterObject.playerIconHeight);
					fighterObject.setZhiye(PLAYERICON.WENLIANGGONG);
					fighterObject.detaiPositionIndex = 1;
					addFighter(fighterObject);
					playerIndex++;
				}else if(playerIndex == 2){
					position = coordinateManager.leftBottomCordinates[positionIndex];
					FighterObject fighterObject = new FighterObject(player.getName(), position.x, position.y, 
							FighterObject.playerIconWidth, FighterObject.playerIconHeight);
					fighterObject.setZhiye(PLAYERICON.BAIJINGJI);
					fighterObject.detaiPositionIndex = 2;
					addFighter(fighterObject);
					playerIndex++;
				}else if(playerIndex == 3){
					position = coordinateManager.rightBottomCordinates[positionIndex];
					FighterObject fighterObject = new FighterObject(player.getName(), position.x, position.y, 
							FighterObject.playerIconWidth, FighterObject.playerIconHeight);
					fighterObject.setZhiye(PLAYERICON.SHENGQIUYUE);
					fighterObject.detaiPositionIndex = 3;
					addFighter(fighterObject);
				}
			}
		}
		
	}
	
	public void onConfirm(Map<Long, Action> inputAction){
		for(Map.Entry<Long, Action> entry:inputAction.entrySet()){
			engine.sw.ActionListRet.add(entry.getValue());
		}
		inputAction.clear();
	}
	
	public void onBtnShowSpeed(){
		if(PlayerAction.getShowSpeed() == 1){
			PlayerAction.setShowSpeed(2);
		}else{
			PlayerAction.setShowSpeed(1);
		}
	}
	
	public void update(float deltaTime){
		
		if(wordState == WordState.waitingNextRound){
			if(canChangeToInputing){
				setWordState(WordState.inputting);
			}
		}else if(wordState == WordState.waitingActions){
			
		}else if(wordState == WordState.showing){
			updateShowing(deltaTime);
			for(FighterObject fighterObject:fighterObjects){
				fighterObject.update(deltaTime);
			}
		}
		
		//fighterObjects.get(0).update(deltaTime);
	}
	
	private void updateShowing(float deltaTime){
		
		if(isAllFighterActionOver()){
			StepActioin stepActioin = stepActioins.poll();
			if(stepActioin == null){
				
				setWordState(WordState.waitingNextRound);
				return;
			}
			
			FighterObject fighterObject = getFighterObject(stepActioin.name);
			
			if(fighterObject == null){
				return;
			}
			Log.d("SuperLineFighter", "name:" + fighterObject.namString);
//			
			if(stepActioin.showAction == SHOWACTION.MOVE){
				fighterObject.move(stepActioin.positon);
			}else if(stepActioin.showAction == SHOWACTION.DAMAGED){
				fighterObject.damageBloodTo(stepActioin.blood);
			}else if(stepActioin.showAction == SHOWACTION.QUAN){
				fighterObject.attack_quan();
			}else if(stepActioin.showAction == SHOWACTION.SHUAI){
				fighterObject.attack_shuai();
			}else if(stepActioin.showAction == SHOWACTION.QIGONG){
				fighterObject.attack_qigong(stepActioin.direction);
			}else if(stepActioin.showAction == SHOWACTION.BISHA){
				fighterObject.attack_bisha(stepActioin.direction);
			}else if(stepActioin.showAction == SHOWACTION.AVOID){
				fighterObject.avoid();
			}
			
			
		}

	}
	
	public void insertStepAction(Queue<StepActioin> stepActioins){
		for(StepActioin stepActioin:stepActioins){
			this.stepActioins.offer(stepActioin);
		}
	}
	
	
	public void setMyName(String myNameString){
		this.myNameString = myNameString;
	}
	public String getMyNameString() {
		return myNameString;
	}
	public List<FighterObject> getFighterObjects() {
		return fighterObjects;
	}

	public void addFighter(FighterObject fighterObject){
		fighterObjects.add(fighterObject);
	}

	public void setWordState(WordState wordState){
		this.wordState = wordState;
	}
	public WordState getWordState(){
		return wordState;
	}
	
	public FighterObject getFighterObject(String name){
		for(FighterObject fighterObject:fighterObjects){
			if(fighterObject.namString.equals(name)){
				return fighterObject;
			}
		}
		return null;
	}
	
//	public void setShowingAction(ShowingAction showingAction) {
//		if(showingAction != null){
//			wordState = WordState.showing;
//			showingRound = 0;
//			this.showingAction = showingAction;
//		}
		
//	}
	
	private boolean isAllFighterActionOver(){
		boolean over = true;
		for(FighterObject fighterObject:fighterObjects){
			if(!fighterObject.isAllActionOver()){
				over = false;
				break;
			}
		}
		return over;
	}

	private void handleShowActions(Queue<StepActioin> stepActioins){
		
		for(StepActioin stepActioin:stepActioins){
			FighterObject fighterObject = getFighterObject(stepActioin.name);
			if(stepActioin.showAction == SHOWACTION.MOVE){
				int iAimX = (int)stepActioin.positon.x;
				int iAimY = (int)stepActioin.positon.y;
				
				Vector2 aimPosition = new Vector2();
				int poisitionIndex = (iAimX - 1)*3 + (iAimY - 1);
				
				if(fighterObject.detaiPositionIndex == 0){
					aimPosition = coordinateManager.centorPositonCordinates[poisitionIndex];
				}else if(fighterObject.detaiPositionIndex == 1){
					aimPosition = coordinateManager.topPositionCordinates[poisitionIndex];
				}else if(fighterObject.detaiPositionIndex == 2){
					aimPosition = coordinateManager.leftBottomCordinates[poisitionIndex];
				}else if(fighterObject.detaiPositionIndex == 3){
					aimPosition = coordinateManager.rightBottomCordinates[poisitionIndex];
				}
				stepActioin.positon = aimPosition;
			}
			
		}
		
		insertStepAction(stepActioins);
		setWordState(WordState.showing);
	}
	
	@Override
	public void handleMessage(Message msg) {
		// TODO Auto-generated method stub
		super.handleMessage(msg);
		
		Bundle bundle = msg.getData();
		
		if(bundle.containsKey("players")){
			List<Player> players = (List<Player>)bundle.getSerializable("players");
			initPlayers(players);
		}else if(bundle.containsKey("input")){
			String strCommand = bundle.getString("input");
			if(strCommand.equals("command")){
				//setWordState(WordState.inputting);
				canChangeToInputing = true;
			}
		}else if(bundle.containsKey("showActions")){
			Queue<StepActioin> stepActioins = (Queue<StepActioin>)bundle.getSerializable("showActions");
			handleShowActions(stepActioins);
		}
		
		
	}
	
}
