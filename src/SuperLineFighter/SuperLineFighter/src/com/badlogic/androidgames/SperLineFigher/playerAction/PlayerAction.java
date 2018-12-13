package com.badlogic.androidgames.SperLineFigher.playerAction;

public abstract class PlayerAction {

	protected boolean isPlaying = false;
	
	protected double delTime = 0;//动作持续时间：秒
	
	protected static int showSpeed = 1;//播放倍率



	public abstract void start();
	
	public abstract void update(double delTime);
	
	public boolean isFinished(){
		return !isPlaying;
	}
	
	public static int getShowSpeed() {
		return showSpeed;
	}


	public static void setShowSpeed(int showSpeed) {
		PlayerAction.showSpeed = showSpeed;
	}
	
}
