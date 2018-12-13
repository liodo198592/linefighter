package com.example.linefighter2;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

//import com.ant.liao.GifView;
//import com.ant.liao.GifView.GifImageType;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.app.Activity;
//import android.graphics.Canvas;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
//import android.widget.EditText;
import android.widget.TextView;
import android.view.View.OnClickListener;
import myView.PlayerView;
import LineFighter.Core.*;
//import android.text.StaticLayout;
import android.text.method.ScrollingMovementMethod;
//import myDialog.*;
public class MainActivity extends Activity {

	PlayerView m_playerView;
	TextView m_textShow;
	TextView m_commandShow;
	
	Button m_btnHand;
	Button m_btnShanbi;
	Button m_btnMove;
	Button m_btnChangeline;
	Button m_btnDefence;
	Button m_btnShuai;
	Button m_btnQigong;
	Button m_btnBisha;
	
	Button m_btnConfirm;
	Button m_btnCancle;
	
	TextView m_infoText;
	
	ThreadTest m_ThreadTest;
	HandlerSet m_handleSet;
	Engine m_engine;
	
	MoveDialog moveDialog;
	
	Map<Long,Action> inputAction;
	Player myplayer;
	//此次按下的是哪个按钮
	Long thisBtnIndex;
	
	
	protected void disableBtn() {
		m_btnHand.setEnabled(false);
		m_btnShanbi.setEnabled(false);
		m_btnMove.setEnabled(false);
		m_btnChangeline.setEnabled(false);
		m_btnDefence.setEnabled(false);
		m_btnShuai.setEnabled(false);
		m_btnQigong.setEnabled(false);
		m_btnBisha.setEnabled(false);
		m_btnConfirm.setEnabled(false);
		m_btnCancle.setEnabled(false);
	}
	
	protected void enableBtn() {
		m_btnHand.setEnabled(true);
		m_btnShanbi.setEnabled(true);
		m_btnMove.setEnabled(true);
		m_btnChangeline.setEnabled(true);
		m_btnDefence.setEnabled(true);
		m_btnShuai.setEnabled(true);
		m_btnQigong.setEnabled(true);
		m_btnBisha.setEnabled(true);
		m_btnConfirm.setEnabled(true);
		m_btnCancle.setEnabled(true);
	}
	
	
	int m_scrsize;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		
		inputAction = new LinkedHashMap<Long,Action>();
		thisBtnIndex = new Long(0);
		
		setContentView(R.layout.activity_main);
		
		Log.d("LineFighter", "onCreateStart！");
		
		findViews();
		setListener();
		
		//创建一个myhandler实例，传入此UI的主线程loop
		m_handleSet = new HandlerSet();
		
		disableBtn();
		m_infoText.append("please wait => ~~~~(>_<)~~~~ ");
		
		m_engine = new Engine(m_handleSet);
		new Thread(m_engine).start();
		
			
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	private void findViews(){
		m_playerView = (PlayerView)findViewById(R.id.PlayerView);
		m_infoText = (TextView)findViewById(R.id.infoshow);
		m_infoText.setMovementMethod(ScrollingMovementMethod.getInstance()); 
		m_textShow = (TextView)findViewById(R.id.textshow);
		m_textShow.setMovementMethod(ScrollingMovementMethod.getInstance()); 
		m_commandShow = (TextView)findViewById(R.id.commandshow);
		m_commandShow.setMovementMethod(ScrollingMovementMethod.getInstance()); 
		m_btnHand = (Button)findViewById(R.id.hand);
		m_btnShanbi = (Button)findViewById(R.id.shanbi);
		m_btnMove = (Button)findViewById(R.id.move);
		m_btnChangeline = (Button)findViewById(R.id.changeline);
		m_btnDefence = (Button)findViewById(R.id.defence);
		m_btnShuai = (Button)findViewById(R.id.shuai);
		m_btnQigong = (Button)findViewById(R.id.qigong);
		m_btnBisha = (Button)findViewById(R.id.bisha);
		m_btnConfirm =  (Button)findViewById(R.id.execute);
		m_btnCancle =  (Button)findViewById(R.id.cancle);
	}
	
	
	private void setListener(){
		m_btnHand.setOnClickListener(on_click);
		m_btnShanbi.setOnClickListener(on_click);
		m_btnMove.setOnClickListener(on_click);
		m_btnChangeline.setOnClickListener(on_click);
		m_btnDefence.setOnClickListener(on_click);
		m_btnShuai.setOnClickListener(on_click);
		m_btnQigong.setOnClickListener(on_click);
		m_btnBisha.setOnClickListener(on_click);
		m_btnConfirm.setOnClickListener(on_click);
		m_btnCancle.setOnClickListener(on_click);
		//m_textShow.setKeyListener(null);//使显示框只读
	}
	
	private OnClickListener on_click = new OnClickListener(){
		public void onClick(View v){
			Button btnClick = (Button)v;
			
			if(btnClick == m_btnHand){
				on_clickBtnHand();
			}else if(btnClick == m_btnShanbi){
				on_clickBtnShanbi();
			}else if(btnClick == m_btnMove){
				on_clickBtnMove();
			}else if(btnClick == m_btnChangeline){
				on_clickBtnChangeline();
			}else if(btnClick == m_btnDefence){
				on_clickBtnDefence();
			}else if(btnClick == m_btnShuai){
				on_clickBtnShuai();
			}else if(btnClick == m_btnQigong){
				on_clickBtnQigong();
			}else if(btnClick == m_btnBisha){
				on_clickBtnBisha();
			}else if(btnClick == m_btnConfirm){
				on_clickBtnConfirm();
			}else if(btnClick == m_btnCancle){
				on_clickBtnCancle();
			}
			
			
			
		}
	};
	
	
	public void showCommand(){
		String strValue = new String("");
		m_commandShow.clearComposingText();
		Iterator<Long> it = inputAction.keySet().iterator();
		
		while(it.hasNext()){
				Long index = it.next();
				strValue += " [" + index + inputAction.get(index).getDescription() + "]";
		}
		m_commandShow.setText(strValue);
	}

	private void on_clickBtnConfirm(){
		if(inputAction.size() != 3){
			return;
		}
		List<Action> lst = new ArrayList<Action>();
		Iterator<Long> it = inputAction.keySet().iterator();
		while(it.hasNext()){
				Long index = it.next();
				lst.add(inputAction.get(index));
		}
		m_handleSet.getActionListRet().addAll(lst);
		showCommand();
	}
	
	private void on_clickBtnCancle(){
		if(inputAction.isEmpty()){
			return;
		}
		Long index = new Long(inputAction.size());
		inputAction.remove(index);
		showCommand();
	}
	
	private void on_clickBtnHand(){
		if(inputAction.size() >= 3){
			return;
		}
		Action act = new Action(Const.getEaction(1L));
		Long id = new Long(inputAction.size() + 1);
		inputAction.put(id , act);
		showCommand();
	}
	private void on_clickBtnShanbi(){
		if(inputAction.size() >= 3){
			return;
		}
		Action act = new Action(Const.getEaction(6L));
		Long id = new Long(inputAction.size() + 1);
		inputAction.put(id , act);
		showCommand();
	}
	private void on_clickBtnMove(){
		
		if(inputAction.size() >= 3){
			return;
		}
		
		thisBtnIndex = 4L;
		moveDialog = new MoveDialog(MainActivity.this,moveClickListener,R.style.MoveDialog,thisBtnIndex,this);
		moveDialog.setContentView(R.layout.move_dialog);
		moveDialog.show();
		
	}
	private void on_clickBtnChangeline(){
		if(inputAction.size() >= 3){
			return;
		}
		Action act = new Action(Const.getEaction(5L));
		Long id = new Long(inputAction.size() + 1);
		inputAction.put(id , act);
		showCommand();
	}
	private void on_clickBtnDefence(){
		//暂时不支持
		return;
	}
	private void on_clickBtnShuai(){
		if(inputAction.size() >= 3){
			return;
		}
		Action act = new Action(Const.getEaction(3L));
		Long id = new Long(inputAction.size() + 1);
		inputAction.put(id , act);
		showCommand();
	}
	private void on_clickBtnQigong(){
		if(inputAction.size() >= 3){
			return;
		}
		
		thisBtnIndex = 2L;
		moveDialog = new MoveDialog(MainActivity.this,moveClickListener,R.style.MoveDialog,thisBtnIndex,this);
		moveDialog.setContentView(R.layout.move_dialog);
		moveDialog.show();
		
	}
	private void on_clickBtnBisha(){
		if(inputAction.size() >= 3){
			return;
		}
		
		thisBtnIndex = 7L;
		moveDialog = new MoveDialog(MainActivity.this,moveClickListener,R.style.MoveDialog,thisBtnIndex,this);
		moveDialog.setContentView(R.layout.move_dialog);
		moveDialog.show();
	}
	
	private View.OnClickListener moveClickListener = new View.OnClickListener() {
		
		public void onClick(View v) {
			// TODO Auto-generated method stub
			Button button = (Button)v;
			
			Action myAction = new Action(Const.getEaction(thisBtnIndex));
			
			if(button == moveDialog.btnButton11){
				myAction.setDestX(1L);
				myAction.setDestY(1L);
			}else if(button == moveDialog.btnButton12){
				myAction.setDestX(1L);
				myAction.setDestY(2L);
			}else if(button == moveDialog.btnButton13){
				myAction.setDestX(1L);
				myAction.setDestY(3L);
			}else if(button == moveDialog.btnButton21){
				myAction.setDestX(2L);
				myAction.setDestY(1L);
			}else if(button == moveDialog.btnButton22){
				myAction.setDestX(2L);
				myAction.setDestY(2L);
			}else if(button == moveDialog.btnButton23){
				myAction.setDestX(2L);
				myAction.setDestY(3L);
			}
			else if(button == moveDialog.btnButtonLeft){
				myAction.setDerection(1L);
			}
			else if(button == moveDialog.btnButtonRight){
				myAction.setDerection(2L);
			}
			
			Long id = new Long(inputAction.size() + 1);
			inputAction.put(id , myAction);
			
			if(moveDialog != null){
				moveDialog.cancel();
				moveDialog = null;
			}
			showCommand();
		}
	};
	
	public class PlayerShowHandler extends Handler{
		public PlayerShowHandler(){
			
		}
		public PlayerShowHandler(Looper L){
			super(L);
		}
		public void handleMessage(Message msg) {
			
			Bundle bundle = msg.getData();
			List<Player> players = (List<Player>)bundle.getSerializable("players");
			myplayer = players.get(0);
			m_playerView.setPlayer(players);
			m_playerView.invalidate();
		}
	}
	//创建myhandler类
	public class TextShowHandler extends Handler{
		
		Map<Long,String> detailMap;
		
		public TextShowHandler(){
			detailMap = new LinkedHashMap<Long,String>();
		}
		public TextShowHandler(Looper L){
			super(L);
			detailMap = new LinkedHashMap<Long,String>();
		}
		/* (non-Javadoc)
		 * @see android.os.Handler#handleMessage(android.os.Message)
		 */
		@Override//当调用myhandler的sendMessage方法时，此方法得到响应
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			//super.handleMessage(msg);
			Bundle b = msg.getData();
			String totalValue = new String("");
			Long detailRow = new Long(0);
			
			Log.d("MyView", "start");
			String strValue = b.getString("value");
			
			if(detailMap.isEmpty()){
				detailMap.put(1L, strValue);
			}
			else if(detailMap.size() < Const.SHOW_LINE_NUM){
				//获取最大的KEY
				Set<Long> set = detailMap.keySet();
				Long key = (Long)Collections.max(set) + 1;
				detailMap.put(key, strValue);
			}
			else{
				Long minkey = Collections.min(detailMap.keySet());
				detailMap.remove(minkey);
				Long key = Collections.max(detailMap.keySet()) + 1;
				detailMap.put(key, strValue);
			}
			
			Iterator<Long> itset = (Iterator<Long>) detailMap.keySet().iterator();
			while(itset.hasNext()){
				Long subkey = itset.next();
				totalValue += subkey.toString() + " " +  detailMap.get(subkey) + "\n";
				int len = detailMap.get(subkey).length();
				detailRow += len / Const.SHOW_COL_NUM + 1;
			}
			
			Log.d("MyView", "detailRow = " + detailRow.toString() + " totalValue= " + totalValue);
			
			MainActivity.this.m_textShow.setText(totalValue);
			
			if(detailRow > Const.SHOW_BOX_ROW){
				
				MainActivity.this.m_textShow.scrollTo(0, (detailRow.intValue()-Const.SHOW_BOX_ROW.intValue())
						* Const.SHOW_ROW_LEN.intValue());
			}
		}
	}
	
	public class InputHandler extends Handler{
		public InputHandler(){
			
		}
		public InputHandler(Looper L){
			super(L);
		}
		/* (non-Javadoc)
		 * @see android.os.Handler#handleMessage(android.os.Message)
		 */
		@Override//当调用myhandler的sendMessage方法时，此方法得到响应
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			//super.handleMessage(msg);
			Bundle b = msg.getData();
			String strValue = b.getString("input");
			if(strValue == "command"){
				Log.d("LineFighter-Show", "command");
				//获取用户命令
				enableBtn();
				m_infoText.setText("");
				m_infoText.append("please input action =====> ^_^");
			}
			
			if(strValue == "clean"){
				Log.d("LineFighter-Show", "clean");
				//获取用户命令
				disableBtn();
				m_infoText.setText("");
				m_infoText.append("please input action =====> ^_^");
				inputAction.clear();
				m_handleSet.getActionListRet().clear();
				showCommand();
			}
		}
	}
	
	public class DialogBtnHandler extends Handler{
		public DialogBtnHandler(Looper L){
			super(L);
		}


		/* (non-Javadoc)
		 * @see android.os.Handler#handleMessage(android.os.Message)
		 */
		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			
			Bundle b = msg.getData();
			
			if(b.containsKey("moveDialog")){
				handleMoveDialogBtn(msg);
			}
		}
	}
	
	private void handleMoveDialogBtn(Message msg){
		Bundle b = msg.getData();
		
		int i = (int)b.getInt("moveDialog");
		moveDialog.cancel();
		
	}
	public class HandlerSet{
		TextShowHandler myHandler;
		PlayerShowHandler playerShowHandler;
		InputHandler m_inputHandler;
		
		List<Action> ActionListRet; 
		
		public List<Action> getActionListRet() {
			return ActionListRet;
		}

		public void setActionListRet(List<Action> ActionListRet) {
			this.ActionListRet = ActionListRet;
		}

		public InputHandler getM_inputHandler() {
			return m_inputHandler;
		}

		public void setM_inputHandler(InputHandler m_inputHandler) {
			this.m_inputHandler = m_inputHandler;
		}

		public HandlerSet() {
			myHandler = new TextShowHandler(MainActivity.this.getMainLooper());
			playerShowHandler = new PlayerShowHandler(MainActivity.this.getMainLooper());
			m_inputHandler =  new InputHandler(MainActivity.this.getMainLooper());
			ActionListRet = new ArrayList<Action>();
		}
		
		public TextShowHandler getMyHandler() {
			return myHandler;
		}
		public void setMyHandler(TextShowHandler myHandler) {
			this.myHandler = myHandler;
		}
		public PlayerShowHandler getPlayerShowHandler() {
			return playerShowHandler;
		}
		public void setPlayerShowHandler(PlayerShowHandler playerShowHandler) {
			this.playerShowHandler = playerShowHandler;
		}	
	}
	
	public Player getMyplayer() {
		return myplayer;
	}

	public void setMyplayer(Player myplayer) {
		this.myplayer = myplayer;
	}
}
