package common.model;

/**
 * ������࣬�߱�����Ļ�����Ϣ
 * @author Administrator
 *
 */
public class Room {	
	
  public static enum EROOM_STATUS{
	  //�´���
	  EROOM_NEW ,
      EROOM_WAIT,	  
      EROOM_GAMMING,
      //���٣������յķ���
      EROOM_DESTROY
  }
  
  int roomId;
  
  int roundNum;
  
  public int getRoundNum() {
	return roundNum;
}

public void setRoundNum(int roundNum) {
	this.roundNum = roundNum;
}

EROOM_STATUS status;
  //��������
  String roomName;
  //��ǰ����
  int playerNumer;
  //�������
  int maxNumer;
  //Status
  
  public Room(int roomId){
	  this.roomId = roomId;
	  roundNum = 1;
	  status = EROOM_STATUS.EROOM_NEW;
  }

	public int getRoomId() {
		return roomId;
	}
	
	public void setRoomId(int roomId) {
		this.roomId = roomId;
	}
	
	public EROOM_STATUS getStatus() {
		return status;
	}
	
	public void setStatus(EROOM_STATUS status) {
		this.status = status;
	}
	
	public String getRoomName() {
		return roomName;
	}
	
	public void setRoomName(String roomName) {
		this.roomName = roomName;
	}
	
	public int getPlayerNumer() {
		return playerNumer;
	}
	
	public void setPlayerNumer(int playerNumer) {
		this.playerNumer = playerNumer;
	}
	
	public int getMaxNumer() {
		return maxNumer;
	}
	
	public void setMaxNumer(int maxNumer) {
		this.maxNumer = maxNumer;
	}
}
