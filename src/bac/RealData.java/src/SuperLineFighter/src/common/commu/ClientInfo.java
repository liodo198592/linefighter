package common.commu;

/**
 * ��¼�ͻ�����Ϣ�࣬ÿ���ͻ���һ��ʵ��
 * ͨ��ΨһID��ʾ���ɷ�������ɺ���ͻ��˷���
 * @author Administrator
 *
 */

public class ClientInfo {
	
	enum EClientInfo_STATUS{
		EClientInfo_STATUS_UNDEFINED, //�����ӣ�δ����IDʱ
		EClientInfo_STATUS_CONNECTING, //��������״̬
		EClientInfo_STATUS_WAITTING, //��������ڵȴ���Client��������
		
	}

	int clientId; //�ͻ���ID��Ψһ��ʾ
	String clientIp; //�ͻ���ip��Ψһ��ʾ
	int clientPort; //�ͻ��˶˿ڣ�������һ��IP��������
	
	
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
	
	//�ͻ��˾��
}
