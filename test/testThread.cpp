#include <freakThread.h>
#include <cstdio>
#include <cstring>

using namespace freak;

FreakMutexPtr mp = new FreakMutex();
FreakCondPtr cp = new FreakCond();
static size_t t=1;

void printInfo(void* data)
{
    char* str = static_cast<char*>(data);

    mp->lock();

    while(t != strlen(str)) {
        printf("threadId=%llu,t=%lu, wait thread %lu\n",FreakThread::selfId(),t,FreakThread::self());
        cp->wait(mp);
    } ;

    t++;
    cp->broadCast();

    printf("thread : %lu, %s\n",pthread_self(),str);
    mp->unLock();


}

int main()
{
    int n = 20;
    FreakThreadPtr* threadArray = new FreakThreadPtr[n];
    const char* array = "ABCDEFGIJK0123456789";

    mp->init();
    cp->init();
    for(int i=0;i<n;++i) {
        FreakThreadPtr t = new FreakThread(true,printInfo,__func__,true,(void*)(array+i));
        threadArray[i] = t;
    }
    cp->signal();
    for(int i=0;i<n;++i) {
        threadArray[i]->join();
        delete threadArray[i];
    }
    mp->destroy();
    cp->destroy();

    delete[] threadArray;
    delete mp;
    delete cp;

    return 0;
}

