/* Only for my mingw compiler =_=b */
#ifdef __MINGW32__
#undef UNICODE
#undef _UNICODE
/*#undef _tWinMain*/
/*#define _tWinMain WinMain*/
#endif

#include "main_win32.h"
#include "AppDelegate.h"
#include "CCEGLView.h"

USING_NS_CC;
int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setViewName("HelloCpp");
    eglView->setFrameSize(2048, 1536);
    // The resolution of ipad3 is very large. In general, PC's resolution is smaller than it.
    // So we need to invoke 'setFrameZoomFactor'(only valid on desktop(win32, mac, linux)) to make the window smaller.
    eglView->setFrameZoomFactor(0.4f);
    return CCApplication::sharedApplication()->run();
}
