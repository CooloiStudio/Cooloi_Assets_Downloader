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

enum class DownloadMode
{
    kNull = 0,
    kOnce = 1,
    kList = 2
};

class AssetsDownloader : public Node, public AssetsManagerDelegateProtocol
{
public:
    AssetsDownloader();
    virtual ~AssetsDownloader();
    
    virtual bool init();
    
    void update(float dt);
    
    void Download(const std::string url);
    int DownloadMultiple(const std::map<std::string, std::string> pkg_vec);
    int Reset();		//重置
    
    virtual void onError(AssetsManager::ErrorCode errorCode);		//错误信息
    virtual void onProgress(int percent);	//更新下载进度
    virtual void onSuccess();		//下载成功
private:
    
    int DownloadUnit();
    AssetsManager* GetAssetManager();
    void InitDownloadDir();		//创建下载目录
    
#pragma mark - Get&Set
public:
    
    int percent() {return percent_;}
    
    std::string path_to_save() { return path_to_save_; }
    
    AssetsManager::ErrorCode status() {return status_;}
    
    bool downloading() {return downloading_;}
    
    DownloadMode mode() { return mode_; }
    void set_mode(DownloadMode mode) { mode_ = mode; }
    
    std::string package_url() { return package_url_; }
    void set_package_url(std::string package_url) { package_url_ = package_url; }
    
    std::string version_url() { return version_url_; }
    void set_version_url(std::string version_url) { version_url_ = version_url; }
    
    std::string now_downloading() { return now_downloading_; }
    
protected:
    void set_percent(int percent) { percent_ = percent; }
    
    void set_status(const AssetsManager::ErrorCode stauts) { status_ = stauts; }
    
    void set_downloading(bool is_downloading) {downloading_ = is_downloading;}
    
    const std::string def_pkg_url() { return def_pkg_url_; }
    void set_def_pkg_url(std::string def_pkg_url) { def_pkg_url_ = def_pkg_url;}
    
    const std::string def_ver_url() { return def_ver_url_; }
    void set_def_ver_url(std::string def_ver_url) { def_ver_url_ = def_ver_url; }
    
    void set_now_downloading(std::string now_downloading)
    { now_downloading_ = now_downloading; }
    
    std::map<std::string, std::string> package_map() { return package_map_; }
    void set_package_map(std::map<std::string, std::string> package_map)
    { package_map_ = package_map; }
    
#pragma mark - Variable
private:
    int percent_;
    std::string path_to_save_;
    AssetsManager::ErrorCode status_;
    bool downloading_;
    DownloadMode mode_;
    
    std::string def_pkg_url_;
    std::string def_ver_url_;
    std::string package_url_;
    std::string version_url_;
    
    std::string now_downloading_;
    
    std::map<std::string, std::string> package_map_;
};

#endif /* assets_downloader_hpp */
