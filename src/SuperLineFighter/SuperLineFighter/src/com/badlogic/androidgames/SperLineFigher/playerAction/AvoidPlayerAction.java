package com.badlogic.androidgames.SperLineFigher.playerAction;

import android.util.Log;

import com.badlogic.androidgames.SperLineFigher.Assests;
import com.badlogic.androidgames.SperLineFigher.Const;
import com.badlogic.androidgames.SperLineFigher.FighterObject;
import com.badlogic.androidgames.SperLineFigher.Const.GameElement;
import com.badlogic.androidgames.framework.math.Vector2;

public class AvoidPlayerAction extends PlayerAction {

	private FighterObject fighterObject;
	
	Vector2 oriPosition = new Vector2();
	
	Const.GameElement gameElementAvoid;
	double durTime = 0;
	int index = 0;
	double wordTime = 0;
	
	public AvoidPlayerAction(FighterObject fighterObject){
		this.fighterObject = fighterObject;
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
			gameElementAvoid = new GameElement(fighterObject.position, 400, 100, 
					Assests.avoidWordRegion, 1);
			fighterObject.elementList.add(gameElementAvoid);
			index = 5;
			wordTime = 0;
			break;
		}
		case 5:{
			//Log.d("quan", "" + wordTime);
			if(wordTime > ((double)1/showSpeed)){
				fighterObject.elementList.remove(gameElementAvoid);
				index = 6;
			}
			wordTime += delTime;
			break;
		}
		case 6:{
			isPlaying = false;
			break;
		}
		}
	}

}
