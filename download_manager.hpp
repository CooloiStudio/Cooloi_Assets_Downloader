//
//  download_manager.hpp
//  Cooloi_Assets_Downloader
//
//  Created by ESoragoto on 11/17/15.
//
//

#ifndef download_manager_hpp
#define download_manager_hpp

#include "cocos2d.h"

class AssetsDownload;
class AssetsDownloader;

class DownloadManager : public cocos2d::Scene
{
public:
    ///
    /// \brief 构造函数
    ///
    /// \param update_info_pkg_url 包含update_list.json资源包的url
    /// \param update_ver_url      版本号(为了保证每次都会下载，请使用时间戳冒充版本号)
    /// \param download_dir        资源下载后的存放路径
    /// \param max_retry           下载出错允许的最大重试次数
    ///
    DownloadManager();
    ~DownloadManager();
    
    virtual bool init();
    static DownloadManager* Create();
    
    virtual void update(float dt);
    
    int ReadConf(std::map<std::string, std::string> &conf_map);
    int ConfRegex(const std::string str,
                 std::string &arg,
                 std::string &value);
    
    int Download(const std::string pkg_url);
    
    int ReadVer(const std::string file_name);
    
    int WriteVer(const std::string file_name);
    
    void Close();
    
    
#pragma mark - Get&Set
public:
    AssetsDownloader* const downloader() { return downloader_; }
    
protected:
    
#pragma mark - Variable
private:
    AssetsDownloader *downloader_;
};

#endif /* download_manager_hpp */
