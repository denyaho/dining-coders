#include "codexion.h"

long long get_start_time()
{
    struct timespec ts;

    clock_gettime(CLOCK_REALTIME, &ts);
    return (long long)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

long long get_current_time(t_params *param)
{
    struct timespec ts;

    clock_gettime(CLOCK_REALTIME, &ts);
    return (long long)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000) - param->start_time;
}

struct timespec *get_shorter_cooldown_dongle(struct timespec *ts, t_coder *coder)
{
    long left_cooldown;
    long right_cooldown;
    t_params *param;

    param = coder->param;
    left_cooldown = coder->left_dongle->available_at + param->start_time;
    right_cooldown = coder->right_dongle->available_at + param->start_time;
    if (left_cooldown > right_cooldown){
        ts->tv_sec = left_cooldown / 1000;
        ts->tv_nsec = (left_cooldown % 1000) * 1000000;
        return ts;
    }
    else
    {
        ts->tv_sec = right_cooldown / 1000;
        ts->tv_nsec = (right_cooldown % 1000) * 1000000;
        return ts;
    }
}