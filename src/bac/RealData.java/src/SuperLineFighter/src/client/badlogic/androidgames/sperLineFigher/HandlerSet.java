package client.badlogic.androidgames.sperLineFigher;

import java.util.ArrayList;
import java.util.List;

import client.badlogic.androidgames.sperLineFigher.control.World;

import common.model.Action;


//ͨ����Ϣ���У�����GSON�Ժ�ɷ���
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
