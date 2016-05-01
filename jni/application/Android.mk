LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := application

LOCAL_SRC_FILES := main_android.cpp \
                   AppDelegate.cpp \
                   HelloWorldScene.cpp

LOCAL_SRC_FILES +=

LOCAL_SRC_FILES += cocos2dxgraphics.cpp
LOCAL_SRC_FILES += cocos2dximage.cpp
LOCAL_SRC_FILES += cocos2dximageloader.cpp
LOCAL_SRC_FILES += cocos2dxinput.cpp

LOCAL_SRC_FILES += guichan/actionevent.cpp guichan/basiccontainer.cpp guichan/cliprectangle.cpp 
LOCAL_SRC_FILES += guichan/color.cpp guichan/defaultfont.cpp guichan/event.cpp guichan/exception.cpp 
LOCAL_SRC_FILES += guichan/focushandler.cpp guichan/font.cpp guichan/genericinput.cpp 
LOCAL_SRC_FILES += guichan/graphics.cpp guichan/gui.cpp guichan/guichan.cpp guichan/image.cpp 
LOCAL_SRC_FILES += guichan/imagefont.cpp guichan/inputevent.cpp guichan/key.cpp 
LOCAL_SRC_FILES += guichan/keyevent.cpp guichan/keyinput.cpp guichan/mouseevent.cpp 
LOCAL_SRC_FILES += guichan/mouseinput.cpp guichan/rectangle.cpp guichan/selectionevent.cpp guichan/widget.cpp

LOCAL_SRC_FILES += guichan/widgets/button.cpp guichan/widgets/checkbox.cpp guichan/widgets/container.cpp guichan/widgets/dropdown.cpp 
LOCAL_SRC_FILES += guichan/widgets/icon.cpp guichan/widgets/imagebutton.cpp guichan/widgets/label.cpp guichan/widgets/listbox.cpp 
LOCAL_SRC_FILES += guichan/widgets/radiobutton.cpp guichan/widgets/scrollarea.cpp guichan/widgets/slider.cpp 
LOCAL_SRC_FILES += guichan/widgets/tab.cpp guichan/widgets/tabbedarea.cpp guichan/widgets/textbox.cpp 
LOCAL_SRC_FILES += guichan/widgets/textfield.cpp guichan/widgets/window.cpp

LOCAL_SRC_FILES += sdlmod/SDLMOD_blit_0.c sdlmod/SDLMOD_blit_1.c sdlmod/SDLMOD_blit_A.c
LOCAL_SRC_FILES += sdlmod/SDLMOD_blit_N.c sdlmod/SDLMOD_ext_pixel.c
LOCAL_SRC_FILES += sdlmod/SDLMOD_surface.c sdlmod/SDLMOD_yuv.c

LOCAL_SRC_FILES += loader/TextureLoader.cpp



LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/guichan
LOCAL_C_INCLUDES += $(LOCAL_PATH)/sdlmod
LOCAL_C_INCLUDES += $(LOCAL_PATH)/loader

LOCAL_CFLAGS := -fexceptions 

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx

include $(BUILD_SHARED_LIBRARY)
