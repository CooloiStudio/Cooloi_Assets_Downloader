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
    
    WriteVer("test.json");
    
    ReadVer();
    
    
    return true;
}


int DownloadScene::ReadVer()
{
    auto name = "test.json";
    auto file = FileUtils::getInstance()->getStringFromFile(name);
    auto file_content = file.c_str();
    
    if ("" == file)
    {
        auto file_to_read = FileUtils::getInstance()->getWritablePath() + name;
        std::ifstream infile(file_to_read);
        
        std::string str((std::istreambuf_iterator<char>(infile)),
                        std::istreambuf_iterator<char>());
        file_content = str.c_str();
    }
    
    
    log("open file:\n----\n%s\n----", file_content);
    
    rapidjson::Document d;
    d.Parse<0>(file_content);
    
    if (d.HasMember("a"))
    {
        auto a = d["a"].GetString();
        log("a = %s", a);
    }
    
    for (auto iter = d.MemberBegin(); iter != d.MemberEnd(); iter++)
    {
        auto key = (iter->name).GetString();
        log("key = %s", key);
        auto value = d[key].GetString();
        log("value = %s", value);
    }
    
//    auto ver_str = d["version"].GetString();
    
    return 0;
}

int DownloadScene::WriteVer(std::string file_name)
{
    auto file = FileUtils::getInstance()->getStringFromFile("version.json");
    
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
