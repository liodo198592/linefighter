package common.model;

//User与PLAYER不同,包含登陆的用户信息，非具体的Player , Player 是 服务器根据User信息创建出来的
public class LFUser {
	String userName = "unknow"; 
	int userID = -1 ; //用户唯一ID,联机版在登陆以后由服务器返回
	int roomID = -1 ; //用户在哪个房间，-1 表示未在任何房间
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
