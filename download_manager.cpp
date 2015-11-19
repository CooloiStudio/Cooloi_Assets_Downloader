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

DownloadManager::DownloadManager():
downloader_()
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
    
    //    downloader_ = new AssetsDownload();
    //    downloader_->init();
    //    Download("");
    
    //    ReadVer("update_list.json");
    //    ReadVer("update_list_b.json");
    std::map<std::string, std::string> conf_map;
    ReadConf(conf_map);
    downloader_ = new AssetsDownloader(conf_map["URL"],
                                       conf_map["VER"],
                                       conf_map["DIR"],
                                       std::stoi(conf_map["TRY"]));
    downloader_->Init();
    
    Download("http://www.deskxd.org:8086/Archive.zip");
    
    //    WriteVer("update_list.json");
    
    //    ReadVer("update_list.json");
    
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
    if (!downloader_->downloading())
    {
        unscheduleUpdate();
//        schedu->unschedule("ca", this);
        
//        ReadVer("update_list.json");
//        ReadVer("update_list_b.json");
        
        
        
        auto path = FileUtils::getInstance()->fullPathForFilename("east.mp3");
        log("%s", path.c_str());
        auto audio = SimpleAudioEngine::getInstance();
        audio->playBackgroundMusic("east.mp3", true);
        
//        sleep(10);
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
}

int DownloadManager::ReadConf(std::map<std::string, std::string> &conf_map)
{
    auto config = FileUtils::getInstance()->fullPathForFilename("Cooloi_ASDL.conf");
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
    std::regex rgx("(\\w+)\\=([^\\s]+)");
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

int DownloadManager::Download(const std::string pkg_url)
{
    std::map<std::string, std::string> pkg_map;
    pkg_map["b"] = "http://www.deskxd.org:8086/update_list_b.zip";
    pkg_map["a"] = "http://www.deskxd.org:8086/update_list.zip";
    downloader_->Download(pkg_url);
    
    scheduleUpdate();
    
    return 0;
}

int DownloadManager::ReadVer(const std::string file_name)
{
    //    log("File name: %s", file_name.c_str());
    auto file_with_path = FileUtils::getInstance()->fullPathForFilename(file_name);
    
    log("Full path: %s", file_with_path.c_str());
    
    auto file = FileUtils::getInstance()->getStringFromFile(file_with_path);
    //    auto file_content = file.c_str();
    
    if ("" == file)
    {
        //        auto file_to_read = FileUtils::getInstance()->getWritablePath()
        //        + "/download/"
        //        + file_name;
        
        auto file_to_read = downloader_->path_to_save() + "/" + file_name;
        
        log("Full path: %s", file_to_read.c_str());
        std::ifstream infile(file_to_read);
        std::string str_by_line = "";
        std::getline(infile, str_by_line);
        
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

int DownloadManager::WriteVer(const std::string file_name)
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


void DownloadManager::Close()
{
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
