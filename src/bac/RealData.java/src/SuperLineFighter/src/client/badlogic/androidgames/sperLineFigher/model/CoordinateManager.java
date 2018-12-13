package client.badlogic.androidgames.sperLineFigher.model;

import client.badlogic.androidgames.framework.math.Vector2;

public class CoordinateManager {

	
	
	public Vector2[] centorPositonCordinates;
	public Vector2[] topPositionCordinates;
	public Vector2[] leftBottomCordinates;
	public Vector2[] rightBottomCordinates;
	
	
	
	public CoordinateManager(){
		init();
	}
	
	public void init(){
		centorPositonCordinates = new Vector2[6];
		centorPositonCordinates[0] = new Vector2(208,752);
		centorPositonCordinates[1] = new Vector2(535,752);
		centorPositonCordinates[2] = new Vector2(862,752);
		centorPositonCordinates[3] = new Vector2(208,395);
		centorPositonCordinates[4] = new Vector2(535,395);
		centorPositonCordinates[5] = new Vector2(862,395);
		
		topPositionCordinates = new Vector2[6];
		topPositionCordinates[0] = new Vector2(208,850);
		topPositionCordinates[1] = new Vector2(535,850);
		topPositionCordinates[2] = new Vector2(862,850);
		topPositionCordinates[3] = new Vector2(208,493);
		topPositionCordinates[4] = new Vector2(535,493);
		topPositionCordinates[5] = new Vector2(862,493);
		
		leftBottomCordinates = new Vector2[6];
		leftBottomCordinates[0] = new Vector2(131,677);
		leftBottomCordinates[1] = new Vector2(458,677);
		leftBottomCordinates[2] = new Vector2(785,677);
		leftBottomCordinates[3] = new Vector2(131,320);
		leftBottomCordinates[4] = new Vector2(458,320);
		leftBottomCordinates[5] = new Vector2(785,320);
		
		rightBottomCordinates = new Vector2[6];
		rightBottomCordinates[0] = new Vector2(289,677);
		rightBottomCordinates[1] = new Vector2(616,677);
		rightBottomCordinates[2] = new Vector2(943,677);
		rightBottomCordinates[3] = new Vector2(289,320);
		rightBottomCordinates[4] = new Vector2(616,320);
		rightBottomCordinates[5] = new Vector2(943,320);
	}
	
}
