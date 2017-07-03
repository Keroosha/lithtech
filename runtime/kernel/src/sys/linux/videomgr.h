#ifndef __VIDEOMGR_H__
#define __VIDEOMGR_H__

#include "ltbasedefs.h"

class VideoInst {
public:
    LTRESULT DrawVideo();  // Stub RKJ
    int  GetVideoStatus(); // Stub RKJ
    void Release();   // Stub RKJ
};


class VideoMgr {
public:
    bool CreateScreenVideo(const char *,uint32, VideoInst*);
    CMoArray<VideoInst*> m_Videos;
};

#endif