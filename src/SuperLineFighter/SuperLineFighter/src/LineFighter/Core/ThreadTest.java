package LineFighter.Core;
//import com.example.linefighter2.MainActivity.TextShowHandler;

import android.R.integer;
import android.widget.EditText;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;

public class ThreadTest implements Runnable {

//	TextShowHandler myHandler;
//	public ThreadTest(TextShowHandler mh){
//		myHandler = mh;
//	}
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
