package server.core;

import android.os.Bundle;

public class ThreadTest implements Runnable {

	public void run() {
		// TODO Auto-generated method stub

		int i = 0;
		
		try{
			while(true){
				//Message msg = myHandler.obtainMessage();
				
				Bundle b = new Bundle();
				String strValue = "this is " + i;
				b.putString("value", strValue);
				//msg.setData(b);
				//myHandler.sendMessage(msg);
				i++;
				Thread.sleep(1000);
			}
		}catch(InterruptedException e){
			e.printStackTrace();
		}

		
	}

}
