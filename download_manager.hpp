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

enum class DownloadStage
{
    kNull = 0,
    kLoadConfig = 1,
    kInitDownloader = 2,
    kDownloadUpdate = 3,
    kCheckUpdate = 4,
    kFinished = 5
};

class DownloadManager : public cocos2d::Scene
{
#pragma mark - Initialization
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
    
#pragma mark - Member Function
    
#pragma mark -stage
public:
    int LoadConfig();
    int InitDownloader();
    int DownloadUpdate();
    int CheckUpdate();

    void Close();
    
#pragma mark -other
private:
    int Download(const std::string pkg_url);
    
    int ReadConf(const std::string file_name,
                 std::map<std::string, std::string> &conf_map);
    int ConfRegex(const std::string str,
                  std::string &arg,
                  std::string &value);
    
    int ReadFile(const std::string file_name,
                 std::string &content);
    
    int WriteFile(const std::string file_name,
                  const std::string content);
    
    int AppendFile(const std::string file_name,
                   const std::string content);
    
#pragma mark - Get&Set
public:
    AssetsDownloader* const downloader() { return downloader_; }
    
protected:
    void set_downloader(AssetsDownloader* downloader) { downloader_ = downloader; }
    
    DownloadStage stage() { return stage_; }
    void set_stage(DownloadStage stage) { stage_ = stage; }
    
    std::map<std::string, std::string> conf() { return conf_; }
    void push_conf(std::string key, std::string value)
    { conf_[key] = value; }
    
    std::vector<std::string> finished() { return finished_; }
    void push_finished(std::string finished)
    { finished_.push_back(finished); }
    
#pragma mark - Variable
private:
    AssetsDownloader *downloader_;
    DownloadStage stage_;
    
    std::map<std::string, std::string> conf_;
    std::vector<std::string> finished_;
};

#endif /* download_manager_hpp */
