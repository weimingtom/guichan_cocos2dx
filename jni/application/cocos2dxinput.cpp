/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naess��n and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naess��n a.k.a jansem/yakslem
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

/*
* For comments regarding functions please see the header file.
*/

#include "cocos2dxinput.hpp"
#include "guichan/exception.hpp"

namespace gcn
{

 COCOS2DXInput::COCOS2DXInput()
 {
 }

 bool COCOS2DXInput::isKeyQueueEmpty()
 {
     return mKeyInputQueue.empty();
 }

 KeyInput COCOS2DXInput::dequeueKeyInput()
 {
     KeyInput keyInput;

     if (mKeyInputQueue.empty())
     {
         throw GCN_EXCEPTION("The queue is empty.");
     }

     keyInput = mKeyInputQueue.front();
     mKeyInputQueue.pop();

     return keyInput;
 }

 bool COCOS2DXInput::isMouseQueueEmpty()
 {
     return mMouseInputQueue.empty();
 }

 MouseInput COCOS2DXInput::dequeueMouseInput()
 {
     MouseInput mouseInput;

     if (mMouseInputQueue.empty())
     {
         throw GCN_EXCEPTION("The queue is empty.");
     }

     mouseInput = mMouseInputQueue.front();
     mMouseInputQueue.pop();

     return mouseInput;
 }

 void COCOS2DXInput::pushKeyDownInput(unsigned char key)
 {
     KeyInput keyInput;
     keyInput.setKey(convertKeyCharacter(key));
     keyInput.setType(KeyInput::PRESSED);
	 mKeyInputQueue.push(keyInput);
 }

 void COCOS2DXInput::pushKeyUpInput(unsigned char key)
 {
     KeyInput keyInput;
     keyInput.setKey(convertKeyCharacter(key));
     keyInput.setType(KeyInput::RELEASED);
     mKeyInputQueue.push(keyInput);
 }

 void COCOS2DXInput::pushSpecialDownInput(unsigned char key)
 {
     KeyInput keyInput;
     keyInput.setKey(convertSpecialCharacter(key));
     keyInput.setType(KeyInput::PRESSED);
     mKeyInputQueue.push(keyInput);

 }

 void COCOS2DXInput::pushSpecialUpInput(unsigned char key)
 {
     KeyInput keyInput;
     keyInput.setKey(convertSpecialCharacter(key));
     keyInput.setType(KeyInput::RELEASED);
     mKeyInputQueue.push(keyInput);
 }

 void COCOS2DXInput::pushMouseInput(int button, int state, int x, int y)
 {
	 //FIXME:
     MouseInput mouseInput;
     mouseInput.setX(x);
     mouseInput.setY(y);
     mouseInput.setButton(convertMouseButton(button));
	
     if (state == cocos2d::CCTOUCHBEGAN)
     {
         mouseInput.setType(MouseInput::PRESSED);
     }
     else
     {
         mouseInput.setType(MouseInput::RELEASED);
     }
     mMouseInputQueue.push(mouseInput);
 }


 void COCOS2DXInput::pushMotionInput(int x, int y)
 {
     MouseInput mouseInput;
     mouseInput.setX(x);
     mouseInput.setY(y);
     mouseInput.setButton(MouseInput::EMPTY);
     mouseInput.setType(MouseInput::MOVED);
     mMouseInputQueue.push(mouseInput);
 }

 void COCOS2DXInput::pushPassiveMotionInput(int x, int y)
 {
     pushMotionInput(x, y);
 }

 Key COCOS2DXInput::convertKeyCharacter(unsigned char key)
 {
     int value = 0;
     Key gcnKey;

     value = (int)key;

     switch(key)
     {
         case 8:
             value = Key::BACKSPACE;
             break;
         case 13:
             value = Key::ENTER;
             break;
         case 27:
             value = Key::ESCAPE;
             break;
         case 127:
             value = Key::DELETE;
             break;
     }

#if 0
     int modifiers = cocos2dxGetModifiers();
     gcnKey.setShiftPressed(modifiers & COCOS2DX_ACTIVE_SHIFT);
     gcnKey.setControlPressed(modifiers & COCOS2DX_ACTIVE_CTRL);
     gcnKey.setAltPressed(modifiers & COCOS2DX_ACTIVE_ALT);
#endif

     gcnKey = (value);

     return gcnKey;
 }

 Key COCOS2DXInput::convertSpecialCharacter(unsigned char key)
 {
     int value = 0;
     Key gcnKey;

     value = (int)key;

//FIXME:
/*
     switch(key)
     {
         case COCOS2DX_KEY_UP:
             value = Key::UP;
             break;
         case COCOS2DX_KEY_DOWN:
             value = Key::DOWN;
             break;
         case COCOS2DX_KEY_RIGHT:
             value = Key::RIGHT;
             break;
         case COCOS2DX_KEY_LEFT:
             value = Key::LEFT;
             break;
         case COCOS2DX_KEY_F1:
             value = Key::F1;
             break;
         case COCOS2DX_KEY_F2:
             value = Key::F2;
             break;
         case COCOS2DX_KEY_F3:
             value = Key::F3;
             break;
         case COCOS2DX_KEY_F4:
             value = Key::F4;
             break;
         case COCOS2DX_KEY_F5:
             value = Key::F5;
             break;
         case COCOS2DX_KEY_F6:
             value = Key::F6;
             break;
         case COCOS2DX_KEY_F7:
             value = Key::F7;
             break;
         case COCOS2DX_KEY_F8:
             value = Key::F8;
             break;
         case COCOS2DX_KEY_F9:
             value = Key::F9;
             break;
         case COCOS2DX_KEY_F10:
             value = Key::F10;
             break;
         case COCOS2DX_KEY_F11:
             value = Key::F11;
             break;
         case COCOS2DX_KEY_F12:
             value = Key::F12;
             break;
         case COCOS2DX_KEY_PAGE_UP:
             value = Key::PAGE_UP;
             break;
         case COCOS2DX_KEY_PAGE_DOWN:
             value = Key::PAGE_DOWN;
             break;
         case COCOS2DX_KEY_HOME:
             value = Key::HOME;
             break;
         case COCOS2DX_KEY_END:
             value = Key::END;
             break;
         case COCOS2DX_KEY_INSERT:
             value = Key::INSERT;
             break;
         default:
             break;
     }
*/

#if 0
     int modifiers = cocos2dxGetModifiers();
     gcnKey.setShiftPressed(modifiers & COCOS2DX_ACTIVE_SHIFT);
     gcnKey.setControlPressed(modifiers & COCOS2DX_ACTIVE_CTRL);
     gcnKey.setAltPressed(modifiers & COCOS2DX_ACTIVE_ALT);
#endif 
     gcnKey = (value);

     return gcnKey;
 }

 int COCOS2DXInput::convertMouseButton(int button)
 {
	 //FIXME:
	/*
     switch (button)
     {
         case COCOS2DX_LEFT_BUTTON:
             return MouseInput::LEFT;
             break;
         case COCOS2DX_RIGHT_BUTTON:
             return MouseInput::RIGHT;
             break;
         case COCOS2DX_MIDDLE_BUTTON:
             return MouseInput::MIDDLE;
             break;
     }
	 
	 throw GCN_EXCEPTION("Unknown COCOS2DX mouse type.");

     return 0;
	 */

	 return MouseInput::LEFT;
 }
}
