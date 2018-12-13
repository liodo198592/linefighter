package LineFighter.Core;

public class Const {
	
	static Long MAX_HP = 17L;
	static Long statusFlag; //UI和engine 之间共享，用来决定是否进行下一步工作
	static Long MAX_PLAYER = 4L;
	static Long MAX_ACTNUM = 3L;
	static String g_Control1Name = "zhouyu";
	static Long SCFlag; //1 表示 服务器  2表示客户端

	static String selfName; //自身的名字
	
	public static Long SHOW_LINE_NUM = 15L;
	public static Long SHOW_COL_NUM = 40L;
	
	public static Long SHOW_ROW_LEN = 12L;
	public static Long SHOW_BOX_ROW = 6L;
	
	static enum ELIFESTAT
	{
		ELIFESTAT_INVALID,
		ELIFESTAT_ALIFE,
		ELIFESTAT_DEAD,
	} ;
	
	static enum EACTION
	{
		EACTION_Hand ,//拳
		EACTION_Air ,//气
		EACTION_Fall,//摔
		EACTION_Move,//移动
		EACTION_ChangeLine,//换线
		EACTION_Avoid,//回避
		EACTION_Kill,//必杀
		EACTION_Invalid,//无效
	};
	
	public static EACTION getEaction(Long index){
		EACTION type = null;
		switch(index.intValue()){
		case 1: type = EACTION.EACTION_Hand; break;
		case 2: type = EACTION.EACTION_Air; break;
		case 3: type = EACTION.EACTION_Fall; break;
		case 4: type = EACTION.EACTION_Move; break;
		case 5: type = EACTION.EACTION_ChangeLine; break;
		case 6: type = EACTION.EACTION_Avoid; break;
		case 7: type = EACTION.EACTION_Kill; break;
		case 8: type = EACTION.EACTION_Invalid; break;
		default: return null;
		}
		return type;
	}
}
