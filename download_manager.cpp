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
#include <stdio.h>

using namespace cocos2d;
using namespace CocosDenshion;

#pragma mark - Initialization
DownloadManager::DownloadManager():
downloader_(nullptr),
stage_(DownloadStage ::kNull),
conf_(),
finished_(),
update_(),
now_downloading_(""),
now_number_(0)
{
} // DownloadManager

DownloadManager::~DownloadManager()
{
    if (downloader_)
        CC_SAFE_DELETE(downloader_);
} // ~DownloadManager


// on "init" you need to initialize your instance
bool DownloadManager::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    
    LoadConfig();
    
    InitDownloader();
    
    LoadUpdate();
    
    return true;
} // init

DownloadManager* DownloadManager::Create()
{
    auto d = new DownloadManager();
    if(d->init())
        return d;
    CC_SAFE_DELETE(d);
    return nullptr;
} // Create

void DownloadManager::update(float dt)
{
    if (downloader_->downloading()) return;
    
    unscheduleUpdate();
    
    switch (downloader()->status())
    {
//        case AssetsManager::ErrorCode::CREATE_FILE:
//        {
//            return;
//        }
//            break;
            
        case AssetsManager::ErrorCode::UNCOMPRESS:
        {
            set_stage(DownloadStage::kNull);
            return;
        }
            break;
            
        case AssetsManager::ErrorCode::NETWORK:
        {
            set_stage(DownloadStage::kNull);
            return;
        }
            break;
            
        default:
            break;
    }
    
    switch (stage())
    {
        case DownloadStage ::kLoadUpdate:
        {
            CheckUpdate();
        }
            break;
            
        case DownloadStage ::kGetUpdate:
        {
            GetUpdate();
        }
            break;
            
        default:
            break;
    }
} // update

#pragma mark - Member Function
#pragma mark -stage

int DownloadManager::LoadConfig()
{
    log("\nStage : Load config info.\n");
    set_stage(DownloadStage::kLoadConfig);
    auto ret = 0;
    std::string file_name = "";
#if COCOS2D_DEBUG
    file_name = "Cooloi_ASDL_DEBUG.conf";
//    file_name = "Cooloi_ASDL.conf";
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
} // LoadConfig

int DownloadManager::InitDownloader()
{
    log("\nStage : Initialization Downloader.\n");
    set_stage(DownloadStage::kInitDownloader);
    for (auto c : conf_)
    {
        log("%s", c.first.c_str());
    }
    downloader_ = new AssetsDownloader(conf_["URL"],
                                       conf_["VER"],
                                       conf_["DIR"],
                                       std::stoi(conf_["TRY"]));
    downloader_->Init();
    return 0;
} // InitDownloader

int DownloadManager::LoadUpdate()
{
    log("\nStage : Download update info.\n");
    set_stage(DownloadStage::kLoadUpdate);
    Download(conf_["URL"]);
    return 0;
} // LoadUpdate

int DownloadManager::CheckUpdate()
{
    log("\nStage : Check update info.\n");
    set_stage(DownloadStage::kCheckUpdate);
    auto ret = 0;
    if ("" != now_downloading_) push_finished(now_downloading());
    ret = ReadConf(conf_["NAME"], pkg_map_);
    ret = ReadConf(conf_["LOCAL_NAME"], loc_map_);
    
    for (auto p : pkg_map())
    {
        if (p.second != loc_map_[p.first])
        {
            push_update(p.first);
        }
    }
    
    if (update().empty())
    {
        set_stage(DownloadStage::kFinished);
    }
    else
    {
        log("New package find!");
//        GetUpdate();
    }
    return ret;
} // CheckUpdate

int DownloadManager::GetUpdate()
{
    log("\nStage : Get update package.\n");
    set_stage(DownloadStage::kGetUpdate);
    auto ret = 0;
    if ("" != now_downloading_) push_finished(now_downloading());
    
    for (auto u : update())
    {
        auto jump = false;
        for (auto f : finished())
        {
            if (u == f)
            {
                jump = true;
                break;
            }
        }
        if (jump) continue;
        
        set_now_downloading(u);
        Download(conf().at("SER") + pkg_map_[u]);
        break;
    }
    
    if (update().size() == finished().size())
    {
        std::string content = "#loacl list\n";
        for (auto p : pkg_map())
        {
            content += (p.first + " = " + p.second + "\n");
        }
        WriteFile(conf_["LOCAL_NAME"], content);
        auto s = FileUtils::getInstance()->getStringFromFile(conf_["LOCAL_NAME"]);
        log("%s", s.c_str());
        set_stage(DownloadStage::kFinished);
    }

    return ret;
} // GetUpdate

void DownloadManager::Close()
{
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
} // Close


#pragma mark -other

int DownloadManager::Download(const std::string pkg_url)
{
    log("Request to download resource : %s", pkg_url.c_str());
    downloader_->Download(pkg_url);
    
    scheduleUpdate();
    return 0;
} // Download

int DownloadManager::ReadConf(const std::string file_name,
                              std::map<std::string, std::string> &conf_map)
{
    log("Read config file : %s", file_name.c_str());
    std::string path_with_file = "";
    FindPathWithFile(file_name, path_with_file);
    if ("" == path_with_file)
    {
        WriteFile(file_name, "");
        return 1;
    }
    
    auto str = FileUtils::getInstance()->getStringFromFile(file_name);
    log("getStringFromFile\n%s",str.c_str());
    
    std::ifstream in_file(path_with_file);
    if(in_file.fail())
        return 1001;
    
    std::string str_by_line = "";
    while (std::getline(in_file, str_by_line))
    {
        std::string arg;
        std::string value;
        ConfRegex(str_by_line, arg, value);
        conf_map[arg] = value;
    }
    return 0;
} // ReadConf

int DownloadManager::ConfRegex(const std::string str,
                               std::string &arg,
                               std::string &value)
{
    log("Regex line : %s", str.c_str());
    std::regex rgx("^(\\w+)\\s*\\=\\s*([^\\s]+)$");
    std::smatch match;
    
    if (std::regex_search(str.begin(), str.end(), match, rgx))
    {
        for(auto q : match)
        {
            log("\t%s", q.str().c_str());
        }
        log("----");
        arg = match[1].str();
        value = match[2].str();
    }
    return 0;
} // ConfRegex


int DownloadManager::ReadFile(const std::string file_name,
                              std::string &content)
{
    log("Preparing Read file : %s", file_name.c_str());
    std::string file_with_path = "";
    FindPathWithFile(file_name, file_with_path);
    if ("" == file_with_path)
        return 1404;
    
    log("Full path: %s", file_with_path.c_str());
    auto file = FileUtils::getInstance()->getStringFromFile(file_with_path);
    
    if ("" == file)
    {
        std::ifstream infile(file_with_path);
        
        if (infile.fail())
        {
            log("Open file fail, file name: %s", file_name.c_str());
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
} // ReadFile

int DownloadManager::WriteFile(const std::string file_name,
                               const std::string content)
{
    log("Preparing write file : %s", file_name.c_str());
    std::string file_to_write = "";
    FindPathWithFile(file_name, file_to_write);
    if ("" == file_to_write)
    {
        file_to_write = FileUtils::getInstance()->getWritablePath();
//        file_to_write += conf_["Dir"];
        file_to_write += file_name;
    }
    
    log("Write path : %s", file_to_write.c_str());
    std::ofstream outfile;
    outfile.open(file_to_write);
    if (outfile.fail())
    {
        log("Open file fail, file name: %s", file_name.c_str());
        return 1001;
    }
    std::string str = "#" + file_name + "\n" + content;
    outfile << str;
    log("write file with fstream : \n----\n%s\n----", str.c_str());
    outfile.close();
    return 0;
} // WriteFile

int DownloadManager::AppendFile(const std::string file_name,
                                const std::string content)
{
    auto ret = 0;
    std::string path_with_file = "";
    FindPathWithFile(file_name, path_with_file);
    
    if ("" == path_with_file)
    {
        return 1404;
    }
    log("Full path: %s", path_with_file.c_str());
    
    std::string front_content = "";
    ret = ReadFile(file_name,
                   front_content);
    if (0 != ret)
        return ret;
    
    ret = WriteFile(file_name, front_content + "\n" + content);
    return ret;
} // AppendFile

int DownloadManager::FindPathWithFile(const std::string file_name,
                                      std::string &path_with_file)
{
    auto path = FileUtils::getInstance()->fullPathForFilename(file_name);
    if ("" == path)
    {
        auto temp_name = FileUtils::getInstance()->getWritablePath() + file_name;
        path = FileUtils::getInstance()->fullPathForFilename(temp_name);
        if ("" == path) log("File not exist!");
    }
    if ("" != path)
        log("Find file with : %s", path.c_str());
    path_with_file = path;
    return 0;
} // FindPathWithFile