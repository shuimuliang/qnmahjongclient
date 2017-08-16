//
//  LoadingLayerLoader.h
//  Casino
//
//  Created by liulihua on 15/3/25.
//
//

#ifndef Casino_LoadingLayerLoader_h
#define Casino_LoadingLayerLoader_h


#include "LoadingLayer.h"

/* Forward declaration. */
class CCBReader;

class LoadingLayerLoader : public cocos2d::extension::CCLayerLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(LoadingLayerLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(LoadingLayer);
};



#endif
