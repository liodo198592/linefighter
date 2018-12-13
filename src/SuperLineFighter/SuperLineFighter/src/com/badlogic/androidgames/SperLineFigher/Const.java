package com.badlogic.androidgames.SperLineFigher;

import com.badlogic.androidgames.framework.gl.TextureRegion;
import com.badlogic.androidgames.framework.math.Vector2;

public class Const {

	public static class GameElement{
		public Vector2 position = new Vector2();
		public float weight;
		public float height;
		TextureRegion textureRegion;
		public float transparent;//͸���ȣ�1����ȫ��͸��  0��ȫ͸��
		
		public GameElement(Vector2 position,float weight,float height,TextureRegion textureRegion,float transparent){
			this.position.set(position);
			this.weight = weight;
			this.height = height;
			this.textureRegion = textureRegion;
			this.transparent = transparent;
		}
		
	}
}
