package com.badlogic.androidgames.SperLineFigher;

import java.util.List;

import javax.microedition.khronos.opengles.GL10;

import org.apache.commons.logging.Log;

import com.badlogic.androidgames.SperLineFigher.SelectMenu.SelectScreen;
import com.badlogic.androidgames.framework.Game;
import com.badlogic.androidgames.framework.Input.TouchEvent;
import com.badlogic.androidgames.framework.gl.Camera2D;
import com.badlogic.androidgames.framework.gl.SpriteBatcher;
import com.badlogic.androidgames.framework.impl.GLScreen;
import com.badlogic.androidgames.framework.math.Vector2;

public class LoadingScreen extends GLScreen {

	private float LastTime = 0;
	Game game;
	
	Camera2D guiCam;
    SpriteBatcher batcher;
    
	public LoadingScreen(Game game){
		super(game);
		this.game = game;
		guiCam = new Camera2D(glGraphics, 480, 320);
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
		float alpha = (float) (1 - LastTime/0.2*0.1);
		GL10 gl = glGraphics.getGL();        
        gl.glClear(GL10.GL_COLOR_BUFFER_BIT);
        gl.glEnable(GL10.GL_TEXTURE_2D);
        
        if(LastTime >= 2){
        	gl.glColor4f(1, 1, 1, 1);
			game.setScreen(new SelectScreen(game));
			return;
		}
        
        guiCam.setViewportAndMatrices();
        gl.glColor4f(1, 1, 1, alpha);//…Ë÷√Ω•±‰
        
        gl.glEnable(GL10.GL_BLEND);
        gl.glBlendFunc(GL10.GL_SRC_ALPHA, GL10.GL_ONE_MINUS_SRC_ALPHA); 
        
        
        batcher.beginBatch(Assests.loadingTexture);
        batcher.drawSprite(250, 150, 200, 300, Assests.loadingTextureRegion);
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
