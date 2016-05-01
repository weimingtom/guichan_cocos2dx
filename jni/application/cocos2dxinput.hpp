/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naess¨¦n and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naess¨¦n a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef GCN_COCOS2DXINPUT_HPP
#define GCN_COCOS2DXINPUT_HPP

#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

#include <queue>

/*#include <GL/cocos2dx.h>*/
#include "cocos2d.h"

#include "guichan/input.hpp"
#include "guichan/key.hpp"
#include "guichan/keyinput.hpp"
#include "guichan/mouseinput.hpp"
#include "guichan/platform.hpp"

namespace gcn
{
 /**
  * COCOS2DX implementation of Input.
  */
 class GCN_EXTENSION_DECLSPEC COCOS2DXInput : public Input
 {
 public:

     /**
      * Constructor.
      */
     COCOS2DXInput();

     /**
      * Pushes a key down. It should be called in the
      * cocos2dxKeyboardFunc call back function.
      *
      * @param key a key from COCOS2DX.
      */
     virtual void pushKeyDownInput(unsigned char key);

     /**
      * Pushes a key up. It should be called in the
      * cocos2dxKeyboardUpFunc call back function.
      *
      * @param key a key from COCOS2DX.
      */
     virtual void pushKeyUpInput(unsigned char key);

     /**
      * Pushes a special key down. It should be called in the
      * cocos2dxSpecialFunc call back function.
      *
      * @param key a key from COCOS2DX.
      */
     virtual void pushSpecialDownInput(unsigned char key);

     /**
      * Pushes a special key up. It should be called in the
      * cocos2dxSpecialUpFunc call back function.
      *
      * @param key a key from COCOS2DX.
      */
     virtual void pushSpecialUpInput(unsigned char key);

     /**
      * Pushes a mouse input. It should be called in the cocos2dxMouseFunc
      * call back function.
      *
      * @param button a button from COCOS2DX.
      * @param state a mouse state from COCOS2DX.
      * @param x an x coordinate from COCOS2DX.
      * @param y an y coordinate from COCOS2DX.
      */
     virtual void pushMouseInput(int button, int state, int x, int y);

     /**
      * Pushes a mouse motion input. It should be called in the
      * cocos2dxMotionFunc call back function.
      *
      * @param x an x coordinate from COCOS2DX.
      * @param y an y coordinate from COCOS2DX.
      */
     virtual void pushMotionInput(int x, int y);

     /**
      * Pushes a passive mouse motion input. It should be called in the
      * cocos2dxPassiveMotionFunc call back function.
      *
      * @param x an x coordinate from COCOS2DX.
      * @param y an y coordinate from COCOS2DX.
      */
     virtual void pushPassiveMotionInput(int x, int y);

     /**
      * Polls all input. It exists for input driver compatibility. If you
      * only use COCOS2DX and plan sticking with COCOS2DX you can safely ignore this
      * function as it in the COCOS2DX case does nothing.
      */
     virtual void _pollInput() { }


     // Inherited from Input

     virtual bool isKeyQueueEmpty();

     virtual KeyInput dequeueKeyInput();

     virtual bool isMouseQueueEmpty();

     virtual MouseInput dequeueMouseInput();

 protected:
     /**
      * Converts a COCOS2DX key to a Key object.
      *
      * @param key a COCOS2DX key.
      * @return a Key object.
      */
     Key convertKeyCharacter(unsigned char key);

     /**
      * Converts a special COCOS2DX key to a Key object.
      *
      * @param key a COCOS2DX key.
      * @return a Key object.
      */
     Key convertSpecialCharacter(unsigned char key);

     /**
      * Converts a mouse button from COCOS2DX to a Guichan mouse button
      * representation.
      *
      * @param button a COCOS2DX mouse button.
      * @return a Guichan mouse button.
      */
     int convertMouseButton(int button);

     std::queue<KeyInput> mKeyInputQueue;
     std::queue<MouseInput> mMouseInputQueue;
 };
}

#endif // end GCN_COCOS2DXINPUT_HPP
