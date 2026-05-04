#include <time.h>
#include <stdio.h>
#include <sys/time.h>

char *ui_get_time(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm *tm = localtime(&tv.tv_sec);
    static char buf[64];
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d.%03d",
        tm->tm_hour,
        tm->tm_min,
        tm->tm_sec,
        tv.tv_usec / 1000  // microseconds → milliseconds
    );
    return buf;
}
