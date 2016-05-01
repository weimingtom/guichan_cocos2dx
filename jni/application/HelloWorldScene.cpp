#include "HelloWorldScene.h"
#include "AppMacros.h"

#include "guichan/guichan.hpp"
#include <iostream>

// Here we store a global Gui object.  We make it global
// so it's easily accessable. Of course, global variables
// should normally be avioded when it comes to OOP, but
// this examples is not an example that shows how to make a 
// good and clean C++ application but merely an example
// that shows how to use Guichan.
namespace globals
{
    gcn::Gui* gui;
}
#include "cocos2dxinput.hpp"
#include "widgets.hpp"



USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
#if 0
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", TITLE_FONT_SIZE);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);
#endif

	//---------------------------
	this->cocos2dxinit();
	widgets::init();
	this->CCNode::scheduleUpdate();
	this->CCLayer::setTouchEnabled(true);
	//TODO:keyboard support, use CCIMEDelegate
	//this->CCLayer::setKeypadEnabled(true);

	SDLMOD_LockSurface(screen);
	cocos2d::CCTexture2D* texture = this->createTexture((const uint32_t*)screen->pixels, screen->w, screen->h);
	SDLMOD_UnlockSurface(screen);
	//---------------------------

    this->pSprite = CCSprite::createWithTexture(texture);

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	
	// scale to full screen
	float scale1 = (float)visibleSize.width/(float)pSprite->getContentSize().width;
	float scale2 = (float)visibleSize.height/(float)pSprite->getContentSize().height;
	pSprite->setScale(MIN(scale1, scale2));
	
	//----------------------------

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);

    return true;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

//----------------------------------------------

void HelloWorld::cocos2dxinit()
{
	g_ID = 0;
	val = 0;
	texture_data = NULL;
	textureContainsData= 0;
    running = true;
	m_isDragged = false;
	m_dragedX = m_dragedY = -1;

    //SDL_Init(SDL_INIT_VIDEO);
    this->screen = SDLMOD_CreateRGBSurface(0,
		640, 480, 32,
		Rmask, Gmask, Bmask, Amask
		);
    //SDL_EnableUNICODE(1);
    //SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

    imageLoader = new gcn::COCOS2DXImageLoader();
    gcn::Image::setImageLoader(imageLoader);
    graphics = new gcn::COCOS2DXGraphics();
    graphics->setTarget(screen);
    input = new gcn::COCOS2DXInput();

    globals::gui = new gcn::Gui();
    globals::gui->setGraphics(graphics);
    globals::gui->setInput(input);
}

void HelloWorld::onExit()
{
	widgets::halt();
	this->cocos2dxhalt();
	SDLMOD_FreeSurface(this->screen);
}

void HelloWorld::cocos2dxhalt()
{
    delete globals::gui;

    delete this->imageLoader;
    delete this->input;
    delete this->graphics;

    //SDL_Quit();
}

void HelloWorld::update(float delta)
{
	//CCLog("update %f", delta);

	globals::gui->logic();
    globals::gui->draw();

	SDLMOD_LockSurface(screen);
	//dumpBMPRaw("image1_out.bmp", screen->pixels, screen->w, screen->h);
	/*
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
		screen->w, screen->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
		screen->pixels);
	*/
	cocos2d::CCTexture2D* texture = this->createTexture((const uint32_t*)screen->pixels, screen->w, screen->h);

	SDLMOD_UnlockSurface(screen);

	pSprite->setTexture(texture);
	//pSprite->setTextureRect(CCRectMake(0, 0, screen->w, screen->h));

	if (this->m_isDragged)
	{
		CCLog("ccTouchesMoved update (%f,%f)", 
			(double)m_dragedX, (double)m_dragedY);
		this->input->pushPassiveMotionInput(m_dragedX, m_dragedY);
	}
}

void HelloWorld::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);
}

void HelloWorld::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator iter = pTouches->begin();
    if (pTouches->count() >= 1)
    {
		CCPoint nodePoint = this->pSprite->convertToNodeSpace(((CCTouch*)(*iter))->getLocation()); 
		CCSize contentSize = this->pSprite->getContentSize();
		float scale = screen->w / contentSize.width;
		int sx = (int)(nodePoint.x * scale);
		int sy = (int)(screen->h - nodePoint.y * scale);
		CCLog("ccTouchesBegan (%f,%f) | %f,%f,%f,%f | %f", 
			(double)sx, (double)sy,
			(double)nodePoint.x, (double)nodePoint.y, 
			(double)contentSize.width, (double)contentSize.height, 
			(double)((float)contentSize.height / (float)contentSize.width));

		this->input->pushMouseInput(0, CCTOUCHBEGAN, sx, sy);
		this->m_dragedX = sx;
		this->m_dragedY = sy;
		this->m_isDragged = true;
	}
}

void HelloWorld::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator iter = pTouches->begin();
    if (pTouches->count() >= 1)
    {
		CCPoint nodePoint = this->pSprite->convertToNodeSpace(((CCTouch*)(*iter))->getLocation()); 
		CCSize contentSize = this->pSprite->getContentSize();
		float scale = screen->w / contentSize.width;
		int sx = (int)(nodePoint.x * scale);
		int sy = (int)(screen->h - nodePoint.y * scale);
		CCLog("ccTouchesMoved (%f,%f) | %f,%f,%f,%f | %f", 
			(double)sx, (double)sy,
			(double)nodePoint.x, (double)nodePoint.y, 
			(double)contentSize.width, (double)contentSize.height, 
			(double)((float)contentSize.height / (float)contentSize.width));

		this->input->pushPassiveMotionInput(sx, sy);
		this->m_dragedX = sx;
		this->m_dragedY = sy;
		this->m_isDragged = true;
	}
}

void HelloWorld::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator iter = pTouches->begin();
    if (pTouches->count() >= 1)
    {
		CCPoint nodePoint = this->pSprite->convertToNodeSpace(((CCTouch*)(*iter))->getLocation()); 
		CCSize contentSize = this->pSprite->getContentSize();
		float scale = screen->w / contentSize.width;
		int sx = (int)(nodePoint.x * scale);
		int sy = (int)(screen->h - nodePoint.y * scale);
		CCLog("ccTouchesEnded (%f,%f) | %f,%f,%f,%f | %f", 
			(double)sx, (double)sy,
			(double)nodePoint.x, (double)nodePoint.y, 
			(double)contentSize.width, (double)contentSize.height, 
			(double)((float)contentSize.height / (float)contentSize.width));

		this->input->pushMouseInput(0, CCTOUCHENDED, sx, sy);
		m_isDragged = false;
	}
}

void HelloWorld::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
	this->ccTouchesEnded(pTouches, pEvent);
	m_isDragged = false;
}

CCTexture2D* HelloWorld::createTexture(const uint32_t* RgbaData, int width, int hight)
{
	CCTexture2D* texture = NULL;
	int pixelLenth = width * hight;
	std::string textureName = "guichan";

	if (RgbaData == NULL)
	{
		return NULL;
	}

	CCImage* img = new CCImage();
	do 
	{
		bool res = true;
		res = img->initWithImageData((void*)RgbaData, pixelLenth, CCImage::kFmtRawData, width, hight, 8);
		if(!res) break;

		CCTextureCache::sharedTextureCache()->removeTextureForKey(textureName.c_str());
        
        //Adding texture to CCTextureCache  to ensure that on the Android platform, when cut into the foreground from the background, the VolatileTexture can reload our texture
		texture = CCTextureCache::sharedTextureCache()->addUIImage(img, textureName.c_str());
	} while (0);

	CC_SAFE_RELEASE(img);

	return texture;
}
