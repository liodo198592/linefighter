package LineFighter.Core;

import java.util.List;

import android.util.Log;

import LineFighter.Core.Const.ELIFESTAT;

public class Player {
	
	
	public Player(String strName){
		this.name = strName;
		iMaxHp = Const.MAX_HP;
		iHp = iMaxHp;
		eLifeStat = ELIFESTAT.ELIFESTAT_ALIFE;
		ai = new LFAI();
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
	
	Long damageHp(Long idmgHp)
	{
		iHp -= idmgHp;
		return 0L;
	}//�޸�HP
	
	ELIFESTAT checkLifeStat()
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
		iMaxHp = Const.MAX_HP;
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
	
	List<Action> getAction(Show sw){
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
	private LFAI ai; //����AI
}
