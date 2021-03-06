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

#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"

using namespace cocos2d;

class AssetsDownloader;

enum class DownloadStage
{
    kNull = 0,
    kLoadConfig = 1,
    kInitDownloader = 2,
    kLoadUpdate = 3,
    kCheckUpdate = 4,
    kGetUpdate = 5,
    kFinished = 6,
    kFileNotFound = 1404
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
    int LoadUpdate();
    int CheckUpdate();
    int GetUpdate();

    void Close();
    
#pragma mark -other
private:
    int Download(const std::string pkg_url);
    
    int ReadConf(const std::string file_name,
                 std::map<std::string, std::string> &conf_map);
    
    int ConfRegex(const std::string str,
                  std::string &arg,
                  std::string &value);
    
    int ReadConfigFromJson(const std::string file_name,
                           std::map<std::string, std::string> &conf_map);
    
    int WriteConfigToJson(const std::string file_name,
                          const std::map<std::string, std::string> conf_map);
    
    int ReadFile(const std::string file_name,
                 std::string &content);
    
    
    int WriteFile(const std::string file_name,
                  const std::string content);
    
    int AppendFile(const std::string file_name,
                   const std::string content);
    
    int FindPathWithFile(const std::string file_name,
                         std::string &path_with_file);
    
    
    void OnHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
#pragma mark - Get&Set
public:
    AssetsDownloader* const downloader() { return downloader_; }
    
    DownloadStage stage() { return stage_; }
    
    const std::vector<std::string> finished() { return finished_; }
    
    const std::vector<std::string> update() { return update_; }
    
protected:
    void set_downloader(AssetsDownloader* downloader) { downloader_ = downloader; }
    
    void set_stage(DownloadStage stage) { stage_ = stage; }
    
    const std::map<std::string, std::string> conf() { return conf_; }
    void push_conf(std::string key, std::string value)
    { conf_[key] = value; }
    
    
    const std::map<std::string, std::string> pkg_map() { return pkg_map_; }
    void push_pkg_map(std::string key, std::string value)
    { pkg_map_[key] = value; }
    
    
    const std::map<std::string, std::string> loc_map() { return loc_map_; }
    void push_loc_map(std::string key, std::string value)
    { loc_map_[key] = value; }
    
    void push_finished(std::string finished)
    { finished_.push_back(finished); }
    
    void push_update(std::string update)
    { update_.push_back(update); }
    
    const std::string now_downloading() { return now_downloading_; }
    void set_now_downloading(std::string now) { now_downloading_ = now; }
    
    int now_number() { return now_number_; }
    void set_now_number(int num) { now_number_ = num; }
    
#pragma mark - Variable
private:
    AssetsDownloader *downloader_;
    DownloadStage stage_;
    
    std::map<std::string, std::string> conf_;
    std::map<std::string, std::string> pkg_map_;
    std::map<std::string, std::string> loc_map_;
    
    std::vector<std::string> finished_;
    std::vector<std::string> update_;
    std::string now_downloading_;
    
    int now_number_;
};

#endif /* download_manager_hpp */
