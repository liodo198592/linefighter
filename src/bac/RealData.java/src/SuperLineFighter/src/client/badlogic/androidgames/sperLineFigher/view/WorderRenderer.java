package client.badlogic.androidgames.sperLineFigher.view;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import javax.microedition.khronos.opengles.GL10;

import android.R.integer;
import android.util.Log;

import client.badlogic.androidgames.sperLineFigher.Const;
import client.badlogic.androidgames.sperLineFigher.Const.GameElement;
import client.badlogic.androidgames.sperLineFigher.control.World;
import client.badlogic.androidgames.sperLineFigher.control.BtnHandler.BTNTYPE;
import client.badlogic.androidgames.sperLineFigher.control.World.WordState;
import client.badlogic.androidgames.sperLineFigher.control.playeraction.PlayerAction;
import client.badlogic.androidgames.sperLineFigher.model.BloodObjectManager;
import client.badlogic.androidgames.sperLineFigher.model.FighterObject;
import client.badlogic.androidgames.sperLineFigher.resource.Assests;
import client.badlogic.androidgames.framework.GameObject;
import client.badlogic.androidgames.framework.gl.SpriteBatcher;
import client.badlogic.androidgames.framework.gl.TextureRegion;
import client.badlogic.androidgames.framework.math.Vector2;

public class WorderRenderer {
	
	public static enum SHOWACTION{
		NONE,
		MOVE,
		QUAN,
		SHUAI,
		QIGONG,
		BISHA,
		SHANBI,
		DAMAGED,
		AVOID
		
	}
	

	public static class StepActioin{
		//每个玩家要演示的动作及动作后的状态
		public String name;
		public SHOWACTION showAction;
		public Vector2 positon = new Vector2();
		public int blood;
		public int direction;//1向左 2向右
	}

	public static final int showAreaLeft = 24;//展示区域的最左边
	public static final int showArearight = 988;//展示区域的最右边
	
	private World world;
	public BloodObjectManager bloodObjectManager;
	
	public GameObject showSpeedObject;
	public WorderRenderer(World world){
		this.world = world;
		init();
	}
	

	
	private void init(){
		
		bloodObjectManager = new BloodObjectManager();
		showSpeedObject = new GameObject(113, 234, 130, 60);
	}
	
	public void render(SpriteBatcher batcher,GL10 gl){
		if(world.getWordState() == WordState.waitingNextRound){
			renderWaitNextRound(batcher,gl);
		}else if(world.getWordState() == WordState.inputting){
			renderInputting(batcher,gl);
		}else if(world.getWordState() == WordState.waitingActions){
			renderWaitActions(batcher,gl);
		}else if(world.getWordState() == WordState.showing){
			renderShowing(batcher,gl);
		}
	}
	
	private void renderWaitNextRound(SpriteBatcher batcher,GL10 gl){
		
		renderPlayers(batcher,gl);
		
		renderBlood(batcher);
		
	}
	
	private void renderInputting(SpriteBatcher batcher,GL10 gl){
		
		renderPlayers(batcher,gl);
		
		renderBlood(batcher);
		
		renderShowSpeed(batcher);
		
	}
	
	private void renderWaitActions(SpriteBatcher batcher,GL10 gl){
		renderPlayers(batcher,gl);
		
		renderBlood(batcher);
		
		renderShowSpeed(batcher);
	}
	
	private void renderShowing(SpriteBatcher batcher,GL10 gl){
		renderPlayers(batcher,gl);
		
		renderBlood(batcher);
		
		renderAttachedElement(batcher, gl);
		
		renderShowSpeed(batcher);
	}
	
	private void renderShowSpeed(SpriteBatcher batcher){
		batcher.beginBatch(Assests.item);
		
		if(PlayerAction.getShowSpeed() == 1){
			batcher.drawSprite(showSpeedObject.position.x, showSpeedObject.position.y, 
					showSpeedObject.bounds.width, showSpeedObject.bounds.height, Assests.showSpeed1);
		}else {
			batcher.drawSprite(showSpeedObject.position.x, showSpeedObject.position.y, 
					showSpeedObject.bounds.width, showSpeedObject.bounds.height, Assests.showSpeed2);
		}
		
		
		batcher.endBatch();
	}
	
	private void renderPlayers(SpriteBatcher batcher,GL10 gl){
		
		List<FighterObject> fighterObjects = world.getFighterObjects();
		if(fighterObjects.size() == 0){
			return;
		}
		
		batcher.beginBatch(Assests.item);
		
		
		
		int iIndex = 0;
		for(FighterObject fighterObject:fighterObjects){
			
			batcher.drawSprite(fighterObject.position.x, fighterObject.position.y,
					fighterObject.bounds.width, fighterObject.bounds.height, 
					Assests.playerIconHashMap.get(fighterObject.getZhiye()));
			
			//渲染附属元素
			
			
		}
		
		
		batcher.endBatch();
		
	}
	
	private void renderAttachedElement(SpriteBatcher batcher,GL10 gl){
		
		List<FighterObject> fighterObjects = world.getFighterObjects();
		
		for(FighterObject fighterObject:fighterObjects){
			if(fighterObject.elementList.isEmpty()){
				continue;
			}
			
			List<Const.GameElement> list = fighterObject.elementList;
			
			for(GameElement gameElement:list){
				gl.glColor4f(1, 1, 1, gameElement.transparent);
				batcher.beginBatch(Assests.item);
				
				batcher.drawSprite(gameElement.position.x, gameElement.position.y, 
						gameElement.weight, gameElement.height, gameElement.textureRegion);
				
				batcher.endBatch();
				gl.glColor4f(1, 1, 1, 1);
			}
			
		}
		
	}
	private void renderBlood(SpriteBatcher batcher){
		
		List<FighterObject> fighterObjects = world.getFighterObjects();
		if(fighterObjects.size() == 0){
			return;
		}
		
		batcher.beginBatch(Assests.item);
		
		int iLen = fighterObjects.size();
		
		for(int i=0;i<iLen;i++){
			GameObject bloodPlayerIconObject = bloodObjectManager.playerIconObjects[i];
			GameObject xuecaoObject = bloodObjectManager.xueCaoObjects[i];
			
			float playerBlood = fighterObjects.get(i).getBlood();
			GameObject xueliangObject = bloodObjectManager.getXueLiangObject(xuecaoObject, FighterObject.MAXBLOOD, playerBlood);
			
			GameObject number1Object = bloodObjectManager.number1Objects[i];
			GameObject number2Object = bloodObjectManager.number2Objects[i];
			
			//渲染图标
			batcher.drawSprite(bloodPlayerIconObject.position.x, bloodPlayerIconObject.position.y,
								bloodPlayerIconObject.bounds.width, bloodPlayerIconObject.bounds.height,
								Assests.playerIconHashMap.get(fighterObjects.get(i).getZhiye()));
			
			//渲染血槽
			batcher.drawSprite(xuecaoObject.position.x, xuecaoObject.position.y, 
								xuecaoObject.bounds.width, xuecaoObject.bounds.height, 
								Assests.xueCaoRegion);
			
			//渲染血量
			batcher.drawSprite(xueliangObject.position.x, xueliangObject.position.y, 
								xueliangObject.bounds.width, xueliangObject.bounds.height, 
								Assests.xueLiangRegion);
			
//			Log.d("SuperLineFighter", "blood:player" + i + ":" + playerBlood);
			//渲染血量值
			int iblood = new Float(playerBlood).intValue();
			if(iblood < 0){
				iblood = 0;
			}
			int number1 = (iblood)%100/10;
			int number2 = (iblood)%10;
			
			batcher.drawSprite(number1Object.position.x, number1Object.position.y, 
								number1Object.bounds.width, number1Object.bounds.height, 
								Assests.numbeRegions[number1]);
			
			batcher.drawSprite(number2Object.position.x, number2Object.position.y, 
								number2Object.bounds.width, number2Object.bounds.height, 
								Assests.numbeRegions[number2]);
			
		}
		
		
		batcher.endBatch();
		
	}
}
