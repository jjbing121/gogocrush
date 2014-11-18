#include "AppDelegate.h"
#include "HelloWorldScene.h"

USING_NS_CC;

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

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();

    // add adjust screen comfot
//    glview->setDesignResolutionSize(1242.0, 2208.0, ResolutionPolicy::SHOW_ALL);    // iphone6s
    glview->setDesignResolutionSize(750.0, 1334.0, ResolutionPolicy::SHOW_ALL);     // iphone6
//    glview->setDesignResolutionSize(640.0, 1136.0, ResolutionPolicy::SHOW_ALL);     // iphone5s
//    glview->setDesignResolutionSize(640.0, 960.0, ResolutionPolicy::SHOW_ALL);      // iphone4s
    // add content scale factor to
    // director->setContentScaleFactor(1334.0/750.0);
    // add new folder to search
    std::vector<std::string> searchPath;
    searchPath.push_back("bgandeffect");
    searchPath.push_back("element");

    
    // run
    director->runWithScene(scene);

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
