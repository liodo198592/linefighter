package common.model;

//User��PLAYER��ͬ,������½���û���Ϣ���Ǿ����Player , Player �� ����������User��Ϣ����������
public class LFUser {
	String userName = "unknow"; 
	int userID = -1 ; //�û�ΨһID,�������ڵ�½�Ժ��ɷ���������
	int roomID = -1 ; //�û����ĸ����䣬-1 ��ʾδ���κη���
	public int getRoomID() {
		return roomID;
	}

	public void setRoomID(int roomID) {
		this.roomID = roomID;
	}

	LFUser(String userName){
		this.userName = userName;
	}
	
	public String getUserName() {
		return userName;
	}
	public void setUserName(String userName) {
		this.userName = userName;
	}
	public int getUserID() {
		return userID;
	}
	public void setUserID(int userID) {
		this.userID = userID;
	}
	
}
