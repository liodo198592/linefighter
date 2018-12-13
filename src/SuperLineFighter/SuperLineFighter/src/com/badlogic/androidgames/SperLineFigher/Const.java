package com.badlogic.androidgames.SperLineFigher;

import com.badlogic.androidgames.framework.gl.TextureRegion;
import com.badlogic.androidgames.framework.math.Vector2;

public class Const {

	public static class GameElement{
		public Vector2 position = new Vector2();
		public float weight;
		public float height;
		TextureRegion textureRegion;
		public float transparent;//透明度：1：完全不透明  0完全透明
		
		public GameElement(Vector2 position,float weight,float height,TextureRegion textureRegion,float transparent){
			this.position.set(position);
			this.weight = weight;
			this.height = height;
			this.textureRegion = textureRegion;
			this.transparent = transparent;
		}
		
	}
}
