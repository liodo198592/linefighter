package common.commu;

/**
 * �����ͳ����Ϣ�����Բ�ѯ����˵Ĺ���״̬
 * һ��������һ����ʵ���������ڶ�̫�������������
 * @author Administrator
 *
 */
public class ServerInfo {

	int serverID; //�ͻ���ID��Ψһ��ʾ������Serverʱȷ��
	String serverIp; //�ͻ���ip��Ψһ��ʾ
	int serverPort; //�ͻ��˶˿ڣ�������һ��IP��������
	
	enum EServerInfo_STATUS{
		EServerInfo_STATUS_CLOSE, //�������ر�
		EServerInfo_STATUS_NORMAL, //��������״̬
		EServerInfo_STATUS_DISCONNECT, //�޷����ӷ�����
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
