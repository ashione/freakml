#include <freakThreadJob.h>
#include <cerrno>
#include <freakException.h>

#include <freakConfig.h>

namespace freak {

int
FreakThreadLocal::init()
{
    return init(NULL);
}

int
FreakThreadLocal::init(freakThreadLocalCleanup c)
{
    int ret;
    if ((ret = pthread_key_create(&key, c)) != 0) {
        errno = ret;
        return -1;
    }
    return 0;
}

void*
FreakThreadLocal::get()
{
    return pthread_getspecific(key);
}

int
FreakThreadLocal::set(void *val)
{
    int err = pthread_setspecific(key, val);
    if (err) {
        errno = err;
        return -1;
    }
    return 0;
}

FreakThreadLocal workerName;
FreakThreadLocal jobName;

void
FreakThreadJob::onceInit()
{
    if (workerName.init() < 0 || jobName.init() < 0)
        throw new FreakThreadInitException("thread local init exception");

}

int
FreakThreadJob::init()
{
    if (jobControl.once(onceInit) < 0)
        return -1;                                               

    return 0;
}

const char *
FreakThreadJob::getJob()
{
    const char *job;

    if ( init() < 0)
        return NULL;

    job = static_cast<char *>(jobName.get());
    if (!job)
        job = static_cast<char *>(workerName.get());

    return job;
}


void
FreakThreadJob::setWorkerName(const char *worker)
{
    if (!worker || init() < 0)
        return;

    if (workerName.set((void *) worker) < 0) {
        FLOG<<errno<<" cannot set worker name to "<<worker;
    }

        /*virReportSystemError(errno,
                             _("cannot set worker name to %s"),
                                 worker);
         */


    //VIR_DEBUG("Thread %llu is running worker %s", FreakThread::selfId(), worker);
    ILOG<<"Thread "<<(FreakThread::selfId())<<" is running worker "<< worker;
}


void
FreakThreadJob::setJob(const char *caller)
{
    const char *worker;

    if (!caller || init() < 0)
        return;

    if (jobName.set((void *) caller) < 0)
        FLOG<<errno<<" cannot set current job to "<<caller;

    if ((worker = static_cast<const char*>(workerName.get()))) {
        ILOG<<"Thread "<<(FreakThread::selfId())<<" ("<<worker<<") is now running job "<<caller;
    } else {
        ILOG<<"Thread "<<(FreakThread::selfId())<<" is now running job "<<caller;
    }
}


void
FreakThreadJob::clear(int rv)
{
    const char *old;
    const char *worker;

    if ( init() < 0)
        return;

    if (!(old = static_cast<const char*>(jobName.get())))
        return;

    if (jobName.set(NULL) < 0)
        FLOG<<errno<<"cannot reset current job";

    if ((worker = static_cast<const char*>(workerName.get()))) {
        ILOG<<"Thread "<<(FreakThread::selfId())<<"("<<worker<<") finished job "<<worker<<" with ret="<<rv;
    } else {
        ILOG<<"Thread "<<(FreakThread::selfId())<<" finished job "<<old<<" with ret="<<rv;
    }
}

}
