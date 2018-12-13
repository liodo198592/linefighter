package client.badlogic.androidgames.sperLineFigher.control.playeraction;

import android.R.integer;
import android.util.Log;

import client.badlogic.androidgames.sperLineFigher.Const;
import client.badlogic.androidgames.sperLineFigher.Const.GameElement;
import client.badlogic.androidgames.sperLineFigher.model.FighterObject;
import client.badlogic.androidgames.sperLineFigher.resource.Assests;
import client.badlogic.androidgames.sperLineFigher.view.WorderRenderer;
import client.badlogic.androidgames.framework.math.Vector2;

public class QiGongPlayerAction extends PlayerAction {

	private FighterObject fighterObject;
	
	Vector2 oriPosition = new Vector2();
	
	Const.GameElement gameElementWorld;
	Const.GameElement gameElementQiGong;
	double durTime = 0;
	int index = 0;
	double wordTime = 0;
	int direction = 0;//1Ïò×ó 2ÏòÓÒ
	


	private final int qigongWidth = 190;
	private final int qigongHeight = 120;
	
	public QiGongPlayerAction(FighterObject fighterObject){
		this.fighterObject = fighterObject;
	}
	

	public void setDirection(int direction) {
		this.direction = direction;
	}
	
	@Override
	public void start() {
		// TODO Auto-generated method stub

		isPlaying = true;
		oriPosition.set(fighterObject.position);
		index = 0;
		durTime = 0;
		
	}

	@Override
	public void update(double delTime) {
		// TODO Auto-generated method stub


		if(!isPlaying){
			return;
		}
		
		if(index < 5 && durTime < 0.1/showSpeed){
			durTime += delTime;
			return;
		}
		durTime = 0;
		switch(index){
		case 0:{
			fighterObject.position.add(10, 10);
			index = 1;
			break;
		}
		case 1:{
			fighterObject.position.set(oriPosition);
			index = 2;
			break;
		}
		case 2:{
			fighterObject.position.add(-10, -10);
			index = 3;
			break;
		}
		case 3:{
			fighterObject.position.set(oriPosition);
			index = 4;
			break;
		}
		case 4:{
			gameElementWorld = new GameElement(fighterObject.position, 400, 100, 
					Assests.skillNameTextureRegionHashMap.get(Assests.SKILL.QIGONG), 1);
			fighterObject.elementList.add(gameElementWorld);
			index = 5;
			wordTime = 0;
			break;
		}
		case 5:{
			//Log.d("quan", "" + wordTime);
			if(wordTime > ((double)1/showSpeed)){
				fighterObject.elementList.remove(gameElementWorld);
				
				index = 6;
			}
			wordTime += delTime;
			break;
		}
		case 6:{
			if(direction == 1){
				gameElementQiGong = new GameElement(fighterObject.position, qigongWidth, qigongHeight, 
						Assests.leftQigongRegion, 1);
			}else {
				gameElementQiGong = new GameElement(fighterObject.position, qigongWidth, qigongHeight, 
						Assests.rightQigongRegion, 1);
			}
			
			fighterObject.elementList.add(gameElementQiGong);
			index = 7;
			break;
		}case 7:{
			
			if(direction == 1){
				
				if((gameElementQiGong.position.x - qigongWidth/2) <= WorderRenderer.showAreaLeft){
					fighterObject.elementList.remove(gameElementQiGong);
					index = 8;
				}else {
					gameElementQiGong.position.add(-6*showSpeed,0);
				}
				
			}else {
				if((gameElementQiGong.position.x + qigongWidth/2) >= WorderRenderer.showArearight){
					fighterObject.elementList.remove(gameElementQiGong);
					index = 8;
				}else {
					gameElementQiGong.position.add(6*showSpeed,0);
				}
			}
			
			break;
		}case 8:{
			isPlaying = false;
			break;
		}
		}
		
		
	}

}
