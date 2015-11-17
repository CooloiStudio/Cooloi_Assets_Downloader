//
//  download_scene.hpp
//  Test
//
//  Created by ESoragoto on 11/12/15.
//
//

#ifndef download_scene_hpp
#define download_scene_hpp

#include "cocos2d.h"

class AssetsDownload;

class DownloadScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    virtual void update(float dt);
    
    void DowanloadDone(float dt);
    
    int Download(const std::string pkg_url);
    
    int ReadVer(const std::string file_name);
    
    int WriteVer(const std::string file_name);
    
    void Close();
    
    // implement the "static create()" method manually
    CREATE_FUNC(DownloadScene);
    
    
#pragma mark - Get&Set
public:
    bool dl_finished() { return dl_finished_; }
    
protected:
    void set_dl_finished(bool dl_finished) { dl_finished_ = dl_finished; }
    
#pragma mark - Variable
private:
    AssetsDownload *downloader_;
    
    bool dl_finished_;
};

#endif /* DownloadScene_hpp */
