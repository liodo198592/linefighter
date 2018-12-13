package server.core.AIBoots;

import java.util.ArrayList;
import java.util.List;

import common.model.Action;
import common.model.AiInterface;
import common.model.ModelConst;
import common.model.Player;

//2B Ai 教学用，只会站立
public class LFAILV0 implements AiInterface{
	public List<Action> getAction(Player  player){
		List<Action> list = new ArrayList<Action>();
		for(int i = 1 ; i <= ModelConst.MAX_ACTNUM; ++i){
			Action  p = new Action(ModelConst.EACTION.EACTION_Invalid);
			p.setDestX(player.getPositionX());
			p.setDestY(player.getPositionY());
			list.add(p);
		}
		return list;
	 }
}
