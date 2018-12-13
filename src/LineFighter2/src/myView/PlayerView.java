package myView;

import LineFighter.Core.Player;
import android.R.integer;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.RectF;
import android.util.AttributeSet;
import android.view.View;
import android.webkit.PluginStub;

import java.util.List;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Map;

public class PlayerView extends View {

	List<Player> m_players;
	
	public PlayerView(Context context,AttributeSet attributeSet){
		super(context,attributeSet);
		m_players = new ArrayList<Player>();
		
		Player  p1 = new Player("cyt");
		p1.setPosition(1L,1L);
		Player  p2 = new Player("zhouyu");
		p2.setPosition(2L,1L);
		Player  p3 = new Player("jinwei");
		p3.setPosition(1L,2L);
		Player  p4 = new Player("TP");
		p4.setPosition(2L,2L);
		
		m_players.add(p1);
		m_players.add(p2);
		m_players.add(p3);
		m_players.add(p4);
		
		
	}
	public void setPlayer(List<Player> players){
		
		m_players.clear();
		for(Player pl:players){
			m_players.add(pl);
		}
	}
	/* (non-Javadoc)
	 * @see android.view.View#onDraw(android.graphics.Canvas)
	 */
	@Override
	protected void onDraw(Canvas canvas) {
		// TODO Auto-generated method stub
		super.onDraw(canvas);
		
		
		//canvas.drawColor(Color.BLUE);
		Paint paint = new Paint();
		paint.setColor(Color.BLUE);
		
		//canvas.drawLine(10, 10, 100, 10, paint);
		
		
		int iWidth = this.getWidth();
		int iHeight = this.getHeight();
		canvas.drawLine((float)iWidth/10, (float)iHeight/5, (float)iWidth*9/10, (float)iHeight/5, paint);
		canvas.drawLine((float)iWidth/10, (float)iHeight*2/3, (float)iWidth*9/10, (float)iHeight*2/3, paint);
		
		RectF rf11 = new RectF((float)iWidth/10-5,(float)iHeight/5-5,(float)iWidth/10+5,(float)iHeight/5+5);
		canvas.drawOval(rf11, paint);
		
		RectF rf12 = new RectF( (float)iWidth*5/10-5,(float)iHeight/5-5,(float)iWidth*5/10+5,(float)iHeight/5+5);
		canvas.drawOval(rf12, paint);
		
		RectF rf13 = new RectF((float)iWidth*9/10-5,(float)iHeight/5-5,(float)iWidth*9/10+5,(float)iHeight/5+5);
		canvas.drawOval(rf13, paint);
		
		RectF rf21 = new RectF((float)iWidth/10-5,(float)iHeight*2/3-5,(float)iWidth/10+5,(float)iHeight*2/3+5);
		canvas.drawOval(rf21, paint);
		
		RectF rf22 = new RectF((float)iWidth*5/10-5,(float)iHeight*2/3-5,(float)iWidth*5/10+5,(float)iHeight*2/3+5);
		canvas.drawOval(rf22, paint);
		
		RectF rf23 = new RectF((float)iWidth*9/10-5,(float)iHeight*2/3-5,(float)iWidth*9/10+5,(float)iHeight*2/3+5);
		canvas.drawOval(rf23, paint);
		
		drawPlayers(canvas, paint);
		

		
	}
	

	void drawPlayers(Canvas canvas,Paint paint){
		
		int iWidth = this.getWidth();
		int iHeight = this.getHeight();
		
		int[][] drawX = new int[2][3];
		int[][] drawY = new int[2][3];
		
		drawX[0][0] = iWidth/10 -10;
		drawY[0][0] = iHeight/5 -10;
		
		drawX[0][1] = iWidth/2 -10;
		drawY[0][1] = iHeight/5 -10;
		
		drawX[0][2] = iWidth*9/10 -10;
		drawY[0][2] = iHeight/5 -10;
		
		drawX[1][0] = iWidth/10 -10;
		drawY[1][0] = iHeight*2/3 -10;
		
		drawX[1][1] = iWidth/2 -10;
		drawY[1][1] = iHeight*2/3 -10;
		
		drawX[1][2] = iWidth*9/10 -10;
		drawY[1][2] = iHeight*2/3 -10;
		
		
		
		
		
		for(Player pl:m_players){
			
			if(pl.getHp() <= 0){
				continue;
			}
			
			Long x = new Long(0);
			Long y = new Long(0);
			x = pl.getPositionX();
			y = pl.getPositionY();
			
			if(x != 0 && y != 0){
				String strName = pl.getName() + " HP:" + pl.getHp();
				canvas.drawText(strName, drawX[x.intValue() - 1][y.intValue() -1], drawY[x.intValue() -1][y.intValue() -1], paint);
				
				drawY[x.intValue() -1 ][y.intValue() -1] -= 20;
			}
			
			
		}
	}
	
	public void refreshPlayer(List<Player> players){
		
	}
	
}
