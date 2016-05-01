#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "guichan/guichan.hpp"
#include "cocos2dxinput.hpp"
#include "cocos2dxgraphics.hpp"
#include "cocos2dximageloader.hpp"
#include "guichan/platform.hpp"

#define TIMER_MILLIS ((int)(1000 / 60))

#define Rmask 0x000000FF
#define Gmask 0x0000FF00
#define Bmask 0x00FF0000
#define Amask 0xFF000000


#define FILENAME "image1.bmp"
#define TITLE "OpenGL & COCOS2DX sample program"

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(cocos2d::CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

	//--------------------------
	virtual void update(float delta);
    virtual void registerWithTouchDispatcher(void);
    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void onExit();

	cocos2d::CCSprite* pSprite;
	cocos2d::CCTexture2D* createTexture(const uint32_t* RgbaData, int width, int hight);


	uint32_t g_TextureArray[1];
	int g_ID;// = 0;

	static int win;
	static int menyid;
	static int animeringsmeny;
	static int springmeny;
	int val;// = 0;

	unsigned char * texture_data;// = NULL;
	int textureContainsData;// = 0;

	/*=============================*/

	SDLMOD_Surface* screen;

	/*=============================*/


    bool running;// = true;

    gcn::COCOS2DXGraphics* graphics;
    gcn::COCOS2DXInput* input;
    gcn::COCOS2DXImageLoader* imageLoader;

	bool m_isDragged;// = false;
	float m_dragedX; // = -1;
	float m_dragedY; // = -1;

	void cocos2dxinit();
	void cocos2dxhalt();
};

#endif // __HELLOWORLD_SCENE_H__
