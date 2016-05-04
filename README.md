# guichan_cocos2dx
Port guichan_memory (guichan) to cocos2dx (cocos2d-x) 2.x (my vc6/mingw32/android modification)

## TODO  
* Keyboard input  
* More demos  

## References  
* guichan 0.8.2  
https://code.google.com/p/guichan/  
* marika_cocos2dx  
https://github.com/weimingtom/marika_cocos2dx  
* Cocos2d-x 2.x  
https://github.com/cocos2d/cocos2d-x  
* gif-for-cocos2dx  
https://github.com/opentalking/gif-for-cocos2dx  
* guichan_memory  
https://github.com/weimingtom/guichan_memory  
* SDL 1.x  
https://www.libsdl.org/  

## How to build  
* VC6  
see guichan_cocos2dx\jni\application\application.dsw  

* MinGW32  
$ cd /D/android_guichan_cocos2dx/guichan_cocos2dx/jni  
$ make clean all  
$ make test  

* Android  
$ console.bat  
$ ndk-build  
$ android_update_prject.bat  
$ ant debug  
$ ant installd  

## Screenshot
![Snapshot](/doc/screenshot002.jpg)    

## History:  
* 2016-05-04: Bug fix, window widget alpha base color bug,  
* 2016-05-02: First commit.  
