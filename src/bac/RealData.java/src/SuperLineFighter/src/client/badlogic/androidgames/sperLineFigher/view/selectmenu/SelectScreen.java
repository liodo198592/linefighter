package client.badlogic.androidgames.sperLineFigher.view.selectmenu;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map.Entry;

import javax.microedition.khronos.opengles.GL10;


import android.R.integer;
import android.app.Activity;
import android.util.Log;

import client.badlogic.androidgames.sperLineFigher.control.BtnHandler;
import client.badlogic.androidgames.sperLineFigher.control.World;
import client.badlogic.androidgames.sperLineFigher.control.playeraction.MovePlayerAction;
import client.badlogic.androidgames.sperLineFigher.control.playeraction.MultiPointMoveAction;
import client.badlogic.androidgames.sperLineFigher.resource.AnimationManager;
import client.badlogic.androidgames.sperLineFigher.resource.Assests;
import client.badlogic.androidgames.sperLineFigher.resource.Assests.PLAYERICON;
import client.badlogic.androidgames.sperLineFigher.view.GameScreen;
import client.badlogic.androidgames.sperLineFigher.view.WorderRenderer;
import client.badlogic.androidgames.framework.Game;
import client.badlogic.androidgames.framework.Input.TouchEvent;
import client.badlogic.androidgames.framework.Sound;
import client.badlogic.androidgames.framework.gl.Animation;
import client.badlogic.androidgames.framework.gl.Camera2D;
import client.badlogic.androidgames.framework.gl.Font;
import client.badlogic.androidgames.framework.gl.SpriteBatcher;
import client.badlogic.androidgames.framework.gl.TextureRegion;
import client.badlogic.androidgames.framework.gl.TextureText;
import client.badlogic.androidgames.framework.impl.GLScreen;
import client.badlogic.androidgames.framework.math.Vector2;

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
    
    /**
     * 选择画面背景的四块的当前坐标
     */
    private Vector2[] selectBackVector2s = null;
    
    /**
     * 选择画面背景的四块的原始坐标
     */
    private Vector2[] selectBackSourceVector2s = null;
    
    /**
     * 选择画面四块坐标隐藏时的坐标
     */
    private Vector2[] selectBackHideVector2s = null;
    
    /**
     * 选择画面四块坐标每块的大小
     */
    private Vector2 selsectBckSize = new Vector2(500,380);
    
    private MoveProperty cardMoveProperty;
    
    //private boolean selectBackInCentor = true;

    private Vector2 runningPlayerAnimationPosition = new Vector2(470,670);
    private Vector2 runningPlayerAnimationSize = new Vector2(375,285);
    private float runningPlayerTimer = 0;
    
    
    private List<MultiPointMoveAction> movePlayerActions = new LinkedList<MultiPointMoveAction>();
    
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
		
		selectBackVector2s = new Vector2[4];
		selectBackVector2s[0] = new Vector2(470,670);
		selectBackVector2s[1] = new Vector2(970,670);
		selectBackVector2s[2] = new Vector2(470,290);
		selectBackVector2s[3] = new Vector2(970,290);
		
		selectBackSourceVector2s = new Vector2[4];
		selectBackSourceVector2s[0] = new Vector2(470,670);
		selectBackSourceVector2s[1] = new Vector2(970,670);
		selectBackSourceVector2s[2] = new Vector2(470,290);
		selectBackSourceVector2s[3] = new Vector2(970,290);
		
		selectBackHideVector2s = new Vector2[4];
		selectBackHideVector2s[0] = new Vector2(-250,1150);
		selectBackHideVector2s[1] = new Vector2(1690,1150);
		selectBackHideVector2s[2] = new Vector2(-250,-190);
		selectBackHideVector2s[3] = new Vector2(1690,-190);
		
		
		cardMoveProperty = new MoveProperty(new Vector2(970,1293), new Vector2(970,670),new Vector2(450,600));
		
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
	
	private void drawBack(){
		batcher.beginBatch(Assests.selectBackTexture);
		batcher.drawSprite(camWidth/2, camHeidth/2, camWidth, camHeidth, Assests.selectBackTextureRegion);
		batcher.endBatch();
	}
	
	private void drawBackGround(){
		

		
        batcher.beginBatch(Assests.background);
        //batcher.drawSprite(camWidth/2, camHeidth/2, camWidth-400, camHeidth-200, Assests.backgroundRegion);

        for(int i = 0;i<4;i++){
        	batcher.drawSprite(selectBackVector2s[i].x, selectBackVector2s[i].y,selsectBckSize.x , selsectBckSize.y, Assests.selectTexture[i]);
        }
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
	
	
	private void drawRunningPlayer(){
		batcher.beginBatch(Assests.animationTexture);
		TextureRegion textureRegion = AnimationManager.getAnimation("littleBoyWalkRight").getKeyFrame(runningPlayerTimer, Animation.ANIMATION_LOOPING);
		batcher.drawSprite(runningPlayerAnimationPosition.x, runningPlayerAnimationPosition.y, runningPlayerAnimationSize.x, runningPlayerAnimationSize.y, textureRegion);
		Log.d("runningplayer", "timer:" + runningPlayerTimer + " region:" + textureRegion.toString());
		batcher.endBatch();
	}
	
	private void drawCard(){
		batcher.beginBatch(Assests.cardTexture);
		
		//TextureRegion textureRegion = cardMoveProperty.getNowTextureRegion();
		TextureRegion textureRegion = Assests.cardTextureRegionMap.get(PLAYERICON.LVQINGCHENG);
		if(textureRegion!= null){
			batcher.drawSprite(cardMoveProperty.getNowPosition().x, cardMoveProperty.getNowPosition().y, 
					cardMoveProperty.getSize().x, cardMoveProperty.getSize().y, textureRegion);
		}
		
		batcher.endBatch();
	}
	
	@Override
	public void update(float deltaTime) {
		// TODO Auto-generated method stub
		
		if(deltaTime > 0.1f)
	        deltaTime = 0.1f;
		
		List<TouchEvent> touchEvents = game.getInput().getTouchEvents();
		if(touchEvents.size() > 0){
			for(TouchEvent te : touchEvents){
				touchPoint.set(te.x, te.y);
		        guiCam.touchToWorld(touchPoint);
		        if(te.type == TouchEvent.TOUCH_DOWN){
		        	
		        	for(CharacterIcon ci:listIcon){
						if(ci.checkTouched(touchPoint.x, touchPoint.y)){
							selectedNameStr = ci.getNameDesc();
							selectedCi = ci;
							moveSelectBack();
							showCard(ci.getPlayerName());
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
		
		

		updateActions(deltaTime);
		
		runningPlayerTimer += deltaTime;
	}
	
	private void updateActions(float deltaTime){
		for(MultiPointMoveAction movePlayerAction:movePlayerActions){
			movePlayerAction.update(deltaTime);
		}
		
		Iterator<MultiPointMoveAction> iterator = movePlayerActions.iterator();
		while(iterator.hasNext()){
			MultiPointMoveAction movePlayerAction = iterator.next();
			if(movePlayerAction.isFinished()){
				iterator.remove();
			}
		}
	}
	
	private void showCard(PLAYERICON playericon){
		cardMoveProperty.resetNowPosition();
		cardMoveProperty.setNowTextureRegion(Assests.cardTextureRegionMap.get(playericon));
		MultiPointMoveAction multiPointMoveAction = new MultiPointMoveAction(cardMoveProperty.getNowPosition(),10);
		multiPointMoveAction.addAimPositon(cardMoveProperty.getAimPosition());
		multiPointMoveAction.setAcceleSpeed(2);
		multiPointMoveAction.start();
		movePlayerActions.add(multiPointMoveAction);
	}
	
	private void moveSelectBack(){
		for(int i =0;i<4;i++){
			MultiPointMoveAction multiPointMoveAction = new MultiPointMoveAction(selectBackVector2s[i], 10);
			multiPointMoveAction.addAimPositon(new Vector2(selectBackSourceVector2s[i]));
			multiPointMoveAction.addAimPositon(new Vector2(selectBackHideVector2s[i]));
			multiPointMoveAction.setAcceleSpeed(2);
			multiPointMoveAction.start();
			movePlayerActions.add(multiPointMoveAction);
			//selectBackInCentor = false;
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
        
        drawBack();
        drawRunningPlayer();
        drawCard();
        drawBackGround();
        
        
        gl.glDisable(GL10.GL_BLEND);
	}

	@Override
	public void pause() {
		// TODO Auto-generated method stub
		//Assests.music.pause();
	}

	@Override
	public void resume() {
		// TODO Auto-generated method stub
		//Assests.music.play();
	}

	@Override
	public void dispose() {
		// TODO Auto-generated method stub
		//Assests.music.dispose();
	}

}
