#ifndef __FREAK_THREAD_H__
#define __FREAK_THREAD_H__

#include <pthread.h>

namespace freak {

typedef class FreakMutex *FreakMutexPtr;

class FreakMutex {
    public :
        pthread_mutex_t mLock;

    public :
        FreakMutex();
        int init(void);
        int initRecursive(void);
        void destroy(void);

        void lock(void);
        void unLock(void);

};


class FreakRWLock {

    public :
        pthread_rwlock_t rwLock;

    public :
        int init(void);
        void destroy(void);

        void read(void);
        void write(void);
        void unLock(void);

};

typedef FreakRWLock *FreakRWLockPtr;

typedef class FreakCond *FreakCondPtr;

class FreakCond {

    public :
       pthread_cond_t cond;

    public :
       int init(void);
       int destroy(void);
       int wait(FreakMutexPtr);
       int waitUntil(FreakMutexPtr,unsigned long long);
       void signal();
       void broadCast();

};


typedef class FreakThread *FreakThreadPtr;

typedef void (*freakThreadFunc)(void *opaque);

class FreakThread {

    private :
        pthread_t thread;

    public :
        FreakThread(){};
        FreakThread(bool,freakThreadFunc,const char*,bool,void*);
        int init();
        void onExit(void);
        void join(void);
        void cancel(void);

        void setThread(pthread_t);
        pthread_t getThread();

        bool isSelf(); 

        static unsigned long long selfId();
        static pthread_t self(void);
};



typedef void (*freakOnceFunc)(void);

class FreakOnceControl {
    private :
        pthread_once_t mOnce;
    public :
        FreakOnceControl();
        int once(freakOnceFunc);
};

}


#endif /* __FREAK_THREAD_H__ */
