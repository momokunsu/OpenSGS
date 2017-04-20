#include "AppDelegate.h"
#include "BattleScene.h"

USING_NS_CC;

#ifdef WIN32
WNDPROC _wproc = nullptr;
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_SIZE:
			auto glview = Director::getInstance()->getOpenGLView();
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);
			if((!width) || (!height))
				break;
			auto size = glview->getFrameSize();
			if(size.width == (float)width + 10 && size.height == (float)height + 10)
				break;
			glview->setFrameSize(width, height);
			auto ws = 1920.0f * 1.2f / width;
			auto hs = 1080.0f * 1.2f / height;
			if(ws > hs)
				glview->setDesignResolutionSize(1920.0f * 1.2f, height * ws, ResolutionPolicy::EXACT_FIT);
			else
				glview->setDesignResolutionSize(width * hs, 1080.0f * 1.2f, ResolutionPolicy::EXACT_FIT);
			Director::getInstance()->getRunningScene()->init();
			break;
	}
	return _wproc(hwnd, message, wParam, lParam);
}
#endif

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }

	auto size = glview->getFrameSize();
	auto ws = 1920.0f * 1.2f / size.width;
	auto hs = 1080.0f * 1.2f / size.height;
	if(ws > hs)
		glview->setDesignResolutionSize(1920.0f * 1.2f, size.height * ws, ResolutionPolicy::EXACT_FIT);
	else
		glview->setDesignResolutionSize(size.width * hs, 1080.0f * 1.2f, ResolutionPolicy::EXACT_FIT);

#ifdef WIN32
	RECT rect, wrect;
	auto hwnd = glview->getWin32Window();
	GetClientRect(hwnd, &rect);
	GetWindowRect(hwnd, &wrect);
	_wproc = (WNDPROC)GetWindowLongW(hwnd, GWL_WNDPROC);
	auto style = GetWindowLongW(hwnd, GWL_STYLE);
	AdjustWindowRect(&rect, style | WS_MAXIMIZEBOX | WS_THICKFRAME, FALSE);
	SetWindowLongW(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME);
	SetWindowLongW(hwnd, GWL_WNDPROC, (LONG)WndProc);
	SetWindowPos(hwnd, NULL, wrect.left, wrect.top, rect.right - rect.left, rect.bottom - rect.top,
		SWP_NOZORDER | SWP_SHOWWINDOW);
#endif


    // turn on display FPS
    //director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    //director->setAnimationInterval(1.0 / 60);

    FileUtils::getInstance()->addSearchPath("res");

    // create a scene. it's an autorelease object
    //auto scene = HelloWorld::createScene();

    // run
	director->runWithScene(BattleScene::getInstance());

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}