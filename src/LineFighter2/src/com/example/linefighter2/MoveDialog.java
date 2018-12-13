package com.example.linefighter2;

import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class MoveDialog extends Dialog{

	Context context;
	
	public Button btnButton11;
	public Button btnButton12;
	public Button btnButton13;
	public Button btnButton21;
	public Button btnButton22;
	public Button btnButton23;
	public Button btnButtonLeft;
	public Button btnButtonRight;
	
	Long fatherIndex;
	MainActivity fatherP;
	
	View.OnClickListener onClickListener;
	public MoveDialog(Context context){
		super(context);
		this.context = context;
	}
	
	public MoveDialog(Context context,View.OnClickListener onClickListener,int theme , Long fatherid,MainActivity father){
		super(context,theme);
		this.context = context;
		fatherIndex = fatherid;
		fatherP = father;
		this.onClickListener = onClickListener;
		
	}

	/* (non-Javadoc)
	 * @see android.app.Dialog#onCreate(android.os.Bundle)
	 */
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		findViews();
		setListener();
		
		if(fatherIndex.equals(2L) || fatherIndex.equals(7L)){
			disable(1);
			disable(2);
			enable(3);
		}
		else if(fatherIndex.equals(4L)){
			if(fatherP.getMyplayer().getPositionX().equals(1L)){
				enable(2);
				disable(2);
			}
			else{
				disable(2);
				enable(2);
			}
			disable(3);
		}
		

	}
	
	private void findViews(){
		
		btnButton11 = (Button)findViewById(R.id.move_dailog_11);
		btnButton12 = (Button)findViewById(R.id.move_dailog_12);
		btnButton13 = (Button)findViewById(R.id.move_dailog_13);
		btnButton21 = (Button)findViewById(R.id.move_dailog_21);
		btnButton22 = (Button)findViewById(R.id.move_dailog_22);
		btnButton23 = (Button)findViewById(R.id.move_dailog_23);
		btnButtonLeft =  (Button)findViewById(R.id.move_dailog_left);
		btnButtonRight =  (Button)findViewById(R.id.move_dailog_right);
	}

	private void setListener(){
		btnButton11.setOnClickListener(onClickListener);
		btnButton12.setOnClickListener(onClickListener);
		btnButton13.setOnClickListener(onClickListener);
		btnButton21.setOnClickListener(onClickListener);
		btnButton22.setOnClickListener(onClickListener);
		btnButton23.setOnClickListener(onClickListener);
		btnButtonLeft.setOnClickListener(onClickListener);
		btnButtonRight.setOnClickListener(onClickListener);
	}
	
	public void disable(int row){
		if(row == 1){
			btnButton11.setEnabled(false);
			btnButton12.setEnabled(false);
			btnButton13.setEnabled(false);
		}
		else if(row == 2){
			btnButton21.setEnabled(false);
			btnButton22.setEnabled(false);
			btnButton23.setEnabled(false);
		}
		else {
			btnButtonLeft.setEnabled(false);
			btnButtonRight.setEnabled(false);
		}
	}
	
	public void enable(int row){
		if(row == 1){
			btnButton11.setEnabled(true);
			btnButton12.setEnabled(true);
			btnButton13.setEnabled(true);
		}
		else if(row == 2){
			btnButton21.setEnabled(true);
			btnButton22.setEnabled(true);
			btnButton23.setEnabled(true);
		}
		else {
			btnButtonLeft.setEnabled(true);
			btnButtonRight.setEnabled(true);
		}
	}
	
}
