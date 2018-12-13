package common.commu;

import java.lang.reflect.Type;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;


import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;
import common.message.AbstractMessage;

public class MessageGson {

	private Map<String, List<String>> dataGsonMap = new LinkedHashMap<String, List<String>>();
	
	public void addMessage(AbstractMessage message){
		String classNameString = message.getClass().getName();
		
		if(dataGsonMap.containsKey(classNameString)){
			dataGsonMap.get(classNameString).add(new Gson().toJson(message));
		}else {
			List<String> stringList = new LinkedList<String>();
			stringList.add(new Gson().toJson(message));
			dataGsonMap.put(classNameString, stringList);
		}
	}
	
	public String toJson(){
        return new Gson().toJson(dataGsonMap);
    }
	
	public void fromJson(String taskInfoString){
        Type mapType = new TypeToken<Map<String, List<String>>>(){}.getType();
        dataGsonMap = new Gson().fromJson(taskInfoString, mapType);
    }
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public List<AbstractMessage> getMessageList(){
		List<AbstractMessage> messgaeList = new LinkedList<AbstractMessage>();
		for(Map.Entry<String, List<String>> entry:dataGsonMap.entrySet()){
			Class class1 = null;
			try {
                class1 = Class.forName(entry.getKey());
            } catch (ClassNotFoundException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
                return null;
            }
			for(String string:entry.getValue()){
				AbstractMessage abstractMessage = (AbstractMessage)new Gson().fromJson(string, class1);
				messgaeList.add(abstractMessage);
			}
		}
		
		return messgaeList;
	}
}
