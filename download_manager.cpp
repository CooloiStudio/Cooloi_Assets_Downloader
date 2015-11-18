//
//  download_manager.cpp
//  Cooloi_Assets_Downloader
//
//  Created by ESoragoto on 11/17/15.
//
//

#include "download_manager.hpp"
#include "assets_downloader.hpp"

#include <iostream>
#include <string>
#include <fstream>

#include "json/rapidjson.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

using namespace cocos2d;

DownloadManager::DownloadManager():
downloader_()
{
    downloader_ = new AssetsDownloader("https://github.com/CooloiStudio/Cooloi_Assets_Downloader/raw/master/config/update_list.zip",
                                       "http://turanga.deskxd.com/thanks/pubdate/",
                                       "download",
                                       10);
    downloader_->Init();
}

DownloadManager::~DownloadManager()
{
    if (downloader_)
        CC_SAFE_DELETE(downloader_);
}


// on "init" you need to initialize your instance
bool DownloadManager::init()
{
    //    downloader_ = new AssetsDownload();
    //    downloader_->init();
    //    Download("");
    
    //    ReadVer("update_list.json");
    //    ReadVer("update_list_b.json");
    
    
    //    WriteVer("update_list.json");
    
    //    ReadVer("update_list.json");
    
    return true;
}

void DownloadManager::DowanloadDone(float dt)
{
    log("%d", downloader_->status());
    if (!downloader_->downloading())
    {
        auto schedu = Director::getInstance()->getScheduler();
        schedu->unschedule("ca", this);
        
        ReadVer("update_list.json");
        ReadVer("update_list_b.json");
        
        
        
        
        auto file_name_in_app = FileUtils::getInstance()->fullPathForFilename("upload_list_b.json");
        
        log("Full path: %s", file_name_in_app.c_str());
        
        std::ifstream infile(file_name_in_app);
        std::string str_by_line = "";
        while (std::getline(infile, str_by_line))
        {
            log("%s", str_by_line.c_str());
        }
    }
}

int DownloadManager::Download(const std::string pkg_url)
{
    
    std::map<std::string, std::string> pkg_map;
    pkg_map["b"] = "https://github.com/CooloiStudio/Cooloi_Assets_Downloader/raw/master/config/update_list_b.zip";
    pkg_map["a"] = "https://github.com/CooloiStudio/Cooloi_Assets_Downloader/raw/master/config/update_list.zip";
    downloader_->DownloadMultiple(pkg_map);
    
    auto schedu = Director::getInstance()->getScheduler();
    schedu->schedule(CC_CALLBACK_1(DownloadManager::DowanloadDone, this), //回调
                     this, //当前对象
                     0.5, //重复调用时间(s)
                     CC_REPEAT_FOREVER, //重复次数
                     0, //延迟调用时间(s)
                     true, //是否暂停等待
                     "ca"); //key
    
    return 0;
}

int DownloadManager::ReadVer(const std::string file_name)
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
