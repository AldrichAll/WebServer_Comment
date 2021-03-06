/*************************************************************************
	> File Name: AsyncLogging.h
	> Author: zhangfeng
	> Mail: brave_zephyr@163.com
	> Created Time: Sun 29 Sep 2019 08:26:10 PM CST
	> Target: 实现一个日志类，启动一个线程来写日志 
 ************************************************************************/

#ifndef _ASYNCLOGGING_H
#define _ASYNCLOGGING_H
#include "CountDownLatch.h"
#include "MutexLock.h"
#include "Thread.h"
#include "LogStream.h"
#include "noncopyable.h"
#include <functional>
#include <string>
#include <vector>

class AsyncLogging: noncopyable {
public:
    AsyncLogging(const std::string basename, int flushInterval = 2);
    ~AsyncLogging() {
        if(running_) {
            stop();
        }
    }
    
    void append(const char *logline, int len);

    void start() {
        running_ = true;
        thread_.start();
        latch_.wait();
    }

    void stop() {
        running_ = false;
        cond_.notify();
        thread_.join();
    }

private:
    void threadFunc();
    typedef FixedBuffer<kLargeBuffer> Buffer;
    typedef std::vector<std::shared_ptr<Buffer>> BufferVector;
    typedef std::shared_ptr<Buffer> BufferPtr;
    const int flushInterval_;
    bool running_;
    std::string basename_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;
    BufferPtr currentBuffer_;
    BufferPtr nextBuffer_;
    BufferVector buffers_;
    CountDownLatch latch_;

};

#endif
