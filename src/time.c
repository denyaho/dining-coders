/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daogawa <daogawa@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 21:31:21 by daogawa           #+#    #+#             */
/*   Updated: 2026/10/13 21:31:22 by daogawa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long get_current_time(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_REALTIME, &ts);
    return (long long)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

long long get_process_time(t_table *table)
{
    struct timespec ts;

    clock_gettime(CLOCK_REALTIME, &ts);
    return (long long)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000) - table->start_time;
}

struct timespec *get_dongles_ready_time(struct timespec *ts, t_coder *coder)
{
    long left_ready_ms;
    long right_ready_ms;
    t_table *table;

    table = coder->table;
    left_ready_ms = coder->left_dongle->available_at + table->start_time;
    right_ready_ms = coder->right_dongle->available_at + table->start_time;
    if (left_ready_ms > right_ready_ms){
        ts->tv_sec = left_ready_ms / 1000;
        ts->tv_nsec = (left_ready_ms % 1000) * 1000000;
        return ts;
    }
    else
    {
        ts->tv_sec = right_ready_ms / 1000;
        ts->tv_nsec = (right_ready_ms % 1000) * 1000000;
        return ts;
    }
}

void ft_usleep(long duration_ms, t_table *table)
{
    long end_time_ms;
    long remaining_time_us;

    end_time_ms = get_current_time() + duration_ms;
    while (!is_stopped(table))
    {
        remaining_time_us = (end_time_ms - get_current_time()) * 1000;
        if (remaining_time_us <= 0)
            break;
        if (remaining_time_us > 500)
            usleep(500);
        else
            usleep(remaining_time_us);
    }
}
