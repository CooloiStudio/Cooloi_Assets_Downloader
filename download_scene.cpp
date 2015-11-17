//
//  download_scene.cpp
//  Test
//
//  Created by ESoragoto on 11/12/15.
//
//
#include <iostream>
#include <string>
#include <fstream>

#include "download_scene.hpp"

#include "assets_download.hpp"

#include "json/rapidjson.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

using namespace cocos2d;

Scene* DownloadScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = DownloadScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool DownloadScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithSystemFont("Hello World", "Arail", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width * 0.5,
                            origin.y + visibleSize.height * 0.5));
    
    addChild(label);
    
    downloader_ = new AssetsDownload();
    downloader_->init();
    Download("");
    
    
    ReadVer("update_list.json");
 
    
//    WriteVer("update_list.json");
    
//    ReadVer("update_list.json");
    
    return true;
}

void DownloadScene::update(float dt)
{
    if (downloader_->success())
    {
        unscheduleUpdate();
        ReadVer("update_list.json");
    }
}

void DownloadScene::DowanloadDone(float dt)
{
    log("Auuuu!");
    if (downloader_->success())
    {
//        usleep(0);
        set_dl_finished(true);
        auto schedu = Director::getInstance()->getScheduler();
        schedu->unschedule("ca", this);
    }
}

int DownloadScene::Download(const std::string pkg_url)
{
    downloader_->Download(pkg_url);
    
    
//    auto n = 0;
//    while (!downloader_->success())
//    {
//        sleep(2);
//        n++;
//        log("%d", n);
//        if (10 < n) break;
//    }
    
    auto schedu = Director::getInstance()->getScheduler();
    schedu->schedule(CC_CALLBACK_1(DownloadScene::DowanloadDone, this), //回调
                     this, //当前对象
                     0.1, //重复调用时间(s)
                     CC_REPEAT_FOREVER, //重复次数
                     0, //延迟调用时间(s)
                     true, //是否暂停等待
                     "ca"); //key
    
//    sleep(100);
    return 0;
}

int DownloadScene::ReadVer(const std::string file_name)
{
//    log("File name: %s", file_name.c_str());
    auto file_name_in_app = FileUtils::getInstance()->fullPathForFilename(file_name);
    
    log("Full path: %s", file_name_in_app.c_str());
    
    auto file = FileUtils::getInstance()->getStringFromFile(file_name_in_app);
//    auto file_content = file.c_str();
    
    if ("" == file)
    {
//        auto file_to_read = FileUtils::getInstance()->getWritablePath()
//        + "/download/"
//        + file_name;
        
        auto file_to_read = downloader_->path_to_save() + "/" + file_name;
        
        log("Full path: %s", file_to_read.c_str());
        std::ifstream infile(file_to_read);
        
        if (infile.fail())
        {
            log("Open file fail");
            return 1001;
        }
        
        std::string str((std::istreambuf_iterator<char>(infile)),
                        std::istreambuf_iterator<char>());
        file = str.c_str();
    }
    
    
    log("open file:\n----\n%s\n----", file.c_str());
    
    rapidjson::Document d;
    d.Parse<0>(file.c_str());
    
//    if (d.HasMember("a"))
//    {
//        auto a = d["a"].GetString();
//        log("a = %s", a);
//    }
    
    for (auto iter = d.MemberBegin(); iter != d.MemberEnd(); iter++)
    {
        auto key = (iter->name).GetString();
        log("key\t = %s", key);
        auto value = d[key].GetString();
        log("value\t = %s", value);
    }
    
//    auto ver_str = d["version"].GetString();
    
    return 0;
}

int DownloadScene::WriteVer(const std::string file_name)
{
    auto file = FileUtils::getInstance()->getStringFromFile(file_name);
    
    auto file_to_write = FileUtils::getInstance()->getWritablePath() + file_name;
    std::ofstream outfile;
    outfile.open(file_to_write);
    if (outfile.fail())
    {
        log("Open file %s fail!", file_name.c_str());
        return 1001;
    }
    log("write file:\n----\n%s\n----", file.c_str());
    outfile << file;
    outfile.close();
    return 0;
}


void DownloadScene::Close()
{
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
