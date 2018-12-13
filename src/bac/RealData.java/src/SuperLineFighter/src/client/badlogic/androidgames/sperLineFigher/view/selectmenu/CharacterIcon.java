package client.badlogic.androidgames.sperLineFigher.view.selectmenu;

import client.badlogic.androidgames.sperLineFigher.resource.Assests.PLAYERICON;
import client.badlogic.androidgames.framework.GameObject;

//角色头像，对应角色头像的id
public class CharacterIcon extends GameObject{

	PLAYERICON playerName;
	String nameDesc;
	String ktcSound;
	
	public String getKtcSound() {
		return ktcSound;
	}

	public void setKtcSound(String ktcSound) {
		this.ktcSound = ktcSound;
	}

	public PLAYERICON getPlayerName() {
		return playerName;
	}

	public void setPlayerName(PLAYERICON playerName) {
		this.playerName = playerName;
	}

	static  public int width = 150;
	static  public int height = 150;
	
	public CharacterIcon(PLAYERICON playerName, float x, float y) {
		super(x, y, width, height);
		this.playerName = playerName;
		// TODO Auto-generated constructor stub
		switch(playerName){
//		case SHENGQIUYUE:  nameDesc = "盛秋月";  break;
//		case BAIJINGJI:  nameDesc = "白敬祺";  break;	
//		case LVQINGCHENG:  nameDesc = "吕青橙";  break;	
//		case WENLIANGGONG:  nameDesc = "温良恭";  break;	
//		case LUSANJIN:  nameDesc = "陆三金";  break;	
//		case QIUYINGLUO:  nameDesc = "邱璎珞";  break;	
//		case CAIBADOU:  nameDesc = "蔡八斗";  break;	
//		case HUHU:  nameDesc = "糊糊";  break;	
//		case MAZHA:  nameDesc = "马扎";  break;	
		case SHENGQIUYUE: {
				nameDesc = "SHENGQIUYUE"; 
				ktcSound = "shengqiuyue-ktc.mp3";
				break;
			}
		case BAIJINGJI: {
				nameDesc = "BAIJINGJI";  
				ktcSound = "baijingqi-ktc.mp3";
				break;	
			}
		case LVQINGCHENG:{
				nameDesc = "LVQINGCHENG";  
				ktcSound = "lvqingcheng-ktc.mp3";
				break;	
			}
		case WENLIANGGONG: {
				nameDesc = "WENLIANGGONG";  
				ktcSound = "wenlianggong-ktc.mp3";
				break;	
			}
		case LUSANJIN:  {
				nameDesc = "LUSANJIN";  
				ktcSound = "lusanjin-ktc.mp3";
				break;	
			}
		case QIUYINGLUO:{
				nameDesc = "QIUYINGLUO";  
				ktcSound = "qiuyingluo-ktc.mp3";
				break;	
			}
		case CAIBADOU: {
				nameDesc = "CAIBADOU";  
				ktcSound = "caibadou-ktc.mp3";
				break;	
			}
		case HUHU:  {
				nameDesc = "HUHU";  
				ktcSound = "shengqiuyue-ktc.mp3";
				break;	
			}
		case MAZHA:  {
				nameDesc = "MAZHA";  
				ktcSound = "shengqiuyue-ktc.mp3";
				break;	
			}
		default: break;
		}
	}
	
	public String getNameDesc() {
		return nameDesc;
	}

	public void setNameDesc(String nameDesc) {
		this.nameDesc = nameDesc;
	}

	public boolean checkTouched(float x, float y){
		//触摸点击到的像素最上面是0，最下面是900，与绘图像素是反的
		if( x > position.x - width/2  && x < position.x + width/2 &&
				y < position.y + height/2 &&   y > position.y - height/2){
			return true;
		}
		return false;
	}
	
}
