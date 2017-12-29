#include <freakThread.h>
#include <freakThreadPool.h>
#include <freakException.h>

#include <freakConfig.h>

namespace freak {

FreakThreadPoolJob::FreakThreadPoolJob(std::string jobName,
                                       freakThreadPoolJobFunc jobFunc,
                                       freakThreadPoolJobCleanUpFunc jobCleanFunc,
                                       void* jobData) :
poolJobName(jobName),func(jobFunc),cleanFunc(jobCleanFunc),data(jobData){
    CHECK_NOTNULL(func);
}

void
FreakThreadPoolJob::run()
{
    func(data);
}

FreakThreadPoolJob::~FreakThreadPoolJob()
{
    if (cleanFunc != NULL)
        cleanFunc(data);
}

FreakThreadPool::FreakThreadPool(size_t minWorkers_,
                                 size_t maxWorkers_)
{
    CHECK_GE(minWorkers_,1);
    CHECK_GE(maxWorkers_,minWorkers_);

    maxWorkers = maxWorkers_;
    minWorkers = minWorkers_;
    freeWorkers = 0;
    int ret = poolLock.init();
    ret &= poolCond.init();
    ret &= poolQuitCond.init();

    if ( ret ) {
        throw FreakThreadPoolInitException("poolLock or poolCond init failed");
    }

    quit = false;

    expandWorkers(minWorkers);

}

void
FreakThreadPool::expandWorkers(size_t gain)
{
    if ( gain + currentWorkerSize() > maxWorkers ) {
        throw FreakThreadPoolExpandException("pool expand exception");
    }

    for (size_t i=0; i<gain; ++i) {
        FreakThreadPtr worker =  new FreakThread(true,FreakThreadPool::run,"Pool",true,this);
        workers.emplace_back(worker);
    }

}

void
FreakThreadPool::run(void* rawPool)
{
    CHECK_NOTNULL(rawPool);
    FreakThreadPoolPtr pool = static_cast<FreakThreadPoolPtr>(rawPool);
    pool->poolLock.lock();

    while (1) {
        while( !pool->quit && pool->jobList.size() <= 0 ) {
            pool->freeWorkers++;

            if ( pool->poolCond.wait(&pool->poolLock) < 0 ) {
                pool->freeWorkers--;
                goto out;
            }
        }

        if (pool->quit) {
            pool->freeWorkers--;
            break;
        }

        FreakThreadPoolJobPtr job = pool->jobList.front();
        pool->jobList.pop();
        pool->poolLock.unLock();

        job->run();
        delete job;

        pool->poolLock.lock();
    }

out :

    for (size_t i=0; i<pool->currentWorkerSize(); i++) {
        if (pool->workers[i]->isSelf()) {
            pool->removeThread(i);
        }
    }

    if (pool->currentWorkerSize() == 0 ) {
        pool->poolQuitCond.signal();
    }

    pool->poolLock.unLock();

}

void
FreakThreadPool::removeThread(size_t no)
{
    CHECK_GE(no,0);
    CHECK_LT(no,currentWorkerSize());
    FreakThreadPtr worker = workers[no];
    ILOG<<"Thread "<<worker->selfId()<<" has been removed";
    worker->onExit();
    delete worker;

    workers.erase(workers.begin()+no,workers.begin()+no+1);

}

bool
FreakThreadPool::insertJob(FreakThreadPoolJobPtr job)
{
    poolLock.lock();

    if (quit) {
        goto error;
    }

    if (freeWorkers <=0 && currentWorkerSize() < maxWorkers) {
        try {
            expandWorkers(1);
        } catch(FreakThreadPoolExpandException& e ){
            ILOG<<"Pool Full";
        }

    }

    jobList.push(job);
    poolCond.signal();
    poolLock.unLock();
    return true;

error :
    poolLock.unLock();
    return false;


}

size_t 
FreakThreadPool::currentWorkerSize()
{
    return workers.size();
}

size_t 
FreakThreadPool::freeWorkerSize()
{
    return freeWorkers;
}

FreakThreadPool::~FreakThreadPool()
{
    poolLock.lock();
    quit = true;

    if (currentWorkerSize() > 0) {
        poolCond.broadCast();
    }

    while ( currentWorkerSize() > 0 ) {
        poolQuitCond.wait(&poolLock);
    }

    while (!jobList.empty()) {
        FreakThreadPoolJobPtr job = jobList.front();
        jobList.pop();
        delete job;
    }
    poolLock.unLock();
    poolLock.destroy();
    poolQuitCond.destroy();
    poolCond.destroy();

}


}
