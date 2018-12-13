package com.badlogic.androidgames.SperLineFigher;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import com.badlogic.androidgames.framework.Screen;
import com.badlogic.androidgames.framework.impl.GLGame;


public class SuperLineFigherGame extends GLGame {


	boolean firstTimeCreate = true;
	@Override
	public Screen getStartScreen() {
		// TODO Auto-generated method stub
		return new LoadingScreen(this);
		
	}


	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		// TODO Auto-generated method stub
		super.onSurfaceCreated(gl, config);
		
        if(firstTimeCreate) {
            //Settings.load(getFileIO());
            Assests.load(this);
            firstTimeCreate = false;            
        } else {
            Assests.reload();
        }
	}


    @Override
    public void onPause() {
        super.onPause();

    }
	

}
