package client.badlogic.androidgames.sperLineFigher.model;

public class ActionClock {

	/**
	 * ��ǰ����������
	 */
	private String currentAnimationDesc;
	

	/**
	 * ��ʱ��
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
