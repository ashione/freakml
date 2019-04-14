#include <freakThreadPool.h>
#include <freakThread.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <unistd.h>

using namespace freak;
using namespace std;

void jobFunc(void* data)
{
    char* str = static_cast<char*>(data);

    for(size_t i=0;i<10;++i) {
        usleep(1e5);
        printf("tid : %llu, data : %s\n",FreakThread::selfId(),str);
    }
}

void poolLog(void *data)
{
    FreakThreadPoolPtr pool = static_cast<FreakThreadPoolPtr>(data);
    while(true) {
        printf("pool currentWorkerSize : %lu, freeWorkerSize : %lu\n",pool->currentWorkerSize(),pool->freeWorkerSize());
        usleep(1e5);

    }

}

int 
main()
{
    int minWorkers = 3;
    int maxWorkers = 5;
    FreakThreadPool pool(minWorkers,maxWorkers);
    const char* array = "ABCDEFGIJK0123456789";
    FreakThreadPoolJobPtr showJob = new FreakThreadPoolJob("showLog",poolLog,NULL,&pool);
    pool.insertJob(showJob);

    for(int i=0;i<20;++i) {
        FreakThreadPoolJobPtr job = new FreakThreadPoolJob(std::to_string(i),jobFunc,NULL,(void*)(array+i));
        pool.insertJob(job);
    }

    usleep(1e7);

    printf("main thread exit\n");

}
