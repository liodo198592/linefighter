package client.badlogic.androidgames.sperLineFigher.view.selectmenu;

import client.badlogic.androidgames.framework.gl.TextureRegion;
import client.badlogic.androidgames.framework.math.Vector2;

public class MoveProperty {

	/**
	 * 当前的位置
	 */
	private Vector2 nowPosition;
	


	/**
	 * 移动的起始坐标
	 */
	private final Vector2 sourcePosition;
	


	/**
	 * 移动的目的坐标
	 */
	private final Vector2 aimPosition;
	

	private Vector2 size;

	private TextureRegion nowTextureRegion;

	

	public MoveProperty(Vector2 sourcePosition,Vector2 aimPosition,Vector2 size){
		this.sourcePosition = sourcePosition;
		this.aimPosition = aimPosition;
		this.size = size;
		this.resetNowPosition();
	}
	
	public void resetNowPosition(){
		this.nowPosition = new Vector2(this.sourcePosition);
	}
	
	public Vector2 getNowPosition() {
		return nowPosition;
	}
	
	public Vector2 getSourcePosition() {
		return sourcePosition;
	}
	
	
	public Vector2 getAimPosition() {
		return aimPosition;
	}
	
	
	public Vector2 getSize() {
		return size;
	}
	

	public TextureRegion getNowTextureRegion() {
		return nowTextureRegion;
	}


	public void setNowTextureRegion(TextureRegion nowTextureRegion) {
		this.nowTextureRegion = nowTextureRegion;
	}
}
