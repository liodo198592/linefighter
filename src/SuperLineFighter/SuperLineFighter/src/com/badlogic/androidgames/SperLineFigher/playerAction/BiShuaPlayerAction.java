package com.badlogic.androidgames.SperLineFigher.playerAction;

import android.util.Log;

import com.badlogic.androidgames.SperLineFigher.Assests;
import com.badlogic.androidgames.SperLineFigher.Const;
import com.badlogic.androidgames.SperLineFigher.FighterObject;
import com.badlogic.androidgames.SperLineFigher.WorderRenderer;
import com.badlogic.androidgames.SperLineFigher.Const.GameElement;
import com.badlogic.androidgames.framework.math.Vector2;

public class BiShuaPlayerAction extends PlayerAction {
	
	private FighterObject fighterObject;
	
	Vector2 oriPosition = new Vector2();
	
	Const.GameElement gameElementWorld;
	Const.GameElement gameElementBiSha;
	double durTime = 0;
	int index = 0;
	double wordTime = 0;
	int direction = 0;//1Ïò×ó 2ÏòÓÒ
	
	private final int bishaWidth = 210;
	private final int bishaHeight = 150;
	
	public BiShuaPlayerAction(FighterObject fighterObject){
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
					Assests.skillNameTextureRegionHashMap.get(Assests.SKILL.BISHA), 1);
			fighterObject.elementList.add(gameElementWorld);
			index = 5;
			wordTime = 0;
			break;
		}
		case 5:{
			Log.d("quan", "" + wordTime);
			if(wordTime > ((double)1/showSpeed)){
				fighterObject.elementList.remove(gameElementWorld);
				//fighterObject.elementList.clear();
				index = 6;
			}
			wordTime += delTime;
			break;
		}
		case 6:{
			if(direction == 1){
				gameElementBiSha = new GameElement(fighterObject.position, bishaWidth, bishaHeight, 
						Assests.leftBishaRegion, 1);
			}else {
				gameElementBiSha = new GameElement(fighterObject.position, bishaWidth, bishaHeight, 
						Assests.rightBishagRegion, 1);
			}
			
			fighterObject.elementList.add(gameElementBiSha);
			index = 7;
			break;
		}case 7:{
			
			if(direction == 1){
				
				if((gameElementBiSha.position.x - bishaWidth/2) <= WorderRenderer.showAreaLeft){
					fighterObject.elementList.remove(gameElementBiSha);
					index = 8;
				}else {
					gameElementBiSha.position.add(-6*showSpeed,0);
				}
				
			}else {
				if((gameElementBiSha.position.x + bishaWidth/2) >= WorderRenderer.showArearight){
					fighterObject.elementList.remove(gameElementBiSha);
					index = 8;
				}else {
					gameElementBiSha.position.add(6*showSpeed,0);
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
