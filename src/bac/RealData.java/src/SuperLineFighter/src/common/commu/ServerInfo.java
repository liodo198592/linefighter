package common.commu;

/**
 * 服务端统计信息，用以查询服务端的工作状态
 * 一个服务器一个此实例，适用于多太服务主机的情况
 * @author Administrator
 *
 */
public class ServerInfo {

	int serverID; //客户端ID，唯一标示，启动Server时确定
	String serverIp; //客户端ip，唯一标示
	int serverPort; //客户端端口，适用于一个IP开多个软件
	
	enum EServerInfo_STATUS{
		EServerInfo_STATUS_CLOSE, //服务器关闭
		EServerInfo_STATUS_NORMAL, //正常连接状态
		EServerInfo_STATUS_DISCONNECT, //无法连接服务器
	}

	public int getServerID() {
		return serverID;
	}

	public void setServerID(int serverID) {
		this.serverID = serverID;
	}

	public String getServerIp() {
		return serverIp;
	}

	public void setServerIp(String serverIp) {
		this.serverIp = serverIp;
	}

	public int getServerPort() {
		return serverPort;
	}

	public void setServerPort(int serverPort) {
		this.serverPort = serverPort;
	}
}
