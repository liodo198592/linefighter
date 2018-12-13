package common.commu;

/**
 * 记录客户端信息类，每个客户端一个实例
 * 通过唯一ID标示，由服务端生成后向客户端发送
 * @author Administrator
 *
 */

public class ClientInfo {
	
	enum EClientInfo_STATUS{
		EClientInfo_STATUS_UNDEFINED, //刚连接，未分配ID时
		EClientInfo_STATUS_CONNECTING, //正常连接状态
		EClientInfo_STATUS_WAITTING, //服务端正在等待此Client重新连接
		
	}

	int clientId; //客户端ID，唯一标示
	String clientIp; //客户端ip，唯一标示
	int clientPort; //客户端端口，适用于一个IP开多个软件
	
	
	public int getClientId() {
		return clientId;
	}
	public void setClientId(int clientId) {
		this.clientId = clientId;
	}
	public String getClientIp() {
		return clientIp;
	}
	public void setClientIp(String clientIp) {
		this.clientIp = clientIp;
	}
	public int getClientPort() {
		return clientPort;
	}
	public void setClientPort(int clientPort) {
		this.clientPort = clientPort;
	}
	
	//客户端句柄
}
