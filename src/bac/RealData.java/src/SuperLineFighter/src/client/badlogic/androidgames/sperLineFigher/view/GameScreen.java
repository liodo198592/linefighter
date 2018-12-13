package client.badlogic.androidgames.sperLineFigher.view;


import java.util.List;


import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;




import client.badlogic.androidgames.sperLineFigher.control.BtnHandler;
import client.badlogic.androidgames.sperLineFigher.control.World;
import client.badlogic.androidgames.sperLineFigher.control.BtnHandler.BTNTYPE;
import client.badlogic.androidgames.sperLineFigher.control.World.WordState;
import client.badlogic.androidgames.sperLineFigher.model.FighterObject;
import client.badlogic.androidgames.sperLineFigher.resource.Assests;
import client.badlogic.androidgames.sperLineFigher.resource.Assests.PLAYERICON;
import client.badlogic.androidgames.framework.Game;

import client.badlogic.androidgames.framework.Input.TouchEvent;
import client.badlogic.androidgames.framework.gl.Camera2D;
import client.badlogic.androidgames.framework.gl.SpriteBatcher;
import client.badlogic.androidgames.framework.impl.GLScreen;
import client.badlogic.androidgames.framework.math.OverlapTester;
import client.badlogic.androidgames.framework.math.Vector2;

//”Œœ∑ΩÁ√Ê
public class GameScreen extends GLScreen {

    enum GameState {
    	Ready,
        Running,
        Paused,
        GameOver
    }
    
    private static final int camWidth = 1440;
    private static final int camHeidth = 960;
    

    
    GameState state = GameState.Running;
	
    Camera2D guiCam;
    Vector2 touchPoint;
    SpriteBatcher batcher;
	
    World world;
    WorderRenderer worderRenderer;
    BtnHandler btnHandler;
    
    Vector2[] positionCoordinate;
    
    
    Game game;
	public GameScreen(Game game){
		super(game);
		this.game = game;
		
		init();
		
	}

	private void init(){
		guiCam = new Camera2D(glGraphics, camWidth, camHeidth);
		batcher = new SpriteBatcher(glGraphics, 1000);
		touchPoint = new Vector2();   
		
		world = new World(((Activity)game).getMainLooper());
		worderRenderer = new WorderRenderer(world);
		btnHandler = new BtnHandler(batcher,guiCam);
		
		//generatePlayerTest();
	}
	

	
	private void generatePlayerTest(){
		Vector2 position = null;
		position = world.coordinateManager.centorPositonCordinates[0];
		FighterObject fighterObject1 = new FighterObject("cyt", position.x, position.y, 
				FighterObject.playerIconWidth, FighterObject.playerIconHeight);
		fighterObject1.setZhiye(PLAYERICON.LUSANJIN);
		world.addFighter(fighterObject1);
		world.setMyName("cyt");
		
		position = world.coordinateManager.topPositionCordinates[1];
		FighterObject fighterObject2 = new FighterObject("cht", position.x, position.y, 
				FighterObject.playerIconWidth, FighterObject.playerIconHeight);
		fighterObject2.setZhiye(PLAYERICON.WENLIANGGONG);
		world.addFighter(fighterObject2);
		
		position = world.coordinateManager.leftBottomCordinates[2];
		FighterObject fighterObject3 = new FighterObject("zhouyu", position.x, position.y, 
				FighterObject.playerIconWidth, FighterObject.playerIconHeight);
		fighterObject3.setZhiye(PLAYERICON.BAIJINGJI);
		world.addFighter(fighterObject3);
		
		position = world.coordinateManager.rightBottomCordinates[3];
		FighterObject fighterObject4 = new FighterObject("jinwei", position.x, position.y, 
				FighterObject.playerIconWidth, FighterObject.playerIconHeight);
		fighterObject4.setZhiye(PLAYERICON.SHENGQIUYUE);
		world.addFighter(fighterObject4);
		
	}
	
	@Override
	public void update(float deltaTime) {
		
		if(deltaTime > 0.1f)
	        deltaTime = 0.1f;
		
		List<TouchEvent> touchEvents = game.getInput().getTouchEvents();
        game.getInput().getKeyEvents();
        
        

        if(state == GameState.Running)
            updateRunning(touchEvents, deltaTime);
        if(state == GameState.Paused)
            updatePaused(touchEvents);
        if(state == GameState.GameOver)
            updateGameOver(touchEvents);  
		
	}

	private void updateRunning(List<TouchEvent> touchEvents, float deltaTime){
		int len = touchEvents.size();
		
		for(int i = 0; i < len; i++) {
			TouchEvent event = touchEvents.get(i);
			touchPoint.set(event.x, event.y);
	        guiCam.touchToWorld(touchPoint);
	        if(event.type == TouchEvent.TOUCH_DOWN){
	        	if(OverlapTester.pointInRectangle(worderRenderer.showSpeedObject.bounds, touchPoint)){
					world.onBtnShowSpeed();
					continue;
				}
	        }
			
			
			if(world.getWordState() == WordState.inputting){
				BTNTYPE btnType = btnHandler.update(event,1);
				if(btnType == BTNTYPE.CONFIRM){
					//≤‚ ‘********************************
//					Queue<StepActioin> stepActioins = new LinkedList<StepActioin>();
//					StepActioin stepActioin = new StepActioin();
//					stepActioin.showAction = SHOWACTION.MOVE;
//					stepActioin.name = "cyt";
//					stepActioin.positon = world.coordinateManager.centorPositonCordinates[1];
//					stepActioins.offer(stepActioin);
			
//					world.insertStepAction(stepActioins);
					//≤‚ ‘********************************
					
					world.onConfirm(btnHandler.getInputAction());
					world.setWordState(WordState.waitingActions);
				}
			}
			
//			world.getFighterObjects().get(0).move(world.coordinateManager.centorPositonCordinates[1]);
//			world.getFighterObjects().get(0).damageBloodTo(15);
		}
		
		
		world.update(deltaTime);
		
	}
	
	
	private void updatePaused(List<TouchEvent> touchEvents) {
		
	}
	
	private void updateGameOver(List<TouchEvent> touchEvents) {
		
	}
	
	@Override
	public void present(float deltaTime) {
		// TODO Auto-generated method stub
		
        GL10 gl = glGraphics.getGL();        
        gl.glClear(GL10.GL_COLOR_BUFFER_BIT);
        gl.glEnable(GL10.GL_TEXTURE_2D);
        
        guiCam.setViewportAndMatrices();
        
        gl.glEnable(GL10.GL_BLEND);
        gl.glBlendFunc(GL10.GL_SRC_ALPHA, GL10.GL_ONE_MINUS_SRC_ALPHA);  
        
        drawBackGround();
        
        if(state == GameState.Ready) 
            drawReadyUI();
        if(state == GameState.Running)
            drawRunningUI();
        if(state == GameState.Paused)
            drawPausedUI();
        if(state == GameState.GameOver)
            drawGameOverUI();
        
        
        gl.glDisable(GL10.GL_BLEND);
		
	}

	private void drawBackGround(){
        batcher.beginBatch(Assests.background);
        //batcher.drawSprite(camWidth/2, camHeidth/2, camWidth-400, camHeidth, Assests.backgroundRegion);
        batcher.endBatch();
	}
	private void drawRunningUI(){
		
		if(world.getWordState().equals(WordState.waitingNextRound)){
			worderRenderer.render(batcher,glGraphics.getGL());
		}else if(world.getWordState().equals(WordState.inputting)){
			//Log.d("SuperLineFighter", "wordstate" + world.getWordState());
			worderRenderer.render(batcher,glGraphics.getGL());
			btnHandler.render();
		}else if(world.getWordState().equals(WordState.waitingActions)){
			worderRenderer.render(batcher,glGraphics.getGL());
		}else if(world.getWordState().equals(WordState.showing)){
			worderRenderer.render(batcher,glGraphics.getGL());
			//btnHandler.render();
		}
		
		
	}

	private void drawPausedUI(){
			
	}
	private void drawGameOverUI(){
		
	}
	private void drawReadyUI(){
		
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
