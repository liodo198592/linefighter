package server.core;

import common.model.Action;
import common.model.ModelConst.EACTION;


public class DemoAction extends Action{
	
	public DemoAction(EACTION eType){
		super(eType);
		this.etype = eType;
		demage = 0L;
		range = -1L;
		derection = -1L;
		destX = 0L;
		destY = 0L;
		switch(eType)
		{
			case EACTION_Hand:
				{
					init(2L,1L);
					break;
				}
			case EACTION_Air:
				{
					init(1L,2L);

					break;
				}
			case EACTION_Fall:
				{
					init(2L,1L);
					break;
				}
			case EACTION_Move:
				{
					init(0L,0L);
					break;
				}
			case EACTION_ChangeLine:
			case EACTION_Avoid:
				{
					break;
				}
			case EACTION_Kill:
				{
					init(5L,2L);
					break;
				}
			case EACTION_Invalid:
				{
					init(1L,2L);
					break;
				}
			default:
				break;
		}
	}
	
	public void init(Long idemage , Long irange){
		demage = idemage;
		range = irange;
	}
	
	//返回伤对方多少血
	public Long damageto(Action act){
		
		if(etype == EACTION.EACTION_Hand || etype == EACTION.EACTION_Air || etype == EACTION.EACTION_Kill)
		{
			if(act.getType() == EACTION.EACTION_Avoid)
			{
				return 0L;
			}
			else return demage;
		}
		if(etype == EACTION.EACTION_Fall)
		{
			if(act.getType() == EACTION.EACTION_Avoid)
			{
				return demage * 2L;
			}
			else return -1L;
		}
		return 0L; 
	}
	
	public Long getDemage() {
		return demage;
	}

	public void setDemage(Long demage) {
		this.demage = demage;
	}

	public Long getRange() {
		return range;
	}

	public void setRange(Long range) {
		this.range = range;
	}

	public Long getDerection() {
		return derection;
	}

	public void setDerection(Long derection) {
		this.derection = derection;
	}

	public Long getDestX() {
		return destX;
	}

	public void setDestX(Long destX) {
		this.destX = destX;
	}

	public Long getDestY() {
		return destY;
	}

	public void setDestY(Long destY) {
		this.destY = destY;
	}

	public EACTION getEtype() {
		return etype;
	}

	public void setEtype(EACTION etype) {
		this.etype = etype;
	}

	//是否能够攻击到对方, > 0 表示 能攻击到，-1 表示不能攻击到, 数字越小击中的可能性越大
	public Long rangeto(Long selfX, Long selfY , Long youX, Long youY){
		
		if(     etype == EACTION.EACTION_Move ||
				etype == EACTION.EACTION_ChangeLine ||
				etype == EACTION.EACTION_Avoid || etype == EACTION.EACTION_Invalid)
			{
				return -1L;
			}

			if (range == 1)
			{
				if(selfX == youX && selfY == youY) return 0L;
				else return -1L;
			}
			else if(range == 2L)
			{
				if(selfX != youX) return -1L;
				else if(derection == 1L)
				{
					if(selfY >= youY) return (youY-selfY);
					else return -1L;
				}
				else if(derection == 2L)
				{
					if(selfY <= youY) return (selfY - youY);
					else return -1L;
				}
			}
			return -1L;
	}
	
	public Long movetoX(Long oldX){
		
		Long newX = oldX;
		if(etype == EACTION.EACTION_Move)
		{
			newX = oldX;
		}
		else if(etype == EACTION.EACTION_ChangeLine)
		{
			if(oldX == 1) newX = 2L;
			else newX = 1L;

		}
		else
		{
			newX = oldX;
		}
		return newX; 
	}
	
	public Long movetoY(Long oldY){
		
		Long newY = oldY;
		if(etype == EACTION.EACTION_Move)
		{
			newY = destY;
		}
		else if(etype == EACTION.EACTION_ChangeLine)
		{
			newY = oldY;
		}
		else
		{
			newY = oldY;
		}
		return newY; 
	}
	
	public EACTION getType(){
		return etype;
	}
	
	public String getDescription(){
		switch(etype)
		{
			case EACTION_Hand:
				{
					return "拳";
				}
			case EACTION_Air:
				{
					if(derection == 1)
					{
						return "向左气功";
					}
					else
					{
						return "向右气功";
					}
				}
			case EACTION_Fall:
				{
					return "摔";
				}
			case EACTION_Move:
				{
					String temp;
					temp = "移动到 ("+destX + "," + destY + ")";
					return temp;
				}
			case EACTION_ChangeLine:
				{
					return "换线";
				}
			case EACTION_Avoid:
				{
					return "回避";
				}
			case EACTION_Kill:
				{
					if(derection == 1)
					{
						return "向左必杀";
					}
					else
					{
						return "向右必杀";
					}
				}
			case EACTION_Invalid:
				{
					return "待机";
				}
			default:
				break;
		}
		return "";
	}
}
