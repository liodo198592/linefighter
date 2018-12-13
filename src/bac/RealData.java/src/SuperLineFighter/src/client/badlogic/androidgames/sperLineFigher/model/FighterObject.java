package client.badlogic.androidgames.sperLineFigher.model;


import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;


import client.badlogic.androidgames.sperLineFigher.Const;
import client.badlogic.androidgames.sperLineFigher.control.playeraction.AvoidPlayerAction;
import client.badlogic.androidgames.sperLineFigher.control.playeraction.BiShuaPlayerAction;
import client.badlogic.androidgames.sperLineFigher.control.playeraction.DamagePlayerAction;
import client.badlogic.androidgames.sperLineFigher.control.playeraction.MovePlayerAction;
import client.badlogic.androidgames.sperLineFigher.control.playeraction.PlayerAction;
import client.badlogic.androidgames.sperLineFigher.control.playeraction.QiGongPlayerAction;
import client.badlogic.androidgames.sperLineFigher.control.playeraction.QuanPlayerAction;
import client.badlogic.androidgames.sperLineFigher.control.playeraction.ShuaiPlayerAction;
import client.badlogic.androidgames.sperLineFigher.resource.Assests.PLAYERICON;
import client.badlogic.androidgames.framework.GameObject;
import client.badlogic.androidgames.framework.math.Vector2;

public class FighterObject extends GameObject{

    public static final int playerIconWidth = 83;
    public static final int playerIconHeight = 83;
	
    public static final int MAXBLOOD = 20;
    
    MovePlayerAction movePlayerAction;
    DamagePlayerAction damagePlayerAction;
    QuanPlayerAction quanPlayerAction;
    ShuaiPlayerAction shuaiPlayerAction;
    QiGongPlayerAction qiGongPlayerAction;
    BiShuaPlayerAction biShuaPlayerAction;
    AvoidPlayerAction avoidPlayerAction;
    
    List<PlayerAction> playerActions = new ArrayList<PlayerAction>();
  //附加显示元素，如技能的名字、气功效果等
    public List<Const.GameElement> elementList = new LinkedList<Const.GameElement>();
    
	float stateTime;
	public String namString;
	private PLAYERICON zhiye = PLAYERICON.SHENGQIUYUE;

	private float blood = MAXBLOOD;

	
	public int detaiPositionIndex;//0中间  1：上  2：左下 3：右下
	
	private ActionClock actionClock = null;

	public FighterObject(String namString,float x, float y, float width, float height) {
		super(x, y, width, height);
		stateTime = 0;
		this.namString = namString;
		
		movePlayerAction = new MovePlayerAction(this.position, 3);
		playerActions.add(movePlayerAction);
		damagePlayerAction = new DamagePlayerAction(this);
		playerActions.add(damagePlayerAction);
		quanPlayerAction = new QuanPlayerAction(this);
		playerActions.add(quanPlayerAction);
		shuaiPlayerAction = new ShuaiPlayerAction(this);
		playerActions.add(shuaiPlayerAction);
		qiGongPlayerAction = new QiGongPlayerAction(this);
		playerActions.add(qiGongPlayerAction);
		biShuaPlayerAction = new BiShuaPlayerAction(this);
		playerActions.add(biShuaPlayerAction);
		avoidPlayerAction = new AvoidPlayerAction(this);
		playerActions.add(avoidPlayerAction);
	}

	public String getNameString(){
		return namString;
	}
	
	public void attack_quan(){
		quanPlayerAction.start();
	}
	
	public void attack_shuai(){
		shuaiPlayerAction.start();
	}
	
	public void attack_qigong(int direction){
		qiGongPlayerAction.setDirection(direction);
		qiGongPlayerAction.start();
	}
	
	public void attack_bisha(int direction){
		biShuaPlayerAction.setDirection(direction);
		biShuaPlayerAction.start();
	}
	
	public void move(Vector2 aimPositon){
		movePlayerAction.setAimPositon(aimPositon);
		movePlayerAction.start();
			
	}
	
	public void avoid(){
		avoidPlayerAction.start();
	}
	
	public void damageBloodTo(int blood){
		if(blood < this.blood){
			damagePlayerAction.setAimBlood(blood);
			damagePlayerAction.start();
			
		}
	}
	
	public void update(double delTime){
		
		for(PlayerAction playerAction:playerActions){
			if(!playerAction.isFinished()){
				playerAction.update(delTime);
			}
		}
	}

	public PLAYERICON getZhiye() {
		return zhiye;
	}

	public void setZhiye(PLAYERICON zhiye) {
		this.zhiye = zhiye;
	}
	
	public float getBlood() {
		return blood;
	}
	public void setBlood(float blood){
		this.blood = blood;
	}
	public boolean isAllActionOver(){

		boolean allFished = true;
		
		for(PlayerAction playerAction:playerActions){
			if(!playerAction.isFinished()){
				allFished = false;
			}
		}
		return allFished;
	}
}
