#ifndef __FREAK_THREAD_JOB_H__
#define __FREAK_THREAD_JOB_H__

#include <freakThread.h>

namespace freak {

typedef class FreakThreadLocal *FreakThreadLocalPtr;

typedef void (*freakThreadLocalCleanup)(void *);

class FreakThreadLocal
{
    private :
        pthread_key_t key;
    
    public :
        int init();
        int init(freakThreadLocalCleanup);
        void* get();
        int set(void*);

};

class FreakThreadJob
{

    private :
        FreakOnceControl jobControl;

    public :
        int init();
        static void onceInit();

        const char* getJob();
        void setJob(const char*);
        void setWorkerName(const char*);
        void clear(int);

};

}

#endif /* __FREAK_THREAD_JOB_H */

