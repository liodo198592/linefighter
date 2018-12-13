package com.badlogic.androidgames.SperLineFigher;

import java.util.HashMap;
import java.util.LinkedHashMap;

import android.R.integer;
import android.content.res.AssetFileDescriptor;

import com.badlogic.androidgames.framework.Music;
import com.badlogic.androidgames.framework.gl.Font;
import com.badlogic.androidgames.framework.gl.SpriteBatcher;
import com.badlogic.androidgames.framework.gl.Texture;
import com.badlogic.androidgames.framework.gl.TextureRegion;
import com.badlogic.androidgames.framework.impl.AndroidAudio;
import com.badlogic.androidgames.framework.impl.AndroidMusic;
import com.badlogic.androidgames.framework.impl.GLGame;

public class Assests {

	public static enum PLAYERICON{
		SHENGQIUYUE,
		BAIJINGJI,
		LVQINGCHENG,
		WENLIANGGONG,
		LUSANJIN,
		QIUYINGLUO,
		CAIBADOU,
		HUHU,
		MAZHA
	}
	
	public static enum SKILL{
		QUAN,
		SHUAI,
		QIGONG,
		BISHA,
		SHANBI,
		YIDONG,
		HUANXIAN
	}
	
	public static Texture loadingTexture;
	public static TextureRegion loadingTextureRegion;
	
	public static Texture background;
	public static TextureRegion backgroundRegion;
	
	public static Texture textItem;
	
	public static Texture item;
	public static HashMap<PLAYERICON, TextureRegion> playerIconHashMap;
	public static HashMap<SKILL, TextureRegion> skillIconTextureRegionHashMap;
	public static HashMap<SKILL, TextureRegion> skillNameTextureRegionHashMap;
	
	public static TextureRegion redCircleRegion;
	
	public static TextureRegion positionWord1Region;
	public static TextureRegion positionWord2Region;
	public static TextureRegion positionWord3Region;
	public static TextureRegion leftWordRegion;
	public static TextureRegion rightWordRegion;
	public static TextureRegion avoidWordRegion;
	
	public static TextureRegion xueCaoRegion;
	public static TextureRegion xueLiangRegion;
	public static TextureRegion[] numbeRegions;
	public static TextureRegion execuTextureRegion;
	public static TextureRegion executeDownTextureRegion;
	public static TextureRegion cancelTextureRegion;
	public static TextureRegion cancelDownTextureRegion;
	
	public static TextureRegion blackBoardTextureRegion;
	
	public static TextureRegion[] positionBtnTextureRegions;
	public static TextureRegion leftBtnTextureRegion;
	public static TextureRegion rightBtnTextureRegion;
	
	public static TextureRegion leftQigongRegion;
	public static TextureRegion rightQigongRegion;
	
	public static TextureRegion leftBishaRegion;
	public static TextureRegion rightBishagRegion;
	
	public static TextureRegion showSpeed1;
	public static TextureRegion showSpeed2;
	
	public static final int cellLength = 59;
	
	public static AndroidAudio andriAudio;
	public static Music music;
	
	
	public static Font font;
	public static TextureRegion ready;
	
	public static void load(GLGame game) {
		
		
		textItem = new Texture(game, "textitems.png");
		font = new Font(textItem, 224, 0, 16, 16, 20);
		ready = new TextureRegion(textItem, 320, 224, 192, 32);
		loadingTexture = new Texture(game, "family.jpg");
		
		loadingTextureRegion = new TextureRegion(loadingTexture, 0, 0, 431, 600);
		
		//播放开始音乐
		andriAudio = new AndroidAudio(game);
		music = andriAudio.newMusic("openning.mp3");
		music.setLooping(true);
		music.play();
		
		background = new Texture(game, "select.jpg");
		backgroundRegion = new TextureRegion(background, 0, 0, 450, 253);
		
		item = new Texture(game, "item-longmen.png");
		PLAYERICON[] playerICon = new PLAYERICON[]{PLAYERICON.SHENGQIUYUE,PLAYERICON.BAIJINGJI,
				PLAYERICON.LVQINGCHENG,PLAYERICON.WENLIANGGONG,PLAYERICON.LUSANJIN,PLAYERICON.QIUYINGLUO,
				PLAYERICON.CAIBADOU,PLAYERICON.HUHU,PLAYERICON.MAZHA};
		
		playerIconHashMap = new LinkedHashMap<Assests.PLAYERICON, TextureRegion>();
		for(int i=0;i<playerICon.length - 1;i++){
			playerIconHashMap.put(playerICon[i], new TextureRegion(item,0,i*cellLength*2,cellLength*2,cellLength*2));
		}
		
		playerIconHashMap.put(PLAYERICON.MAZHA, new TextureRegion(item,cellLength*2,0,cellLength*2,cellLength*2));
		
		redCircleRegion = new TextureRegion(item, 4*cellLength, 15*cellLength, cellLength, cellLength);
		
		SKILL[] skills = new SKILL[]{SKILL.QUAN,SKILL.SHUAI,SKILL.QIGONG,SKILL.BISHA,
				SKILL.SHANBI,SKILL.YIDONG,SKILL.HUANXIAN};
		
		skillIconTextureRegionHashMap = new HashMap<Assests.SKILL, TextureRegion>();
		for(int i=0;i<skills.length;i++){
			skillIconTextureRegionHashMap.put(skills[i],new TextureRegion(item,cellLength*2,(i + 1)*cellLength*2,cellLength*2,cellLength*2));
		}
		
		skillNameTextureRegionHashMap = new HashMap<Assests.SKILL, TextureRegion>();
		for(int i=0;i<skills.length;i++){
			skillNameTextureRegionHashMap.put(skills[i],new TextureRegion(item,cellLength*4,i*cellLength/2,cellLength*2,cellLength/2));
		}
		
		positionWord1Region = new TextureRegion(item, cellLength*6, 0, cellLength*2, cellLength/2);
		positionWord2Region = new TextureRegion(item, cellLength*6, cellLength/2, cellLength*2, cellLength/2);
		positionWord3Region = new TextureRegion(item, cellLength*6, cellLength, cellLength*2, cellLength/2);
		leftWordRegion = new TextureRegion(item, cellLength*6, cellLength/2*3, cellLength*2, cellLength/2);
		rightWordRegion = new TextureRegion(item, cellLength*6, cellLength*2, cellLength*2, cellLength/2);
		avoidWordRegion = new TextureRegion(item, cellLength*6, cellLength*5/2, cellLength*2, cellLength);
	
		//获取血槽及血量
		xueCaoRegion = new TextureRegion(item, cellLength*4, cellLength*4, cellLength*4, cellLength);
		
		xueLiangRegion = new TextureRegion(item, cellLength*4, cellLength*6, cellLength*4, cellLength);
		
		
		numbeRegions = new TextureRegion[10];
		//获取数字
		for(int i=0;i<10;i++){
			if(i>=0 && i<=3){
				numbeRegions[i] = new TextureRegion(item, cellLength*(4 + i), cellLength*8, cellLength, cellLength);
			}else if(i>=4 && i<= 7){
				numbeRegions[i] = new TextureRegion(item, cellLength*i, cellLength*9, cellLength, cellLength);
			}else if(i>=8 && i<=9){
				numbeRegions[i] = new TextureRegion(item, cellLength*(i - 4), cellLength*10, cellLength, cellLength);
			}
		}
		
		execuTextureRegion = new TextureRegion(item, cellLength*4, cellLength*12, cellLength*2, cellLength);
		executeDownTextureRegion = new TextureRegion(item, cellLength*6, cellLength*2, cellLength*2, cellLength);
		cancelTextureRegion = new TextureRegion(item, cellLength*4, cellLength*13, cellLength*2, cellLength);
		cancelDownTextureRegion = new TextureRegion(item, cellLength*6, cellLength*13, cellLength*2, cellLength);
		
		blackBoardTextureRegion = new TextureRegion(item, cellLength*8, 0, cellLength*6, cellLength*2);
		
		positionBtnTextureRegions = new TextureRegion[3];
		for(int i =0;i<3;i++){
			positionBtnTextureRegions[i] = new TextureRegion(item, cellLength*(8 + i*2), cellLength*2, cellLength*2, cellLength);
		}
		
		leftBtnTextureRegion = new TextureRegion(item, cellLength*8, cellLength*3, cellLength*2, cellLength);
		rightBtnTextureRegion = new TextureRegion(item, cellLength*10, cellLength*3, cellLength*2, cellLength);
		
		leftQigongRegion = new TextureRegion(item, cellLength*8, cellLength*5, cellLength*2, cellLength);
		rightQigongRegion = new TextureRegion(item, cellLength*10, cellLength*5, cellLength*2, cellLength);
		
		leftBishaRegion = new TextureRegion(item, cellLength*8, cellLength*6, cellLength*2, cellLength);
		rightBishagRegion = new TextureRegion(item, cellLength*10, cellLength*6, cellLength*2, cellLength);
		
		showSpeed1 = new TextureRegion(item, cellLength*8, cellLength*8, cellLength*2, cellLength);
		showSpeed2 = new TextureRegion(item, cellLength*10, cellLength*8, cellLength*2, cellLength);
	}
	
	public static void reload() {
		background.reload();
		item.reload();
	}
}
