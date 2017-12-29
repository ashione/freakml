#ifndef __FREAK_THREAD_POOL_JOB_H__
#define __FREAK_THREAD_POOL_JOB_H__

#include <freakThread.h>

#include <vector>
#include <string>
#include <queue>

namespace freak {

typedef class FreakThreadPoolJob* FreakThreadPoolJobPtr;

typedef void (*freakThreadPoolJobFunc)(void *);
typedef void (*freakThreadPoolJobCleanUpFunc)(void *);

class FreakThreadPoolJob {

    private :
        std::string poolJobName;
        freakThreadPoolJobFunc func;
        freakThreadPoolJobCleanUpFunc cleanFunc;
        void* data;

    public :
        FreakThreadPoolJob(std::string,freakThreadPoolJobFunc,freakThreadPoolJobCleanUpFunc,void*);
        void run();
        ~FreakThreadPoolJob();

};

typedef class FreakThreadPool* FreakThreadPoolPtr;

class FreakThreadPool {

    private :
        FreakMutex poolLock;
        FreakCond poolCond;
        FreakCond poolQuitCond;

        size_t minWorkers;
        size_t maxWorkers;
        size_t freeWorkers;

        std::vector<FreakThreadPtr> workers;
        std::queue<FreakThreadPoolJobPtr> jobList;
        bool quit;

    public :
        FreakThreadPool(size_t,size_t);
        bool insertJob(FreakThreadPoolJobPtr);
        size_t currentWorkerSize();
        size_t freeWorkerSize();
        void removeThread(size_t);
        ~FreakThreadPool();

        static void run(void*);


    private :

        void expandWorkers(size_t);

};

}

#endif /* __FREAK_THREAD_POOL_JOB_H__ */

