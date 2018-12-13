package client.badlogic.androidgames.sperLineFigher.view.selectmenu;

import client.badlogic.androidgames.framework.GameObject;
import client.badlogic.androidgames.framework.gl.TextureRegion;

//角色头像，对应角色头像的id
public class SelectMenuBtnIcon extends GameObject{

	TextureRegion thisTR;
	
	static  public int width = 192;
	static  public int height = 32;
	
	public SelectMenuBtnIcon(TextureRegion tr , float x, float y) {
		super(x, y, width, height);
		this.thisTR = tr;
	}	
	
	public boolean checkTouched(int x, int y){
		//触摸点击到的像素最上面是0，最下面是900，与绘图像素是反的
		//修正X,Y
		float tempy = y / (float)720 * (float)900;
		float tempx = x / (float)1258 * (float)1440;
		if( tempx > position.x - width/2  && tempx < position.x + width &&
				(900 - tempy) < position.y + height/2 &&  (900- tempy) > position.y - height){
			return true;
		}
		return false;
	}
}
