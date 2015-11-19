//
//  download_manager.cpp
//  Cooloi_Assets_Downloader
//
//  Created by ESoragoto on 11/17/15.
//
//

#include "download_manager.hpp"
#include "assets_downloader.hpp"

#include "SimpleAudioEngine.h"

#include <iostream>
#include <string>
#include <fstream>
#include <regex>

#include "json/rapidjson.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

using namespace cocos2d;
using namespace CocosDenshion;

#pragma mark - Initialization
DownloadManager::DownloadManager():
downloader_(nullptr),
stage_(DownloadStage ::kNull),
conf_(),
finished_()
{
}

DownloadManager::~DownloadManager()
{
    if (downloader_)
        CC_SAFE_DELETE(downloader_);
}


// on "init" you need to initialize your instance
bool DownloadManager::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto label = Label::createWithSystemFont("", "Arail", 30);
    
    std::string label_str = "This is ";
    
#if COCOS2D_DEBUG
    label_str += "debug";
#else
    label_str += "release";
#endif
    
    label->setString(label_str);
    label->setPosition(Vec2(origin.x + visibleSize.width - label->getContentSize().width/2,
                            origin.y + label->getContentSize().height/2));
    addChild(label);
    
    //    downloader_ = new AssetsDownload();
    //    downloader_->init();
    //    Download("");
    
    //    ReadVer("update_list.json");
    //    ReadVer("update_list_b.json");
    
    
    InitDownloader();
    
    DownloadUpdate();
    
    return true;
}

DownloadManager* DownloadManager::Create()
{
    auto d = new DownloadManager();
    if(d->init())
        return d;
    CC_SAFE_DELETE(d);
    return nullptr;
}

void DownloadManager::update(float dt)
{
    if (downloader_->downloading()) return;
    
    unscheduleUpdate();
    
    switch (stage_)
    {
        case DownloadStage ::kDownloadUpdate:
        {
            CheckUpdate();
        }
            break;
            
        case DownloadStage ::kCheckUpdate:
        {
            CheckUpdate();
        }
            break;
            
        default:
            break;
    }
    
    auto path = FileUtils::getInstance()->fullPathForFilename("east.mp3");
    log("%s", path.c_str());
    auto audio = SimpleAudioEngine::getInstance();
    audio->playBackgroundMusic("east.mp3", true);
    
    sleep(10);
    auto file_with_path = FileUtils::getInstance()->fullPathForFilename("AnimationLoading.json");
    
    log("Full path: %s", file_with_path.c_str());
    
    if ("" == file_with_path)
        return;
    
    std::ifstream infile(file_with_path);
    std::string str_by_line = "";
    while (std::getline(infile, str_by_line))
    {
        log("%s", str_by_line.c_str());
    }
}

#pragma mark - Member Function
#pragma mark -stage

int DownloadManager::LoadConfig()
{
    log("Stage : Load config info.");
    set_stage(DownloadStage::kLoadConfig);
    auto ret = 0;
    std::string file_name = "";
#if COCOS2D_DEBUG
    file_name = "Cooloi_ASDL.conf";
#else
    file_name = "Cooloi_ASDL.conf";
#endif
    ret = ReadConf(file_name,
                   conf_);
    if (0 != ret)
    {
        set_stage(DownloadStage::kNull);
    }
    return ret;
}

int DownloadManager::InitDownloader()
{
    log("Stage : Initialization Downloader.");
    set_stage(DownloadStage::kInitDownloader);
    downloader_ = new AssetsDownloader(conf_["URL"],
                                       conf_["VER"],
                                       conf_["DIR"],
                                       std::stoi(conf_["TRY"]));
    downloader_->Init();
    return 0;
}

int DownloadManager::DownloadUpdate()
{
    log("Stage : Download update info.");
    set_stage(DownloadStage::kDownloadUpdate);
    Download(conf_["URL"]);
    return 0;
}

int DownloadManager::CheckUpdate()
{
    log("Stage : Check update info.");
    set_stage(DownloadStage::kCheckUpdate);
    auto ret = 0;
    std::map<std::string, std::string> pkg_map;
    ret = ReadConf(conf_["NAME"], pkg_map);
    std::map<std::string, std::string> loc_map;
    ret = ReadConf(conf_["LOCAL_NAME"], loc_map);
    
    for (auto n : pkg_map)
    {
        auto iter = std::find(finished().begin(),
                              finished().end(),
                              n.first);
        if (iter != finished().end())
            continue;
        if (n.second != loc_map[n.first])
        {
            Download(conf_["SER"] + n.second);
            break;
        }
    }
    set_stage(DownloadStage::kFinished);
    return ret;
}

void DownloadManager::Close()
{
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


#pragma mark -other

int DownloadManager::Download(const std::string pkg_url)
{
    log("Request to download resource : %s", pkg_url.c_str());
    downloader_->Download(pkg_url);
    
    scheduleUpdate();
    return 0;
}

int DownloadManager::ReadConf(const std::string file_name,
                              std::map<std::string, std::string> &conf_map)
{
    auto config = FileUtils::getInstance()->fullPathForFilename(file_name);
    
    while ("" == config)
    {
        WriteFile(file_name, "");
        return 0;
    }
    std::ifstream in_file(config);
    
    
    std::string str_by_line = "";
    while (std::getline(in_file, str_by_line))
    {
        std::string arg;
        std::string value;
        ConfRegex(str_by_line, arg, value);
        conf_map[arg] = value;
    }
    return 0;
}

int DownloadManager::ConfRegex(const std::string str,
                               std::string &arg,
                               std::string &value)
{
    std::regex rgx("^(\\w+)\\s*\\=\\s*([^\\s]+)$");
    std:: smatch match;
    
    if (std::regex_search(str.begin(), str.end(), match, rgx))
    {
        for(auto q : match)
        {
            log("%s", q.str().c_str());
        }
        arg = match[1].str();
        value = match[2].str();
    }
    return 0;
}


int DownloadManager::ReadFile(const std::string file_name,
                              std::string &content)
{
    auto file_with_path = FileUtils::getInstance()->fullPathForFilename(file_name);
    
    if ("" == file_name)
        return 1404;
    
    log("Full path: %s", file_with_path.c_str());
    auto file = FileUtils::getInstance()->getStringFromFile(file_with_path);
    
    if ("" == file)
    {
        log("Full path: %s", file_with_path.c_str());
        std::ifstream infile(file_with_path);
        
        if (infile.fail())
        {
            log("Open file fail");
            return 1001;
        }
        
        std::string str((std::istreambuf_iterator<char>(infile)),
                        std::istreambuf_iterator<char>());
        file = str.c_str();
        infile.close();
    }
    
    log("open file:\n----\n%s\n----", file.c_str());
    content = file;
    return 0;
}

int DownloadManager::WriteFile(const std::string file_name,
                               const std::string content)
{
    auto file_to_write = FileUtils::getInstance()->getWritablePath() + file_name;
    std::ofstream outfile;
    outfile.open(file_to_write);
    if (outfile.fail())
    {
        log("Open file %s fail!", file_name.c_str());
        return 1001;
    }
    log("write file:\n----\n%s\n----", content.c_str());
    
    outfile << content;
    outfile.close();
    
    return 0;
}

int DownloadManager::AppendFile(const std::string file_name,
                                const std::string content)
{
    auto ret = 0;
    auto file = FileUtils::getInstance()->fullPathForFilename(file_name);
    log("Full path: %s", file.c_str());
    
    std::string front_content = "";
    ret = ReadFile(file_name,
                   front_content);
    if (0 != ret)
        return ret;
    
    ret = WriteFile(file_name, front_content + "\n" + content);
    return ret;
}