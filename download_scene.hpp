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

class DownloadScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    int ReadVer(std::string file_name);
    
    int WriteVer(std::string file_name);
    
    void Close();
    
    // implement the "static create()" method manually
    CREATE_FUNC(DownloadScene);
    
    
#pragma mark - Variable
private:
};

#endif /* DownloadScene_hpp */
