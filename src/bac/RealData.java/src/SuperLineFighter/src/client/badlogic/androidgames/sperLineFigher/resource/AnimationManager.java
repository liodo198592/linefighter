package client.badlogic.androidgames.sperLineFigher.resource;

import java.util.LinkedHashMap;
import java.util.Map;

import client.badlogic.androidgames.framework.gl.Animation;


public class AnimationManager {


	private static Map<String, Animation> animationMap = new LinkedHashMap<String, Animation>();
	
	public static void addAnimation(String animationName,Animation animation){
		if(animationName == null || animation == null){
			return;
		}
		
		animationMap.put(animationName, animation);
	}
	
	
	public static Animation getAnimation(String animationName){
		if(animationMap.containsKey(animationName)){
			return animationMap.get(animationName);
		}
		return null;
	}
	
	public static boolean isContainAnimation(String animationName){
		return animationMap.containsKey(animationName);
	}
}
