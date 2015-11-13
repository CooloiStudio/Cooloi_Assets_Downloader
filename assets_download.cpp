//
//  assets_download.cpp
//  Turanga
//
//  Created by ZhongHan on 11/11/15.
//
//

#include "assets_download.hpp"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <dirent.h>
#include <sys/stat.h>
#endif

USING_NS_CC;
USING_NS_CC_EXT;

const auto kDownloadPath = "download";	//下载后保存的文件夹名

AssetsDownload::AssetsDownload():
path_to_save_(""),
percent_(0),
status_(),
success_(false),
package_url_(""),
version_url_("time.nist.gov")
{
}

AssetsDownload::~AssetsDownload()
{
    AssetsManager* assetManager = getAssetManager();
    CC_SAFE_DELETE(assetManager);
}

bool AssetsDownload::init()
{
    initDownloadDir();
    getAssetManager()->update();
    
    return true;
}

void AssetsDownload::Download()
{
    getAssetManager()->update();
}

int AssetsDownload::Reset()
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
    getAssetManager()->deleteVersion();
    initDownloadDir();
    
    return 0;
}

void AssetsDownload::onError(AssetsManager::ErrorCode errorCode)
{
    set_status(errorCode);
}

void AssetsDownload::onProgress(int percent)
{
    set_percent(percent);
}

void AssetsDownload::onSuccess()
{
    CCLOG("download success");
    set_success(true);
}

AssetsManager* AssetsDownload::getAssetManager()
{
    static AssetsManager *assetManager = NULL;
    
    if ("" == package_url_ || "" == version_url_)
    {
        return nullptr;
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

void AssetsDownload::initDownloadDir()
{
    CCLOG("initDownloadDir");
    path_to_save_ = FileUtils::getInstance()->getWritablePath();
    path_to_save_ += kDownloadPath;
    CCLOG("Path: %s", path_to_save_.c_str());
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
    CCLOG("initDownloadDir end");
}
