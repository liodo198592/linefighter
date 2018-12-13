#ifdef MAKE_ANDRIOD
#include "AppDelegate.h"
#include "cocos2d.h"
#include "CCEventType.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <android/log.h>
#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif

#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

using namespace cocos2d;

extern "C"
{

	jint JNI_OnLoad(JavaVM *vm, void *reserved)
	{
		JniHelper::setJavaVM(vm);

		return JNI_VERSION_1_4;
	}

	void Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInit(JNIEnv*  env, jobject thiz, jint w, jint h)
	{
		if (!CCDirector::sharedDirector()->getOpenGLView())
		{
			CCEGLView *view = CCEGLView::sharedOpenGLView();
			view->setFrameSize(w, h);
			//view->Create(1200,800);
			//view->setFrameSize(800, 450);
			//view->setDesignResolutionSize(SCREEN_SIZE_X,SCREEN_SIZE_Y);

			AppDelegate *pAppDelegate = new AppDelegate();
			CCApplication::sharedApplication()->run();
		}
		else
		{
			ccGLInvalidateStateCache();
			CCShaderCache::sharedShaderCache()->reloadDefaultShaders();
			ccDrawInit();
			CCTextureCache::reloadAllTextures();
			CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_COME_TO_FOREGROUND, NULL);
			CCDirector::sharedDirector()->setGLDefaultValues(); 
		}
	}

}

#else

#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"
#ifdef LF_COCOS 
#include "global_cocos.h"
#else
#include "global.h"
#endif


USING_NS_CC;

// uncomment below line, open debug console
// #define USE_WIN32_CONSOLE

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef USE_WIN32_CONSOLE
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#endif

	// create the application instance
	AppDelegate app;
	CCEGLView* eglView = CCEGLView::sharedOpenGLView();
	eglView->setFrameSize(SCREEN_SIZE_X, SCREEN_SIZE_Y);
	//eglView->setDesignResolutionSize(SCREEN_SIZE_X,SCREEN_SIZE_Y,kResolutionNoBorder);
	int ret = CCApplication::sharedApplication()->run();

#ifdef USE_WIN32_CONSOLE
	FreeConsole();
#endif

	return ret;
}
#endif // MAKE_ANDRIOD






