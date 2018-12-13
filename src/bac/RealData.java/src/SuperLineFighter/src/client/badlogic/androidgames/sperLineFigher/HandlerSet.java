package client.badlogic.androidgames.sperLineFigher;

import java.util.ArrayList;
import java.util.List;

import client.badlogic.androidgames.sperLineFigher.control.World;

import common.model.Action;


//通信消息队列，采用GSON以后可废弃
public class HandlerSet{
		
		List<Action> ActionListRet; 
		World world;
		public HandlerSet(World world) {
			this.world = world;
			ActionListRet = new ArrayList<Action>();
		}
		
		public List<Action> getActionListRet() {
			return ActionListRet;
		}

		public void setActionListRet(List<Action> ActionListRet) {
			this.ActionListRet = ActionListRet;
		}
		
		
		
	}
