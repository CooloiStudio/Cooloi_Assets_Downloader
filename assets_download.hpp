//
//  assets_download.hpp
//  Turanga
//
//  Created by ZhongHan on 11/11/15.
//
//

#ifndef assets_download_hpp
#define assets_download_hpp

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

class AssetsDownload : public cocos2d::extension::AssetsManagerDelegateProtocol
{
public:
    AssetsDownload();
    virtual ~AssetsDownload();
    
    virtual bool init();
    
    
    void Download();	//检查版本更新
    int Reset();		//重置版本
    
    virtual void onError(cocos2d::extension::AssetsManager::ErrorCode errorCode);		//错误信息
    virtual void onProgress(int percent);	//更新下载进度
    virtual void onSuccess();		//下载成功
private:
    cocos2d::extension::AssetsManager* getAssetManager();
    void initDownloadDir();		//创建下载目录
    
#pragma mark - Get&Set
public:
    
    int percent() {return percent_;}
    cocos2d::extension::AssetsManager::ErrorCode status() {return status_;}
    bool success() {return success_;}
    
    std::string package_url() { return package_url_; }
    void set_package_url(std::string package_url) { package_url_ = package_url; }
    
    std::string version_url() { return version_url_; }
    void set_version_url(std::string version_url) { version_url_ = version_url; }
    
protected:
    void set_percent(int percent) { percent_ = percent; }
    void set_status(const cocos2d::extension::AssetsManager::ErrorCode stauts) { status_ = stauts; }
    void set_success(bool is_success) {success_ = is_success;}

#pragma mark - Variable
private:
    int percent_;
    std::string path_to_save_;
    cocos2d::extension::AssetsManager::ErrorCode status_;
    bool success_;
    
    std::string package_url_;
    std::string version_url_;
};

#endif /* assets_download_hpp */
