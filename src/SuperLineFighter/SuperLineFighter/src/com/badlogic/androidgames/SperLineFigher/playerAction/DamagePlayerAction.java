package com.badlogic.androidgames.SperLineFigher.playerAction;

import com.badlogic.androidgames.SperLineFigher.Assests;
import com.badlogic.androidgames.SperLineFigher.Const;
import com.badlogic.androidgames.SperLineFigher.FighterObject;
import com.badlogic.androidgames.SperLineFigher.Const.GameElement;
import com.badlogic.androidgames.framework.gl.TextureRegion;
import com.badlogic.androidgames.framework.math.Vector2;

public class DamagePlayerAction extends PlayerAction {

	FighterObject fighterObject;
	int aimBlood;
	double durTime = 0;
	Vector2 oriPosition = new Vector2();
	int index = 0;
	Const.GameElement gameElementDamaged;
	public DamagePlayerAction(FighterObject fighterObject){
		this.fighterObject = fighterObject;
	}
	
	public void setAimBlood(int aimBlood){
		this.aimBlood = aimBlood;
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
			gameElementDamaged = new GameElement(fighterObject.position, fighterObject.bounds.width, fighterObject.bounds.height, 
					Assests.redCircleRegion, 1);
			fighterObject.elementList.add(gameElementDamaged);
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
			fighterObject.elementList.remove(gameElementDamaged);
			index = 5;
			break;
		}
		case 5:{
			updateBlood();
			break;
		}
		}
		
		
	}

	private void updateBlood(){
		
		float playerBlood = fighterObject.getBlood();
		if( playerBlood > aimBlood){
			fighterObject.setBlood((float)(playerBlood - 0.2*showSpeed));
		}else {
			fighterObject.setBlood(aimBlood);
			isPlaying = false;
		}
	}
}
