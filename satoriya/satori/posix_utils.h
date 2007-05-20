/*
  POSIX���ŕK�v�ɂȂ�}�N����C�����C���֐��ށB
*/
#if !defined(_POSIX_UTILS_H_INCLUDED_) && defined(POSIX)
#define _POSIX_UTILS_H_INCLUDED_
#include <sys/time.h>

inline long posix_get_current_millis() {
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    // tv_usec�̓G�|�b�N����̃}�C�N���b�ł�*�Ȃ�*!
    // tv_sec����̒ǉ��}�C�N���b�ł���B
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

#endif
