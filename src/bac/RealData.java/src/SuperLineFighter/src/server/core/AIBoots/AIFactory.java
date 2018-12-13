package server.core.AIBoots;

import common.model.AiInterface;

/**
 * ����AI�Ĺ��� , ��������
 * @author Administrator
 *
 */
public class AIFactory {

	static AIFactory aif = null;
	
	private AIFactory(){
		
	}
	
	public static AIFactory getInstance(){
		if(aif == null){
			aif = new AIFactory();
			return aif;
		}
		else{
			return aif;
		}
	}
	
	public AiInterface createAI(String aiName){
		if(aiName.equals("LFAI")){
			return (AiInterface) (new LFAI());
		}
		else{
			return (AiInterface) (new LFAILV0());
		}
	}
}
