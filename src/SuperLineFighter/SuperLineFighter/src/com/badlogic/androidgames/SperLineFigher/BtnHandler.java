package com.badlogic.androidgames.SperLineFigher;

import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import javax.microedition.khronos.opengles.GL10;

import LineFighter.Core.Action;
import LineFighter.Core.Const;
import LineFighter.Core.Const.EACTION;

import android.text.StaticLayout;
import android.util.Log;

import com.badlogic.androidgames.SperLineFigher.Assests.SKILL;
import com.badlogic.androidgames.framework.GameObject;
import com.badlogic.androidgames.framework.Input.TouchEvent;
import com.badlogic.androidgames.framework.gl.Camera2D;
import com.badlogic.androidgames.framework.gl.SpriteBatcher;
import com.badlogic.androidgames.framework.math.OverlapTester;
import com.badlogic.androidgames.framework.math.Vector2;

public class BtnHandler {

	public static enum BTNTYPE{
		QUAN,
		SHUAI,
		QIGONG_LEFT,
		QIGONG_RIGHT,
		BISHA_LEFT,
		BISHA_RIGHT,
		SHANBI,
		YIDONG_1,
		YIDONG_2,
		YIDONG_3,
		HUANXIAN,
		CONFIRM,
		CANCEL
		
	}
	
	HashMap<Assests.SKILL, GameObject> actionBtnRectangleHashMap;
	HashMap<Assests.SKILL, GameObject> actionNameRectangleHashMap;
	
	GameObject moveBoardObject;
	GameObject move_p1Object;
	GameObject move_p2Object;
	GameObject move_p3Object;
	
	GameObject qigongBoardObject;
	GameObject qigong_leftObject;
	GameObject qigong_rightObject;
	
	GameObject bishBoardObject;
	GameObject bisha_leftObject;
	GameObject bisha_rightObject;
	
	GameObject executeObject;
	GameObject cancelObject;
	
	GameObject[] actionNameGameObjects;
	
	SpriteBatcher batcher;
	Camera2D guiCam;
	Vector2 touchPoint;
	
	boolean moveBoardShow = false;
	boolean qigongBoardShow = false;
	boolean bishaBoardShow = false;
	
	
	Map<Long,Action> inputAction;

	public BtnHandler(SpriteBatcher batcher,Camera2D guiCam){
		
		this.batcher = batcher;
		this.guiCam = guiCam;
		init();
		touchPoint = new Vector2();
		inputAction = new LinkedHashMap<Long,Action>();
		
	}
	
	private void init(){
		actionBtnRectangleHashMap = new HashMap<Assests.SKILL, GameObject>();
		actionBtnRectangleHashMap.put(Assests.SKILL.QUAN, new GameObject(131, 133, 107, 107));
		actionBtnRectangleHashMap.put(Assests.SKILL.SHUAI, new GameObject(262, 133, 107, 107));
		actionBtnRectangleHashMap.put(Assests.SKILL.QIGONG, new GameObject(393, 133, 107, 107));
		actionBtnRectangleHashMap.put(Assests.SKILL.BISHA, new GameObject(524, 133, 107, 107));
		actionBtnRectangleHashMap.put(Assests.SKILL.SHANBI, new GameObject(655, 133, 107, 107));
		actionBtnRectangleHashMap.put(Assests.SKILL.YIDONG, new GameObject(786, 133, 107, 107));
		actionBtnRectangleHashMap.put(Assests.SKILL.HUANXIAN, new GameObject(917, 133, 107, 107));
		
		actionNameRectangleHashMap = new HashMap<Assests.SKILL, GameObject>();
		actionNameRectangleHashMap.put(Assests.SKILL.QUAN, new GameObject(131, 64, 120, 30));
		actionNameRectangleHashMap.put(Assests.SKILL.SHUAI, new GameObject(262, 64, 120, 30));
		actionNameRectangleHashMap.put(Assests.SKILL.QIGONG, new GameObject(393, 64, 120, 30));
		actionNameRectangleHashMap.put(Assests.SKILL.BISHA, new GameObject(524, 64, 120, 30));
		actionNameRectangleHashMap.put(Assests.SKILL.SHANBI, new GameObject(655, 64, 120, 30));
		actionNameRectangleHashMap.put(Assests.SKILL.YIDONG, new GameObject(786, 64, 120, 30));
		actionNameRectangleHashMap.put(Assests.SKILL.HUANXIAN, new GameObject(917, 64, 120, 30));
		
		
		moveBoardObject = new GameObject(786, 275, 655, 150);
		move_p1Object = new GameObject(593, 275, 167, 77);
		move_p2Object = new GameObject(786, 275, 167, 77);
		move_p3Object = new GameObject(979, 275, 167, 77);
		
		qigongBoardObject =  new GameObject(393, 275, 437, 150);
		qigong_leftObject = new GameObject(284, 275, 167, 77);
		qigong_rightObject = new GameObject(502, 275, 167, 77);
		
		bishBoardObject = new GameObject(524, 275, 437, 150);
		bisha_leftObject = new GameObject(415, 275, 167, 77);
		bisha_rightObject = new GameObject(633, 275, 167, 77);
		
		executeObject = new GameObject(1140,107,137,60);
		
		cancelObject = new GameObject(1307, 107, 137, 60);
		
		actionNameGameObjects = new GameObject[3];
		actionNameGameObjects[0] = new GameObject(1220, 284, 125, 36);
		actionNameGameObjects[1] = new GameObject(1220, 236, 125, 36);
		actionNameGameObjects[2] = new GameObject(1220, 188, 125, 36);
	}
	
	public BTNTYPE update(TouchEvent event,int myRow){
		
		BTNTYPE btnType = null;
		if(event.type == TouchEvent.TOUCH_DOWN){
			
			touchPoint.set(event.x, event.y);
	        guiCam.touchToWorld(touchPoint);
			
	        btnType = handleConfirm();
	        if(btnType == null){
	        	btnType = handleAction(myRow);
	        }
	        
		}
		
		return btnType;
	}
	
	private BTNTYPE handleAction(int myRow){
		BTNTYPE btnType = null;
		if(moveBoardShow){
        	//若打开了移动显示框，则只处理此框内的触摸事件
        	
        	//long aimY = 0;
			if(inputAction.size() < 3){
				Action myAction = new Action(Const.getEaction(4L));
				if(OverlapTester.pointInRectangle(move_p1Object.bounds,touchPoint)){
					if(myRow == 1){
						myAction.setDestX(1L);
						myAction.setDestY(1L);
					}else{
						myAction.setDestX(2L);
						myAction.setDestY(1L);
					}
					Long id = new Long(inputAction.size() + 1);
					inputAction.put(id , myAction);
					btnType = BTNTYPE.YIDONG_1;
				}else if(OverlapTester.pointInRectangle(move_p2Object.bounds,touchPoint)){
					//aimY = 2;
					if(myRow == 1){
						myAction.setDestX(1L);
						myAction.setDestY(2L);
					}else{
						myAction.setDestX(2L);
						myAction.setDestY(2L);
					}
					Long id = new Long(inputAction.size() + 1);
					inputAction.put(id , myAction);
					btnType = BTNTYPE.YIDONG_2;
				}else if(OverlapTester.pointInRectangle(move_p3Object.bounds,touchPoint)){
					//aimY = 3;
					if(myRow == 1){
						myAction.setDestX(1L);
						myAction.setDestY(3L);
					}else{
						myAction.setDestX(2L);
						myAction.setDestY(3L);
					}
					Long id = new Long(inputAction.size() + 1);
					inputAction.put(id , myAction);
					btnType = BTNTYPE.YIDONG_3;
				}
    		}
        	
			moveBoardShow = false;
        }else if(qigongBoardShow){
			
        	if(inputAction.size() < 3){
             	Action myAction = new Action(Const.getEaction(2L));
            	if(OverlapTester.pointInRectangle(qigong_leftObject.bounds,touchPoint)){
            		myAction.setDerection(1L);
            		Long id = new Long(inputAction.size() + 1);
        			inputAction.put(id , myAction);
        			btnType = BTNTYPE.QIGONG_LEFT;
            	}else if(OverlapTester.pointInRectangle(qigong_rightObject.bounds,touchPoint)){
            		myAction.setDerection(2L);
            		Long id = new Long(inputAction.size() + 1);
        			inputAction.put(id , myAction);
        			btnType = BTNTYPE.QIGONG_RIGHT;
            	}
        	}
   
        	
        	
			
        	qigongBoardShow = false;
		}else if(bishaBoardShow){
			if(inputAction.size() < 3){
				Action myAction = new Action(Const.getEaction(7L));
				if(OverlapTester.pointInRectangle(bisha_leftObject.bounds,touchPoint)){
					myAction.setDerection(1L);
					Long id = new Long(inputAction.size() + 1);
					inputAction.put(id , myAction);
					btnType = BTNTYPE.BISHA_LEFT;
	        	}else if(OverlapTester.pointInRectangle(bisha_rightObject.bounds,touchPoint)){
	        		myAction.setDerection(2L);
	        		Long id = new Long(inputAction.size() + 1);
	    			inputAction.put(id , myAction);
	    			btnType = BTNTYPE.BISHA_RIGHT;
	        	}
			}

			
			bishaBoardShow = false;
		}
        else {
        	if(OverlapTester.pointInRectangle(actionBtnRectangleHashMap.get(Assests.SKILL.QUAN).bounds,
					touchPoint)){
				//若按钮为拳
        		if(inputAction.size() >= 3){
        			return null;
        		}
        		Action act = new Action(Const.getEaction(1L));
        		Long id = new Long(inputAction.size() + 1);
        		inputAction.put(id , act);
				
        		btnType = BTNTYPE.QUAN;
			}
        	
        	if(OverlapTester.pointInRectangle(actionBtnRectangleHashMap.get(Assests.SKILL.SHUAI).bounds,
					touchPoint)){
				//若按钮为摔
        		if(inputAction.size() >= 3){
        			return null;
        		}
        		Action act = new Action(Const.getEaction(3L));
        		Long id = new Long(inputAction.size() + 1);
        		inputAction.put(id , act);
        		
        		btnType = BTNTYPE.SHUAI;
			}
        	
			
			if(OverlapTester.pointInRectangle(actionBtnRectangleHashMap.get(Assests.SKILL.QIGONG).bounds,
					touchPoint)){
				//若按钮为气功
				qigongBoardShow = true;
			}
			
			if(OverlapTester.pointInRectangle(actionBtnRectangleHashMap.get(Assests.SKILL.BISHA).bounds,
					touchPoint)){
				//若按钮为必杀
				bishaBoardShow = true;
			}
			
			if(OverlapTester.pointInRectangle(actionBtnRectangleHashMap.get(Assests.SKILL.SHANBI).bounds,
					touchPoint)){
				//若按钮为闪避
				
			}
			
			if(OverlapTester.pointInRectangle(actionBtnRectangleHashMap.get(Assests.SKILL.YIDONG).bounds,
					touchPoint)){
				//若按钮时移动
				moveBoardShow = true;
			}
			
			if(OverlapTester.pointInRectangle(actionBtnRectangleHashMap.get(Assests.SKILL.HUANXIAN).bounds,
					touchPoint)){
				//若按钮为换线
				
				if(inputAction.size() >= 3){
					return null;
				}
				Action act = new Action(Const.getEaction(5L));
				Long id = new Long(inputAction.size() + 1);
				inputAction.put(id , act);
				btnType = BTNTYPE.HUANXIAN;
			}
			
		}
		
		return btnType;
	}
	
	private BTNTYPE handleConfirm(){
		
		BTNTYPE btnType = null;
		if(OverlapTester.pointInRectangle(executeObject.bounds,touchPoint)){
			if(inputAction.size() != 3){
				return null;
			}
			
			
			//通知引擎
			
			
			btnType = BTNTYPE.CONFIRM;
		}
		
		if(OverlapTester.pointInRectangle(cancelObject.bounds,touchPoint)){
			//取消按钮
			if(inputAction.isEmpty()){
				return null;
			}
			Long index = new Long(inputAction.size());
			inputAction.remove(index);
			
			btnType = BTNTYPE.CANCEL;
		}
		
		
		return btnType;
	}
	
	public void render(){
		
		batcher.beginBatch(Assests.item);
		drawActionBtn();
		
		drawConfirmBtn();
		
		drawMoveBoard();
		
		drawQigongBoard();
		
		drawBishaBoard();
		
		drawActionName();
		
		batcher.endBatch();
	}
	
	private void drawActionBtn(){
		//batcher.beginBatch(Assests.item);
		
		Iterator iterator = actionBtnRectangleHashMap.entrySet().iterator();
		
		
		
		while(iterator.hasNext()){
			Map.Entry entry = (Map.Entry) iterator.next();
			GameObject tempObject = (GameObject)entry.getValue();
			batcher.drawSprite(tempObject.position.x, tempObject.position.y, 
			tempObject.bounds.width, tempObject.bounds.height, 
			Assests.skillIconTextureRegionHashMap.get(entry.getKey()));
		}

		iterator = actionNameRectangleHashMap.entrySet().iterator();
		
		while(iterator.hasNext()){
			Map.Entry entry = (Map.Entry) iterator.next();
			GameObject tempObject = (GameObject)entry.getValue();
			batcher.drawSprite(tempObject.position.x, tempObject.position.y, 
			tempObject.bounds.width, tempObject.bounds.height, 
			Assests.skillNameTextureRegionHashMap.get(entry.getKey()));
		}

		
		//batcher.endBatch();
	}
	
	void drawConfirmBtn(){
		//batcher.beginBatch(Assests.item);
		
		batcher.drawSprite(executeObject.position.x, executeObject.position.y,
				executeObject.bounds.width, executeObject.bounds.height, Assests.execuTextureRegion);
		batcher.drawSprite(cancelObject.position.x, cancelObject.position.y,
				cancelObject.bounds.width, cancelObject.bounds.height, Assests.cancelTextureRegion);
		
		//batcher.endBatch();
	}
	private void drawMoveBoard(){
		
		if(!moveBoardShow){
			return;
		}
		//batcher.beginBatch(Assests.item);
		
		batcher.drawSprite(moveBoardObject.position.x, moveBoardObject.position.y, 
				moveBoardObject.bounds.width, moveBoardObject.bounds.height, 
				Assests.blackBoardTextureRegion);
		
		batcher.drawSprite(move_p1Object.position.x, move_p1Object.position.y, 
				move_p1Object.bounds.width, move_p1Object.bounds.height, 
				Assests.positionBtnTextureRegions[0]);
		
		batcher.drawSprite(move_p2Object.position.x, move_p2Object.position.y, 
				move_p2Object.bounds.width, move_p2Object.bounds.height, 
				Assests.positionBtnTextureRegions[1]);
		
		batcher.drawSprite(move_p3Object.position.x, move_p3Object.position.y, 
				move_p3Object.bounds.width, move_p3Object.bounds.height, 
				Assests.positionBtnTextureRegions[2]);
		
		//batcher.endBatch();
		
	}
	
	private void drawQigongBoard(){
		
		if(!qigongBoardShow){
			return;
		}
		//batcher.beginBatch(Assests.item);
		batcher.drawSprite(qigongBoardObject.position.x, qigongBoardObject.position.y, 
				qigongBoardObject.bounds.width, qigongBoardObject.bounds.height, 
				Assests.blackBoardTextureRegion);
		
		batcher.drawSprite(qigong_leftObject.position.x, qigong_leftObject.position.y, 
				qigong_leftObject.bounds.width, qigong_leftObject.bounds.height, 
				Assests.leftBtnTextureRegion);
		
		batcher.drawSprite(qigong_rightObject.position.x, qigong_rightObject.position.y, 
				qigong_rightObject.bounds.width, qigong_rightObject.bounds.height, 
				Assests.rightBtnTextureRegion);
		
		//batcher.endBatch();
	}
	
	private void drawBishaBoard(){
		
		if(!bishaBoardShow){
			return;
		}
		
		//batcher.beginBatch(Assests.item);
		batcher.drawSprite(bishBoardObject.position.x, bishBoardObject.position.y, 
				bishBoardObject.bounds.width, bishBoardObject.bounds.height, 
				Assests.blackBoardTextureRegion);
		
		batcher.drawSprite(bisha_leftObject.position.x, bisha_leftObject.position.y, 
				bisha_leftObject.bounds.width, bisha_leftObject.bounds.height, 
				Assests.leftBtnTextureRegion);
		
		batcher.drawSprite(bisha_rightObject.position.x, bisha_rightObject.position.y, 
				bisha_rightObject.bounds.width, bisha_rightObject.bounds.height, 
				Assests.rightBtnTextureRegion);
		
		//batcher.endBatch();
		
	}
	
	private void drawActionName(){
		
		if(inputAction.size() == 0){
			return;
		}
		
		//batcher.beginBatch(Assests.item);
		
		Iterator iterator = inputAction.entrySet().iterator();
		int i = 0;
		while (iterator.hasNext()) {
			Map.Entry entry = (Map.Entry)iterator.next();
			Long iActionIndex = (Long)entry.getKey() - 1;
			Action action = (Action)entry.getValue();
			
			
			GameObject tempObject = actionNameGameObjects[iActionIndex.intValue()];
			
			switch(action.getEtype()){
			case EACTION_Hand:{
				batcher.drawSprite(tempObject.position.x, tempObject.position.y,
						tempObject.bounds.width, tempObject.bounds.height, 
						Assests.skillNameTextureRegionHashMap.get(Assests.SKILL.QUAN));
				break;
			}
			case EACTION_Air:{
				batcher.drawSprite(tempObject.position.x, tempObject.position.y,
						tempObject.bounds.width, tempObject.bounds.height, 
						Assests.skillNameTextureRegionHashMap.get(Assests.SKILL.QIGONG));
				
				if(action.getDerection() == 1L){
					//向左
					batcher.drawSprite(tempObject.position.x + 60, tempObject.position.y,
							tempObject.bounds.width, tempObject.bounds.height, 
							Assests.leftWordRegion);
				}else {
					//向右
					batcher.drawSprite(tempObject.position.x + 60, tempObject.position.y,
							tempObject.bounds.width, tempObject.bounds.height, 
							Assests.rightWordRegion);
				}
				break;
			}
			case EACTION_Fall:{
				batcher.drawSprite(tempObject.position.x, tempObject.position.y,
						tempObject.bounds.width, tempObject.bounds.height, 
						Assests.skillNameTextureRegionHashMap.get(Assests.SKILL.SHUAI));
				break;
			}
			case EACTION_Move:{
				
				batcher.drawSprite(tempObject.position.x, tempObject.position.y,
						tempObject.bounds.width, tempObject.bounds.height, 
						Assests.skillNameTextureRegionHashMap.get(Assests.SKILL.YIDONG));
				
				if(action.getDestY() == 1L){
					//位置1
					batcher.drawSprite(tempObject.position.x + 80, tempObject.position.y,
							tempObject.bounds.width, tempObject.bounds.height, 
							Assests.positionWord1Region);
				}else if(action.getDestY() == 2L){
					//位置2
					batcher.drawSprite(tempObject.position.x + 80, tempObject.position.y,
							tempObject.bounds.width, tempObject.bounds.height, 
							Assests.positionWord2Region);
					
				}else if(action.getDestY() == 3L){
					//位置3
					batcher.drawSprite(tempObject.position.x + 80, tempObject.position.y,
							tempObject.bounds.width, tempObject.bounds.height, 
							Assests.positionWord3Region);
				}
				
				
				break;
			}
			case EACTION_ChangeLine:{
				batcher.drawSprite(tempObject.position.x, tempObject.position.y,
						tempObject.bounds.width, tempObject.bounds.height, 
						Assests.skillNameTextureRegionHashMap.get(Assests.SKILL.HUANXIAN));
				break;
			}
			case EACTION_Avoid:{
				break;
			}
			case EACTION_Kill:{
				batcher.drawSprite(tempObject.position.x, tempObject.position.y,
						tempObject.bounds.width, tempObject.bounds.height, 
						Assests.skillNameTextureRegionHashMap.get(Assests.SKILL.BISHA));
				
				if(action.getDerection() == 1L){
					//向左
					batcher.drawSprite(tempObject.position.x + 60, tempObject.position.y,
							tempObject.bounds.width, tempObject.bounds.height, 
							Assests.leftWordRegion);
				}else {
					//向右
					batcher.drawSprite(tempObject.position.x + 60, tempObject.position.y,
							tempObject.bounds.width, tempObject.bounds.height, 
							Assests.rightWordRegion);
				}
				break;
			}
			case EACTION_Invalid:{
				break;
			}
			}
		}
		
		//batcher.endBatch();
	
		
	}
	

	public Map<Long, Action> getInputAction() {
		return inputAction;
	}

	
}
