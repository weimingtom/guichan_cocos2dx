#pragma once

#include <string>
#include "SDLMOD_video.h"
#include "SDLMOD_ext_pixel.h"
#include "guichan/color.hpp"
#include "guichan/platform.hpp"
#include "guichan/image.hpp"

namespace gcn
{
    class GCN_EXTENSION_DECLSPEC COCOS2DXImage : public Image
    {
    public:
        COCOS2DXImage(SDLMOD_Surface* surface, bool autoFree);
        virtual ~COCOS2DXImage();
        virtual SDLMOD_Surface* getSurface() const;
        virtual void free();
        virtual int getWidth() const;
        virtual int getHeight() const;
        virtual Color getPixel(int x, int y);
        virtual void putPixel(int x, int y, const Color& color);
        virtual void convertToDisplayFormat();

    protected:
        SDLMOD_Surface* mSurface;
        bool mAutoFree;
    };
}
