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


const auto kDownloadPath = "download";	//下载后保存的文件夹名

AssetsDownloader::AssetsDownloader():
path_to_save_(""),
percent_(0),
status_(),
downloading_(false),
def_pkg_url_("https://github.com/CooloiStudio/Cooloi_Assets_Downloader/raw/master/config/update_list.zip"),
def_ver_url_("http://turanga.deskxd.com/thanks/pubdate/"),
package_url_(""),
version_url_("")
{
    set_version_url(def_ver_url());
}

AssetsDownloader::~AssetsDownloader()
{
    AssetsManager* assetManager = GetAssetManager();
    CC_SAFE_DELETE(assetManager);
}

bool AssetsDownloader::init()
{
    InitDownloadDir();
    return true;
}

void AssetsDownloader::update(float delta)
{
    if (false == downloading())
    {
        
    }
}

void AssetsDownloader::Download(const std::string url)
{
    set_downloading(true);
    set_mode(DownloadMode::kOnce);
    set_package_url(url);
    GetAssetManager()->update();
}

int AssetsDownloader::DownloadMultiple(const std::map<std::string, std::string> pkg_vec)
{
    set_downloading(true);
    set_package_map(pkg_vec);
    set_mode(DownloadMode::kList);
    DownloadUnit();
    return 0;
}

int AssetsDownloader::DownloadUnit()
{
    if (!package_map().empty())
    {
        for (auto p : package_map())
        {
            set_now_downloading(p.first);
            break;
        }
        set_package_url(package_map()[now_downloading()]);
        package_map_.erase(package_map().begin());
        GetAssetManager()->update();
    }
    else
    {
        set_downloading(false);
    }
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
}

void AssetsDownloader::onProgress(int percent)
{
    set_percent(percent);
}

void AssetsDownloader::onSuccess()
{
    log("Download success!");
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
    static AssetsManager *assetManager = NULL;
    
    //    set_version_url(def_ver_url());
    if ("" == package_url())
    {
        set_package_url(def_pkg_url());
    }
    
    if (!assetManager)
    {
        assetManager = new AssetsManager(package_url_.c_str(),
                                         version_url_.c_str(),
                                         path_to_save_.c_str());
        assetManager->setDelegate(this);
        assetManager->setConnectionTimeout(3);
    }
    return assetManager;
}

void AssetsDownloader::InitDownloadDir()
{
    log("InitDownloadDir");
    path_to_save_ = FileUtils::getInstance()->getWritablePath();
    path_to_save_ += kDownloadPath;
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
