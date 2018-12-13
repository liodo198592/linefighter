package client.badlogic.androidgames.framework.gl;

import javax.microedition.khronos.opengles.GL10;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.opengl.GLUtils;

import client.badlogic.androidgames.framework.impl.GLGame;

public class TextureText extends Texture {

	String textString;
	public TextureText(GLGame glGame,String textString){
		super(glGame, "");
		this.textString = textString;
	}
	
	protected void load(){
        GL10 gl = glGraphics.getGL();
        int[] textureIds = new int[1];
        gl.glGenTextures(1, textureIds, 0);
        textureId = textureIds[0];
        
        Bitmap bitmap = Bitmap.createBitmap(256, 256, Bitmap.Config.ARGB_8888);
        Canvas canvas = new Canvas(bitmap);
        Paint paint = new Paint();
        paint.setColor(Color.WHITE);
        paint.setStyle(Style.FILL);
        
        canvas.drawText(this.textString, 10, 15, paint);
        gl.glEnable(GL10.GL_TEXTURE_2D);
        
        gl.glBindTexture(GL10.GL_TEXTURE_2D, textureId);
        GLUtils.texImage2D(GL10.GL_TEXTURE_2D, 0, bitmap, 0);
        setFilters(GL10.GL_NEAREST, GL10.GL_NEAREST);
        gl.glBindTexture(GL10.GL_TEXTURE_2D, 0);
        width = bitmap.getWidth();
        height = bitmap.getHeight();
        bitmap.recycle();
	}
}
