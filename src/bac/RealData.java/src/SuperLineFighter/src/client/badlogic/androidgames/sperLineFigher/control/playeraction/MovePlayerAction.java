package client.badlogic.androidgames.sperLineFigher.control.playeraction;

import android.util.Log;

import client.badlogic.androidgames.framework.math.Vector2;

public class MovePlayerAction extends PlayerAction {

	Vector2 aimPosition;
	Vector2 playerPosition;
	private float moveSpeed = 3;
	
	/**
	 * ¼ÓËÙ¶È
	 */
	private float acceleSpeed = 0;
	
	public MovePlayerAction(Vector2 playerPosition,float moveSpeed){
		aimPosition = new Vector2();
		this.playerPosition = playerPosition;
		this.moveSpeed = moveSpeed;
	}
	
	public void setAimPositon(float x,float y){
		aimPosition.set(x, y);
	}
	
	public void setAimPositon(Vector2 aimPositon){
		this.aimPosition.set(aimPositon);
	}
	
	@Override
	public void start() {
		// TODO Auto-generated method stub
		isPlaying = true;

	}

	@Override
	public void update(double delTime) {
		// TODO Auto-generated method stub

		if(!isPlaying){
			return;
		}
		
		if(this.playerPosition.x != aimPosition.x){
			float moveLength = aimPosition.x - this.playerPosition.x;
			if(Math.abs(moveLength) > moveSpeed*showSpeed){
				if(moveLength >0){
					moveLength = moveSpeed*showSpeed;
				}else {
					moveLength = -moveSpeed*showSpeed;
				}
			}
			
			this.playerPosition.x += moveLength;
		}
		
		if(this.playerPosition.y != aimPosition.y){
			float moveLength = aimPosition.y - this.playerPosition.y;
			if(Math.abs(moveLength) > moveSpeed*showSpeed){
				if(moveLength >0){
					moveLength = moveSpeed*showSpeed;
				}else {
					moveLength = -moveSpeed*showSpeed;
				}
			}
			this.playerPosition.y += moveLength;
		}
		
		Log.d("SuperLineFighter", "thisPosition:x:" + this.playerPosition.x + " y:" + this.playerPosition.y
				+ "  aimPosition:x:" + aimPosition.x + " y:" + aimPosition.y);
		//if(this.playerPosition.equals(aimPosition)){
		if(this.playerPosition.x == aimPosition.x && this.playerPosition.y == aimPosition.y){
			isPlaying = false;
		}
		
		moveSpeed += acceleSpeed;
	}

	public float getAcceleSpeed() {
		return acceleSpeed;
	}


	public void setAcceleSpeed(float acceleSpeed) {
		this.acceleSpeed = acceleSpeed;
	}
	
}
