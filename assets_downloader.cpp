//
//  assets_downloader.cpp
//  Cooloi_Assets_Downloader
//
//  Created by ESoragoto on 11/16/15.
//
//

#include "assets_downloader.hpp"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <dirent.h>
#include <sys/stat.h>
#endif

AssetsDownloader::AssetsDownloader(const std::string def_pkg_url,
                                   const std::string def_ver_url,
                                   const std::string download_dir,
                                   int max_retry):
retry_(0),
max_retry_(max_retry),
percent_(0),
dir_to_save_(download_dir),
path_to_save_(""),
status_(),
downloading_(false),
def_pkg_url_(def_pkg_url),
def_ver_url_(def_ver_url),
package_url_(""),
version_url_(def_ver_url)
{
}

AssetsDownloader::~AssetsDownloader()
{
    AssetsManager* asset_manager = GetAssetManager();
    CC_SAFE_DELETE(asset_manager);
}

bool AssetsDownloader::Init()
{
    InitDownloadDir();
    return true;
}

int AssetsDownloader::Download(const std::string url)
{
    set_downloading(true);
    set_mode(DownloadMode::kOnce);
    set_now_downloading(url);
    return DoDownload(url);
}

int AssetsDownloader::DownloadMultiple(const std::map<std::string, std::string> pkg_map)
{
    set_downloading(true);
    set_package_map(pkg_map);
    set_mode(DownloadMode::kList);
    return DownloadUnit();
}

int AssetsDownloader::DownloadUnit()
{
    if (!package_map().empty())
    {
        
        if (AssetsManager::ErrorCode::NETWORK == status())
        {
            DoDownload(package_map().at(now_downloading()));
            return 0;
        }
        std::vector<std::string> finished;
        for (auto p : package_map())
        {
            if(now_downloading() == p.first
               ||
               package_url() == package_map().at(p.first))
            {
                finished.push_back(p.first);
                continue;
            }
            set_now_downloading(p.first);
            break;
        }
        for (auto f : finished)
        {
            package_map_.erase(f);
        }
        if (package_map().empty())
        {
            set_downloading(false);
            return 0;
        }
        DoDownload(package_map().at(now_downloading()));
    }
    else
    {
        set_downloading(false);
    }
    return 0;
}

int AssetsDownloader::DoDownload(const std::string url)
{
    log("Now downloading : %s", now_downloading().c_str());
    
    if (AssetsManager::ErrorCode::NETWORK != status())
    {
        set_retry(0);
    }
    set_package_url(url);
    GetAssetManager()->update();
    set_status(AssetsManager::ErrorCode::CREATE_FILE);
    return 0;
}

int AssetsDownloader::Reset()
{
    // Remove downloaded files
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    
    std::string command = "rm -r ";
    // Path may include space.
    command += "\"" + path_to_save_ + "\"";
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    system(command.c_str());
#pragma GCC diagnostic pop
    
#else
    std::string command = "rd /s /q ";
    // Path may include space.
    command += "\"" + path_to_save_ + "\"";
    system(command.c_str());
#endif
    GetAssetManager()->deleteVersion();
    InitDownloadDir();
    
    return 0;
}

void AssetsDownloader::onError(AssetsManager::ErrorCode errorCode)
{
    set_status(errorCode);
    log("Error on Downloading : %s\nError code : %d",
        now_downloading().c_str(),
        status());
     if (errorCode == AssetsManager::ErrorCode::NETWORK)
    {
        if (max_retry() <= retry())
        {
            set_retry(0);
            return;
        }
        log("Retry download");
        set_retry(retry() + 1);
        log("Retry : %d", retry());
        DownloadUnit();
    }
}

void AssetsDownloader::onProgress(int percent)
{
    set_percent(percent);
}

void AssetsDownloader::onSuccess()
{
    log("Download finished with : %s", now_downloading().c_str());
    set_now_downloading("");
    switch (mode())
    {
        case DownloadMode::kOnce:
        {
            set_downloading(false);
        }
            break;
            
        case DownloadMode::kList:
        {
            DownloadUnit();
        }
            break;
            
        default:
            break;
    }
}

AssetsManager* AssetsDownloader::GetAssetManager()
{
    static AssetsManager *asset_manager = NULL;
    
    //    set_version_url(def_ver_url());
    if ("" == package_url())
    {
        set_package_url(def_pkg_url());
    }
    
    if (asset_manager)
    {
        asset_manager->setPackageUrl(package_url().c_str());
    }
    
    if (!asset_manager)
    {
        asset_manager = new AssetsManager(package_url_.c_str(),
                                          version_url_.c_str(),
                                          path_to_save_.c_str());
        asset_manager->setDelegate(this);
        asset_manager->setConnectionTimeout(3);
    }
    return asset_manager;
}

void AssetsDownloader::InitDownloadDir()
{
    log("InitDownloadDir");
    path_to_save_ = FileUtils::getInstance()->getWritablePath();
    path_to_save_ += dir_to_save_;
    log("Path: %s", path_to_save_.c_str());
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    DIR *pDir = NULL;
    pDir = opendir(path_to_save_.c_str());
    if (!pDir)
    {
        mkdir(path_to_save_.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
    }
#else
    if ((GetFileAttributesA(path_to_save_.c_str())) == INVALID_FILE_ATTRIBUTES)
    {
        CreateDirectoryA(path_to_save_.c_str(), 0);
    }
#endif
    log("InitDownloadDir end");
}
