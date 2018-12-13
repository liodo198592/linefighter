package com.badlogic.androidgames.SperLineFigher;

import java.util.ArrayList;
import java.util.List;

import LineFighter.Core.Action;


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
