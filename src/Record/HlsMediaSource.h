/*
 * MIT License
 *
 * Copyright (c) 2016-2019 xiongziliang <771730766@qq.com>
 *
 * This file is part of ZLMediaKit(https://github.com/xiongziliang/ZLMediaKit).
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef ZLMEDIAKIT_HLSMEDIASOURCE_H
#define ZLMEDIAKIT_HLSMEDIASOURCE_H

#include <atomic>
#include "Common/MediaSource.h"
namespace mediakit{

class HlsCookieData{
public:
    HlsCookieData(const MediaInfo &info);
    ~HlsCookieData();
    void addByteUsage(uint64_t bytes);
private:
    uint64_t _bytes = 0;
    MediaInfo _info;
};

class HlsMediaSource : public MediaSource {
public:
    friend class HlsCookieData;
    typedef std::shared_ptr<HlsMediaSource> Ptr;
    HlsMediaSource(const string &vhost,
                   const string &app,
                   const string &stream_id) :
            MediaSource(HLS_SCHEMA, vhost, app, stream_id){
        _readerCount = 0;
    }

    virtual ~HlsMediaSource() = default;

    /**
	 * 获取播放器个数
	 * @return
	 */
    int readerCount() override {
        return _readerCount.load();
    }

    /**
     * 注册hls
     */
    void registHls(){
        if(!_registed){
            regist();
            _registed = true;
        }
    }

private:
    /**
     * 修改观看者个数
     * @param add 添加海思删除
     */
    void modifyCount(bool add) {
        if (add) {
            ++_readerCount;
            return;
        }

        if (--_readerCount == 0 && totalReaderCount() == 0) {
            onNoneReader();
        }
    }
private:
    atomic_int _readerCount;
    bool _registed = false;
};


}//namespace mediakit
#endif //ZLMEDIAKIT_HLSMEDIASOURCE_H