package com.badlogic.androidgames.SperLineFigher.SelectMenu;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map.Entry;

import javax.microedition.khronos.opengles.GL10;


import android.app.Activity;
import android.util.Log;

import com.badlogic.androidgames.SperLineFigher.Assests;
import com.badlogic.androidgames.SperLineFigher.BtnHandler;
import com.badlogic.androidgames.SperLineFigher.GameScreen;
import com.badlogic.androidgames.SperLineFigher.WorderRenderer;
import com.badlogic.androidgames.SperLineFigher.World;
import com.badlogic.androidgames.SperLineFigher.Assests.PLAYERICON;
import com.badlogic.androidgames.framework.Game;
import com.badlogic.androidgames.framework.Input.TouchEvent;
import com.badlogic.androidgames.framework.Sound;
import com.badlogic.androidgames.framework.gl.Camera2D;
import com.badlogic.androidgames.framework.gl.Font;
import com.badlogic.androidgames.framework.gl.SpriteBatcher;
import com.badlogic.androidgames.framework.gl.TextureRegion;
import com.badlogic.androidgames.framework.gl.TextureText;
import com.badlogic.androidgames.framework.impl.GLScreen;
import com.badlogic.androidgames.framework.math.Vector2;

//负责选人界面
public class SelectScreen extends GLScreen {
	
    private static final int camWidth = 1440;
    private static final int camHeidth = 960;

	private float LastTime = 0;
	Game game;
	
	Camera2D guiCam;
    SpriteBatcher batcher;
    World world;
    WorderRenderer worderRenderer;
    BtnHandler btnHandler;
    Vector2 touchPoint;
    List<CharacterIcon> listIcon;
    String selectedNameStr;
    CharacterIcon selectedCi;
    SelectMenuBtnIcon smb;
    boolean needConfirm;
    boolean goNext;
    Sound snd;
    
    
	private void init(){
		guiCam = new Camera2D(glGraphics, camWidth, camHeidth);
		batcher = new SpriteBatcher(glGraphics, 1000);
		touchPoint = new Vector2();   
		
		world = new World(((Activity)game).getMainLooper());
		worderRenderer = new WorderRenderer(world);
		btnHandler = new BtnHandler(batcher,guiCam);
		listIcon = generateCharacterIcon(Assests.playerIconHashMap);
		smb = new SelectMenuBtnIcon(Assests.ready, 1000,50);
		selectedNameStr = null;
		selectedCi = null;
		needConfirm = false;
		goNext = false;
		LastTime = 0;
		 //generatePlayerTest();
		
	}
	
	/**
	 * 根据characters中记录的角色数在选人界面来创建对应的CharacterIcon，通过计算分布各角色，左边排四个，右边排四个
	 * @param characters : 从资源中截取出来的人物
	 * @return   List<CharacterIcon> : 生成的任务ICON类， 不包含图像
	 */
	private List<CharacterIcon> generateCharacterIcon(HashMap<PLAYERICON, TextureRegion> characters){
		
		List<CharacterIcon> listRet = new ArrayList<CharacterIcon>();
		if(characters.isEmpty()){
			Log.d("SelectScreen", "EgenerateCharacterIcon characters is Empty!");
			return null;
		}
		int tempx = 100;
		int tempY = camHeidth ;
		int count = 0;
		int totalCount = 0;
		for(Entry<PLAYERICON, TextureRegion> et : characters.entrySet()){
			tempY -= CharacterIcon.height + 50;
			count++;
			totalCount++;
			if(count > characters.size() / 2){
				count = 1;
				tempx = camWidth - CharacterIcon.width/2 - 20; 
				tempY =  camHeidth - CharacterIcon.height - 50;
			}
			CharacterIcon ci = new CharacterIcon(et.getKey(),tempx, tempY);
			listRet.add(ci);
			if(totalCount > 7){
				break;
			}
		}
		return listRet;
	}
	
	
    
	public SelectScreen(Game game){
		super(game);
		this.game = game;
		guiCam = new Camera2D(glGraphics, 480, 320);
		batcher = new SpriteBatcher(glGraphics, 100);
		init();
	}
	
	private void drawBackGround(){
        batcher.beginBatch(Assests.background);
        batcher.drawSprite(camWidth/2, camHeidth/2, camWidth-400, camHeidth-200, Assests.backgroundRegion);
        batcher.endBatch();
        
        //选择文字
        if(selectedNameStr == null){
        	batcher.beginBatch(Assests.textItem);
            Assests.font.drawText(batcher, "please selectting......", 300, 50);
            batcher.endBatch();
        }
        else{
        	 batcher.beginBatch(Assests.textItem);
        	 Assests.font.drawText(batcher, "                                      ", 300, 50);
             Assests.font.drawText(batcher, "you have select " + selectedNameStr , 300, 50);
             batcher.endBatch();
             
             batcher.beginBatch(Assests.textItem);
             batcher.drawSprite(1200, 50, 192, 50, Assests.ready);
             batcher.endBatch();
             needConfirm = true;
        }
        
       
        
        
        batcher.beginBatch(Assests.item);
		for(CharacterIcon ci:listIcon){
		
			batcher.drawSprite(ci.position.x, ci.position.y,
			ci.bounds.width, ci.bounds.height, 
			Assests.playerIconHashMap.get(ci.getPlayerName()));
			//渲染附属元素
		}
		batcher.endBatch();
		
		 //选择
        if(selectedCi != null){
        	batcher.beginBatch(Assests.item);
    		batcher.drawSprite(selectedCi.position.x, selectedCi.position.y,
    				selectedCi.bounds.width*2, selectedCi.bounds.height*2, 
    				Assests.playerIconHashMap.get(selectedCi.getPlayerName()));
    		batcher.endBatch();
        }
	}
	
	@Override
	public void update(float deltaTime) {
		// TODO Auto-generated method stub
		
		if(deltaTime > 0.1f)
	        deltaTime = 0.1f;
		
		List<TouchEvent> touchEvents = game.getInput().getTouchEvents();
		if(touchEvents.size() > 0){
			for(TouchEvent te : touchEvents){
				for(CharacterIcon ci:listIcon){
					if(ci.checkTouched(te.x, te.y)){
						selectedNameStr = ci.getNameDesc();
						selectedCi = ci;
						break;
					}
				}
				
				if(needConfirm && smb.checkTouched(te.x, te.y) && goNext == false){
					snd = Assests.andriAudio.newSound(selectedCi.getKtcSound());
					Assests.music.pause();
					snd.play(1);
					//Assests.music.play();
					goNext = true;
				}
			}
		}

	}

	@Override
	public void present(float deltaTime) {
		// TODO Auto-generated method stub
		if(deltaTime > 0.1f)
	        deltaTime = 0.1f;
		
		if(goNext){
			LastTime += deltaTime;
		}
		if(LastTime > 3){
			game.setScreen(new GameScreen(game));
			return;
		}
		
		GL10 gl = glGraphics.getGL();        
        gl.glClear(GL10.GL_COLOR_BUFFER_BIT);
        gl.glEnable(GL10.GL_TEXTURE_2D);
        
        guiCam.setViewportAndMatrices();
        
        gl.glEnable(GL10.GL_BLEND);
        gl.glBlendFunc(GL10.GL_SRC_ALPHA, GL10.GL_ONE_MINUS_SRC_ALPHA);  
        
        drawBackGround();
	}

	@Override
	public void pause() {
		// TODO Auto-generated method stub

	}

	@Override
	public void resume() {
		// TODO Auto-generated method stub

	}

	@Override
	public void dispose() {
		// TODO Auto-generated method stub

	}

}
