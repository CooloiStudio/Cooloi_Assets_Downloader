//
//  assets_downloader.hpp
//  Cooloi_Assets_Downloader
//
//  Created by ESoragoto on 11/16/15.
//
//

#ifndef assets_downloader_hpp
#define assets_downloader_hpp

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class AssetsDownloader : public AssetsManagerDelegateProtocol
{
public:
    
    ///
    /// \brief 构造函数
    ///
    /// \param def_pkg_url  默认资源url
    /// \param def_ver_url  默认版本信息url
    /// \param download_dir 保持下载资源的本地文件夹
    /// \param max_retry    最大下载重试次数
    ///
    AssetsDownloader(const std::string def_pkg_url,
                     const std::string def_ver_url,
                     const std::string download_dir,
                     int max_retry);
    
    virtual ~AssetsDownloader();
    
    virtual bool Init();
    
    ///
    /// \brief 单资源下载
    ///
    /// \param url 资源url
    ///
    /// \return 0
    ///
    int Download(const std::string url);
    
    int Reset();
    
    int Delet();
    
    virtual void onError(AssetsManager::ErrorCode errorCode);
    virtual void onProgress(int percent);
    virtual void onSuccess();
    
private:
    int DoDownload(const std::string url);
    AssetsManager* GetAssetManager();
    void InitDownloadDir();
    
#pragma mark - Get&Set
public:
    
    const unsigned int percent() {return percent_;}
    
    const std::string path_to_save() { return path_to_save_; }
    
    AssetsManager::ErrorCode status() {return status_;}
    
    bool downloading() { return downloading_; }
    
    const std::string package_url() { return package_url_; }
    void set_package_url(std::string package_url) { package_url_ = package_url; }
    
    const std::string version_url() { return version_url_; }
    void set_version_url(std::string version_url) { version_url_ = version_url; }
    
    const std::string now_downloading() { return now_downloading_; }
    
protected:
    const unsigned int retry() { return retry_; }
    void set_retry(unsigned int retry) { retry_ = retry; }
    
    const unsigned int max_retry() { return max_retry_; }
    void set_max_retry(unsigned int max) { max_retry_ = max; }
    
    void set_percent(unsigned int percent) { percent_ = percent; }
    
    void set_status(const AssetsManager::ErrorCode stauts) { status_ = stauts; }
    
    void set_downloading(bool is_downloading) {downloading_ = is_downloading;}
    
    void set_now_downloading(std::string now_downloading)
    { now_downloading_ = now_downloading; }
    
    const std::map<std::string, std::string> package_map() { return package_map_; }
    void set_package_map(std::map<std::string, std::string> package_map)
    { package_map_ = package_map; }
    
#pragma mark - Variable
private:
    unsigned int retry_;
    unsigned int max_retry_;
    unsigned int percent_;
    std::string dir_to_save_;
    std::string path_to_save_;
    AssetsManager::ErrorCode status_;
    bool downloading_;
    
    std::string package_url_;
    std::string version_url_;
    
    std::string now_downloading_;
    
    std::map<std::string, std::string> package_map_;
};

#endif /* assets_downloader_hpp */
