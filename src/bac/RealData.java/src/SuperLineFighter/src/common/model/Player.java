package common.model;

import java.util.List;

import server.core.Show;

import common.model.ModelConst.ELIFESTAT;


import android.util.Log;

public class Player {
	
	
	int PlayerID;
	
	public int getPlayerID() {
		return PlayerID;
	}

	public void setPlayerID(int playerID) {
		PlayerID = playerID;
	}

	public Long getpX() {
		return pX;
	}

	public void setpX(Long pX) {
		this.pX = pX;
	}

	public Long getpY() {
		return pY;
	}

	public void setpY(Long pY) {
		this.pY = pY;
	}

	public Long getiHp() {
		return iHp;
	}

	public void setiHp(Long iHp) {
		this.iHp = iHp;
	}

	public Long getiMaxHp() {
		return iMaxHp;
	}

	public void setiMaxHp(Long iMaxHp) {
		this.iMaxHp = iMaxHp;
	}

	public Long getiTeam() {
		return iTeam;
	}

	public void setiTeam(Long iTeam) {
		this.iTeam = iTeam;
	}

	public ELIFESTAT geteLifeStat() {
		return eLifeStat;
	}

	public void seteLifeStat(ELIFESTAT eLifeStat) {
		this.eLifeStat = eLifeStat;
	}

	public AiInterface getAi() {
		return ai;
	}

	public void setAi(AiInterface ai) {
		this.ai = ai;
	}

	public void setName(String name) {
		this.name = name;
	}

	public Player(String strName , AiInterface ai){
		this.name = strName;
		iMaxHp = ModelConst.MAX_HP;
		iHp = iMaxHp;
		eLifeStat = ELIFESTAT.ELIFESTAT_ALIFE;
		this.ai = ai;
	}

	//�趨λ��
	public Long setPosition(Long X , Long Y)	{
		pX = X; 
		pY = Y; 
		return 0L;
	} 
	
	//��ȡλ����Ϣ
	public Long getPositionX(){

		return pX;
	} 
	public Long getPositionY(){

		return pY;
	} 
	
	public Long damageHp(Long idmgHp)
	{
		iHp -= idmgHp;
		return 0L;
	}//�޸�HP
	
	public ELIFESTAT checkLifeStat()
	{
		if(eLifeStat == ELIFESTAT.ELIFESTAT_DEAD)
		{
			return ELIFESTAT.ELIFESTAT_INVALID;
		}
		if(iHp <= 0)
		{
			eLifeStat = ELIFESTAT.ELIFESTAT_DEAD;
			return eLifeStat;
		}
		return eLifeStat;
	}
	
	
	void init()
	{
		iMaxHp = ModelConst.MAX_HP;
		iHp = 	iMaxHp;
	}

	public String getName(){
		return name;
	}
	
	public Long getHp(){
		return iHp;
	}
	
	public ELIFESTAT getLifeStat(){
		return eLifeStat;
	}
	
	public List<Action> getAction(Show sw){
		String name =  this.getName();
		if(name.compareTo("PLAYER") == 0)
		{
			Log.d("LineFighter", "getAction :: getActionFromConsole��");
			return sw.getActionFromConsole(this);
		}
		else
		{
			Log.d("LineFighter", "getAction :: ai.getAction(this)��");
			return ai.getAction(this);
			
		}
	}

	
	private String name;
	private Long pX; //  X�� 1 �� 2 ��ʾ A �� B 
	private Long pY; //  1 �� 2�� 3   
	private Long iHp;// Ѫ��
	private Long iMaxHp;// Ѫ��
	private Long iTeam; //�����֣�����һ����Ϊͬһ����
	private ELIFESTAT eLifeStat; //1 ��ʾ��� 2 ��ʾ����
	private AiInterface ai; //����AI
}
