package client.badlogic.androidgames.sperLineFigher.model;

public class ActionClock {

	/**
	 * 当前动作的描述
	 */
	private String currentAnimationDesc;
	

	/**
	 * 计时器
	 */
	private float clockMilliSeconds;
	
	
	public ActionClock(String currentAnimationDesc,float clockMilliSeconds){
		this.currentAnimationDesc = currentAnimationDesc;
		this.clockMilliSeconds = clockMilliSeconds;
	}
	
	public void update(float delTime){
		this.clockMilliSeconds += delTime;
	}
	
	public void reset(){
		clockMilliSeconds = 0;
	}
	
	public void reset(String currentAnimationDesc){
		this.currentAnimationDesc = currentAnimationDesc;
		clockMilliSeconds = 0;
	}
	
	public String getCurrentAnimationDesc() {
		return currentAnimationDesc;
	}

	
	public float getClockMilliSeconds() {
		return clockMilliSeconds;
	}
}
