/*
  POSIX���ŕK�v�ɂȂ�}�N����C�����C���֐��ށB
*/
#ifndef _POSIX_UTILS_H_INCLUDED_
#define _POSIX_UTILS_H_INCLUDED_

#ifdef POSIX
#include <time.h>
#include <sys/time.h>

inline time_t posix_get_current_tick() {
	struct timeval ts;
//	struct timespec {time_t tv_sec; long tv_nsec;} ts;
//    struct timespec ts;
#ifdef CLOCK_UPTIME
	clock_gettime(CLOCK_UPTIME, &ts);
#else
	gettimeofday(&ts, NULL);
//	clock_gettime(CLOCK_MONOTONIC, &ts);
#endif
	return (ts.tv_sec * 1000) + (ts.tv_usec/1000/1000);
//	return (ts.tv_sec * 1000) + (ts.tv_nsec/1000/1000);
}

#else  //POSIX

inline time_t posix_get_current_tick() {
    return ::GetTickCount();
}

#endif //POSIX

inline time_t posix_get_current_sec() {
    return posix_get_current_tick() / 1000;
}

#endif //_POSIX_UTILS_H_INCLUDED_
