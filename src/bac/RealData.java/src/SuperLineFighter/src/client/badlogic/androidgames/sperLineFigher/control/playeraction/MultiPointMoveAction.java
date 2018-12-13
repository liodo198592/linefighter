package client.badlogic.androidgames.sperLineFigher.control.playeraction;

import java.util.LinkedList;
import java.util.List;
import java.util.Queue;

import android.R.integer;

import client.badlogic.androidgames.framework.math.Vector2;

public class MultiPointMoveAction extends PlayerAction {

	private List<Vector2> aimPositions = new LinkedList<Vector2>();
	private Vector2 playerPosition;
	private float moveSpeed = 3;
	
	/**
	 * º”ÀŸ∂»
	 */
	private float acceleSpeed = 0;
	


	//private int showingIndex = 0;
	private Queue<MovePlayerAction> movePlayerActions = new LinkedList<MovePlayerAction>();
	private MovePlayerAction showingAction = null;
	public MultiPointMoveAction(Vector2 playerPosition,float moveSpeed){
		this.playerPosition = playerPosition;
		this.moveSpeed = moveSpeed;
	}
	
	public void addAimPositon(Vector2 aimPosition){
		aimPositions.add(aimPosition);
	}
	
	@Override
	public void start() {
		// TODO Auto-generated method stub
		if(aimPositions.size() == 0){
			return;
		}
		for(Vector2 vector2:aimPositions){
			MovePlayerAction movePlayerAction = new MovePlayerAction(playerPosition, moveSpeed);
			movePlayerAction.setAcceleSpeed(acceleSpeed);
			movePlayerAction.setAimPositon(vector2);
			movePlayerActions.add(movePlayerAction);
		}
		
		isPlaying = true;

		showingAction = movePlayerActions.poll();
		showingAction.start();
		
	}

	@Override
	public void update(double delTime) {
		// TODO Auto-generated method stub

		if(!isPlaying){
			return;
		}
		
		
		if(showingAction.isFinished()){
			showingAction = movePlayerActions.poll();
			if(showingAction == null){
				isPlaying = false;
				return;
			}else {
				showingAction.start();
			}
		}
		
		
		showingAction.update(delTime);
		
	}

	

	public float getAcceleSpeed() {
		return acceleSpeed;
	}


	public void setAcceleSpeed(float acceleSpeed) {
		this.acceleSpeed = acceleSpeed;
	}
}
