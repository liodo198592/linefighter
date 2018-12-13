package client.badlogic.androidgames.sperLineFigher.view.selectmenu;

import client.badlogic.androidgames.framework.GameObject;
import client.badlogic.androidgames.framework.gl.TextureRegion;

//��ɫͷ�񣬶�Ӧ��ɫͷ���id
public class SelectMenuBtnIcon extends GameObject{

	TextureRegion thisTR;
	
	static  public int width = 192;
	static  public int height = 32;
	
	public SelectMenuBtnIcon(TextureRegion tr , float x, float y) {
		super(x, y, width, height);
		this.thisTR = tr;
	}	
	
	public boolean checkTouched(int x, int y){
		//�����������������������0����������900�����ͼ�����Ƿ���
		//����X,Y
		float tempy = y / (float)720 * (float)900;
		float tempx = x / (float)1258 * (float)1440;
		if( tempx > position.x - width/2  && tempx < position.x + width &&
				(900 - tempy) < position.y + height/2 &&  (900- tempy) > position.y - height){
			return true;
		}
		return false;
	}
}
