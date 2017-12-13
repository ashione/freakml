#ifndef __FREAK_LOG_H__
#define __FREAK_LOG_H__

#include <glog/logging.h>

#define ILOG LOG(INFO)
#define WLOG LOG(WARN)
#define ELOG LOG(ERROR)
#define FLOG LOG(FATAL)

#define REGISTER_LOG(argv)  do { \
    google::InitGoogleLogging(argv); \
    FLAGS_alsologtostderr = 1; \
} while(0);


#endif /*__FREAK_LOG_H__*/
