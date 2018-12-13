package common.message;

public abstract class AbstractMessage {

	private int sessionID;
	private String requestName;

	
	public String getRequestName() {
		return requestName;
	}

	public void setRequestName(String requestName) {
		this.requestName = requestName;
	}

	public int getSessionID() {
		return sessionID;
	}
	
	public void setSessionID(int sessionID) {
		this.sessionID = sessionID;
	}
}
