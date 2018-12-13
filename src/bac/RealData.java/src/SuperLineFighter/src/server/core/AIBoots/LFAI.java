package server.core.AIBoots;

import java.util.ArrayList;
import java.util.List;

import common.model.Action;
import common.model.AiInterface;
import common.model.ModelConst.EACTION;
import common.model.Player;
import common.model.ModelConst;

public class LFAI implements AiInterface{
	public List<Action> getAction(Player  player){
		//随机选择3部的动作
		Long Rate = 0L;
		Long frandom = 0L;
		Long X = 0L, Y = 0L;
		X = player.getPositionX();
		Y = player.getPositionY();
		EACTION eType;
		List<Action> list = new ArrayList<Action>();

		for(Long i = 0L ; i < 3L; ++i)
		{
			if(player.getHp() < 7)
			{
				Rate = 7L;
			}
			else
			{
				Rate = 6L;
			}
			
			frandom = (long) (Math.random() *  Rate) + 1;
			eType = ModelConst.getEaction(frandom);
			Action  p = new Action(eType);
			if(eType == EACTION.EACTION_Move)
			{
				p.setDestX(X);
				p.setDestY((long) (Math.random() *  3) + 1);
			}
			else if(eType == EACTION.EACTION_Air || eType == EACTION.EACTION_Kill)
			{
				p.setDerection((long) (Math.random() *  2) + 1);
			}
			else if(eType == EACTION.EACTION_ChangeLine)
			{
				if(X == 1L) X = 2L;
				else X = 1L;
			}
			list.add(p);
		}
		return list;
	 }
	
}
