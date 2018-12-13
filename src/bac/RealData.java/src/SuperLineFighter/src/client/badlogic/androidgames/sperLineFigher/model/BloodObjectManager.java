package client.badlogic.androidgames.sperLineFigher.model;

import client.badlogic.androidgames.framework.GameObject;

public class BloodObjectManager{

	
	
	public GameObject[] playerIconObjects;

	
	public GameObject[] xueCaoObjects;

	public GameObject[] number1Objects;
	public GameObject[] number2Objects;
	
	public BloodObjectManager(){
		init();
	}
	
	private void init(){
		playerIconObjects = new GameObject[4];
		playerIconObjects[0] = new GameObject(1095, 859, 60, 60);
		playerIconObjects[1] = new GameObject(1095, 781, 60, 60);
		playerIconObjects[2] = new GameObject(1095, 703, 60, 60);
		playerIconObjects[3] = new GameObject(1095, 625, 60, 60);
		
		xueCaoObjects = new GameObject[4];
		xueCaoObjects[0] = new GameObject(1238, 859, 202, 42);
		xueCaoObjects[1] = new GameObject(1238, 781, 202, 42);
		xueCaoObjects[2] = new GameObject(1238, 703, 202, 42);
		xueCaoObjects[3] = new GameObject(1238, 625, 202, 42);
		
		number1Objects = new GameObject[4];
		number1Objects[0] = new GameObject(1357, 859, 24, 36);
		number1Objects[1] = new GameObject(1357, 781, 24, 36);
		number1Objects[2] = new GameObject(1357, 703, 24, 36);
		number1Objects[3] = new GameObject(1357, 625, 24, 36);
		
		number2Objects = new GameObject[4];
		number2Objects[0] = new GameObject(1381, 859, 24, 36);
		number2Objects[1] = new GameObject(1381, 781, 24, 36);
		number2Objects[2] = new GameObject(1381, 703, 24, 36);
		number2Objects[3] = new GameObject(1381, 625, 24, 36);
		
		
	}

	public GameObject getXueLiangObject(GameObject xuecaoObject,float maxBlood,float blood){
		
		float width = ((float)blood/maxBlood)*(xuecaoObject.bounds.width - 10);
		float height = xuecaoObject.bounds.height - 10;
		
		float x = xuecaoObject.bounds.lowerLeft.x + 5 + width/2;
		float y = xuecaoObject.position.y;
		
		return new GameObject(x, y, width, height);
		
	}
	
}
