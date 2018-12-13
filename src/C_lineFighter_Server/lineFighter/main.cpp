#include "global.h"
#include "model_comm.h"
#include "comm_com.h"
#include "CProtocol.h"
#include "CGameMaster.h"


char g_Control1Name[30];

int main(int argc , char ** argv)
{
	 //������־����
	 if (ACE_Service_Config::open (argc,argv,ACE_DEFAULT_LOGGER_KEY,1,0,1) < 0) return 0;
	 ACE_DEBUG ((HRJ_ERROR_PX ACE_TEXT("main():LFServer start up!\n")));

	 //��־����߳�
	 CLogMonitor logMonitor;
	 logMonitor.run();

	 CGameMaster cGameMaster;
	 cGameMaster.run();

	 LFclear();//��ʼ��clear

	 //�ն˲��������
	 ACE_INET_Addr addr1(10091);
	 Gateway_Acceptor gateway_acceptor;
	 if(gateway_acceptor.open(addr1, 0, 1, 100,1,0,1)== -1) //��validate_new_connection�ص�
	 {
		 ACE_DEBUG ((HRJ_ERROR_PX ACE_TEXT("main():Open terminal port failed.[PORT]:10091.\n")));
		 return -1;
	 }
	 ACE_DEBUG ((HRJ_DEBUG_PX ACE_TEXT("main():Terminal Connection Listen port(10091) ok ...\n")));

	 ACE_Proactor::instance()->proactor_run_event_loop();

	 return 0;
}