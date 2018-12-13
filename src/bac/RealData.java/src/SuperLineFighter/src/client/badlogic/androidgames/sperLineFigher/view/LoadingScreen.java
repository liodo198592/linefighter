package client.badlogic.androidgames.sperLineFigher.view;

import java.util.List;

import javax.microedition.khronos.opengles.GL10;

import org.apache.commons.logging.Log;

import client.badlogic.androidgames.sperLineFigher.resource.Assests;
import client.badlogic.androidgames.sperLineFigher.view.selectmenu.SelectScreen;
import client.badlogic.androidgames.framework.Game;
import client.badlogic.androidgames.framework.Input.TouchEvent;
import client.badlogic.androidgames.framework.gl.Camera2D;
import client.badlogic.androidgames.framework.gl.SpriteBatcher;
import client.badlogic.androidgames.framework.impl.GLScreen;
import client.badlogic.androidgames.framework.math.Vector2;

public class LoadingScreen extends GLScreen {

	private float LastTime = 0;
	Game game;
	
	Camera2D guiCam;
    SpriteBatcher batcher;
    
	public LoadingScreen(Game game){
		super(game);
		this.game = game;
		guiCam = new Camera2D(glGraphics, 512, 320);
		batcher = new SpriteBatcher(glGraphics, 100);
	}
	
	@Override
	public void update(float deltaTime) {
		// TODO Auto-generated method stub
		
		if(deltaTime > 0.1f)
	        deltaTime = 0.1f;
		
		LastTime += deltaTime;
		
		
		
		List<TouchEvent> touchEvents = game.getInput().getTouchEvents();
        game.getInput().getKeyEvents();

	}

	@Override
	public void present(float deltaTime) {
		// TODO Auto-generated method stub

		
		//android.util.Log.d("superLineFighter", "lastTime:" + LastTime);
		float alpha = (float) (1 - LastTime/0.3*0.1);
		GL10 gl = glGraphics.getGL();        
        gl.glClear(GL10.GL_COLOR_BUFFER_BIT);
        gl.glEnable(GL10.GL_TEXTURE_2D);
        
        if(LastTime >= 3){
        	gl.glColor4f(1, 1, 1, 1);
			game.setScreen(new SelectScreen(game));
			return;
		}
        
        guiCam.setViewportAndMatrices();
        gl.glColor4f(1, 1, 1, alpha);//…Ë÷√Ω•±‰
        
        gl.glEnable(GL10.GL_BLEND);
        gl.glBlendFunc(GL10.GL_SRC_ALPHA, GL10.GL_ONE_MINUS_SRC_ALPHA); 
        
        
        batcher.beginBatch(Assests.loadingTexture);
        batcher.drawSprite(256, 160, 512, 320, Assests.loadingTextureRegion);
        batcher.endBatch();
        
        gl.glDisable(GL10.GL_BLEND);
		
		
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
