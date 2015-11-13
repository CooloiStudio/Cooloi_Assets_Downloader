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
    
    int ReadVer(const std::string file_name);
    
    int WriteVer(const std::string file_name);
    
    void Close();
    
    // implement the "static create()" method manually
    CREATE_FUNC(DownloadScene);
    
    
#pragma mark - Variable
private:
    AssetsDownload *downloader_;
};

#endif /* DownloadScene_hpp */
