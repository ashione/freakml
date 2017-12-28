#include <freakThread.h>
#include <freakThreadJob.h>

#include <cerrno>
#include <cstdlib>
#if HAVE_SYS_SYSCALL_H
# include <sys/syscall.h>
#endif

namespace freak {

struct FreakThreadArgs {
    freakThreadFunc func;
    const char *funcName;
    bool worker;
    void *opaque;
};

FreakThreadJob threadjob;

static void *freakThreadHelper(void *data)
{
    FreakThreadArgs *args = static_cast<FreakThreadArgs*>(data);
    FreakThreadArgs local = *args;

    /* Free args early, rather than tying it up during the entire thread.  */
    //VIR_FREE(args);
    free(args);
    /*
    if (local.worker)
        virThreadJobSetWorker(local.funcName);
    else
        virThreadJobSet(local.funcName);
    */
    
    if (local.worker)
        threadjob.setWorkerName(local.funcName);
    else
        threadjob.setJob(local.funcName);

    local.func(local.opaque);

    /*
    if (!local.worker)
        virThreadJobClear(0);
    */

    if (!local.worker)
        threadjob.clear(0);

    return NULL;
}

FreakMutex::FreakMutex()
{
    mLock = PTHREAD_MUTEX_INITIALIZER;
}

int
FreakMutex::init()
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);
    int ret = pthread_mutex_init(&mLock, &attr);
    pthread_mutexattr_destroy(&attr);
    if (ret != 0) {
        errno = ret;
        return -1;
    }
    return 0;
}

int 
FreakMutex::initRecursive()
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    int ret = pthread_mutex_init(&mLock, &attr);
    pthread_mutexattr_destroy(&attr);
    if (ret != 0) {
        errno = ret;
        return -1;
    }
    return 0;
}

void
FreakMutex::destroy()
{
    pthread_mutex_destroy(&mLock);
}

void 
FreakMutex::lock()
{
    pthread_mutex_lock(&mLock);
}

void
FreakMutex::unLock()
{
    pthread_mutex_unlock(&mLock);
}

int
FreakRWLock::init()
{
    int ret = pthread_rwlock_init(&rwLock, NULL);
    if (ret != 0) {
        errno = ret;
        return -1;
    }
    return 0;

}

void 
FreakRWLock::destroy()
{
    pthread_rwlock_destroy(&rwLock);
}


void 
FreakRWLock::read()
{
    pthread_rwlock_rdlock(&rwLock);
}

void
FreakRWLock::write()
{
    pthread_rwlock_wrlock(&rwLock);
}


void
FreakRWLock::unLock()
{
    pthread_rwlock_unlock(&rwLock);
}

int
FreakCond::init()
{
    int ret;
    if ((ret = pthread_cond_init(&cond, NULL)) != 0) {
        errno = ret;
        return -1;
    }
    return 0;
}

int
FreakCond::destroy()
{
    int ret;
    if ((ret = pthread_cond_destroy(&cond)) != 0) {
        errno = ret;
        return -1;
    }
    return 0;
}

int
FreakCond::wait(FreakMutexPtr m)
{
    int ret;
    if ((ret = pthread_cond_wait(&cond, &m->mLock)) != 0) {
        errno = ret;
        return -1;
    }
    return 0;
}

void
FreakCond::singal()
{
    pthread_cond_signal(&cond);
}

void
FreakCond::broadCast()
{
    pthread_cond_broadcast(&cond);
}



int 
FreakCond::waitUntil(FreakMutexPtr m, unsigned long long whenms)
{
    int ret;
    struct timespec ts;

    ts.tv_sec = whenms / 1000;
    ts.tv_nsec = (whenms % 1000) * 1000000;

    if ((ret = pthread_cond_timedwait(&cond, &m->mLock, &ts)) != 0) {
        errno = ret;
        return -1;
    }
    return 0;
}

FreakThread::FreakThread(bool joinable,
                         freakThreadFunc func,
                         const char *funcName,
                         bool worker,
                         void *opaque)
{
    FreakThreadArgs *args;
    pthread_attr_t attr;
    int ret = -1;
    int err;

    if ((err = pthread_attr_init(&attr)) != 0)
        goto cleanup;
    args = (FreakThreadArgs * )malloc(sizeof(FreakThreadArgs));
    /*
    if (VIR_ALLOC_QUIET(args) < 0) {
        err = ENOMEM;
        goto cleanup;
    }
    */
    if (!args) {
        err = ENOMEM;
        goto cleanup;
    }

    args->func = func;
    args->funcName = funcName;
    args->worker = worker;
    args->opaque = opaque;

    if (!joinable)
        pthread_attr_setdetachstate(&attr, 1);

    err = pthread_create(&thread, &attr, freakThreadHelper, args);
    if (err != 0) {
        free(args);
        //VIR_FREE(args);
        goto cleanup;
    }
    /* New thread owns 'args' in success case, so don't free */

    ret = 0;
 cleanup:
    pthread_attr_destroy(&attr);
    if (ret < 0)
        errno = err;
    //return ret;
}

int
FreakThread::init()
{
    return 0;

}

void
FreakThread::onExit()
{

}

void
FreakThread::join()
{
    pthread_join(thread, NULL);
}

void
FreakThread::cancel()
{
    pthread_cancel(thread);
}

pthread_t 
FreakThread::self()
{
    return pthread_self();
}

bool
FreakThread::isSelf()
{
    return pthread_equal(pthread_self(), thread) ? true : false;
}

void
FreakThread::setThread(pthread_t pthd)
{
    thread = pthd;
}

unsigned long long
FreakThread::selfId()
{

#if defined(HAVE_SYS_SYSCALL_H) && defined(SYS_gettid)
    pid_t tid = syscall(SYS_gettid);
    return tid;
#else
    union {
        unsigned long long l;
        pthread_t t;
    } u;
    u.t = pthread_self();
    return u.l;
#endif

}

FreakOnceControl::FreakOnceControl()
{
    mOnce = PTHREAD_ONCE_INIT;
}

int 
FreakOnceControl::once(freakOnceFunc init)
{
    return pthread_once(&mOnce, init);
}

}
