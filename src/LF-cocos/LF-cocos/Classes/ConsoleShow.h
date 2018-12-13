#include "model_comm.h"
#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif

void showPlayer(vector<CPlayer>& vplayers);

void LcmSetCursor(int byRow, int byCol);

void showGameOver(CPlayer player,int index);

void showRoundIndex(int index);

void waitAkey();

void showDetail(map<int,vector<string> >& vdiscreption , map<int,vector<CPlayer> >& vShowPlayer,map<string,vector<CAction> >& vActions,vector<CPlayer>& vplayers);

void showComand(map<string,vector<CAction> >& vActions,vector<CPlayer>& vplayers);

void showComandSetp(int index,map<string,vector<CAction> >& vActions,vector<CPlayer>& vplayers);

void showDetailByStep(map<int,vector<string> >& vdiscreption , map<int,vector<CPlayer> >& vShowPlayer,map<string,vector<CAction> >& vActions,vector<CPlayer>& vplayers);

int getActionFromConsole(vector<CAction>& subVec, CPlayer player);
