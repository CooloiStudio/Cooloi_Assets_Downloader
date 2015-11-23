#include "HelloWorldScene.h"

#include "SimpleAudioEngine.h"
#include "download_manager.hpp"
#include "assets_downloader.hpp"

#include <iostream>
#include <string>
#include <fstream>

using namespace CocosDenshion;

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
//    auto scene = Scene::create();
    auto scene = DownloadManager::Create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

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
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithSystemFont("Show Stage", "Arail", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1, "msg");
    
    auto label_percent = Label::createWithSystemFont("Show package number",
                                                     "Arail",
                                                     20);
    label_percent->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                    origin.y + label_percent->getContentSize().height));
    addChild(label_percent, 1, "percent");
    
    auto label_process = Label::createWithSystemFont("Show percent by each package",
                                                     "Arail",
                                                     20);
    label_process->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                    origin.y + label_process->getContentSize().height * 2));
    addChild(label_process, 1, "process");

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
//    auto audio = SimpleAudioEngine::getInstance();
//    audio->playBackgroundMusic("east.mp3", true);
    
//    auto downloader = new DownloadManager();
//    
//    downloader->init();
    
    scheduleUpdate();
    
    return true;
}

void HelloWorld::update(float)
{
    auto dl = (DownloadManager*)Director::getInstance()->getRunningScene();
    std::string str = "";
    switch (dl->stage())
    {
        case DownloadStage::kNull:
            break;
            
        case DownloadStage::kLoadConfig:
        {
            str = "加载咒语";
        }
            break;
        case DownloadStage::kInitDownloader:
        {
            str = "正在感应魔法能量";
        }
            break;
        case DownloadStage::kLoadUpdate:
        {
            str = "寻找最新的神谕";
        }
            break;
        case DownloadStage::kCheckUpdate:
        {
            str = "正在突破混沌，检查魔力碎片";
        }
            break;
        case DownloadStage::kGetUpdate:
        {
            str = "同步魔术资源中...";
            
            std::string str_pro = "";
            str_pro = "正在同步";
            str_pro += std::to_string(dl->finished().size() + 1);
            str_pro += "/";
            str_pro += std::to_string(dl->update().size());
            
            auto lable_process = (Label*)getChildByName("process");
            lable_process->setString(str_pro);
            
            auto percent = dl->downloader()->percent();
            std::string str_per = "";
            switch (percent)
            {
                case 0:
                    break;
                case 100:
                {
                    str_per = "\n解读神谕中...";
                }
                default:
                {
                    str_per = "\n已同步 " + std::to_string(percent) + "%";
                }
                    break;
            }
            
            auto lable_per = (Label*)getChildByName("percent");
            lable_per->setString(str_per);
        }
            break;
        case DownloadStage::kFinished:
        {
            str = "准备完毕，开始进入图兰卡";
            unscheduleUpdate();
        }
            break;
    }
    auto lable = (Label*)getChildByName("msg");
    lable->setString(str);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    auto dl = (DownloadManager*)Director::getInstance()->getRunningScene();
    dl->GetUpdate();
    
//    dl->downloader()->Reset();
//    Director::getInstance()->end();
//
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    exit(0);
//#endif
}
