/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleep.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daogawa <daogawa@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 21:31:18 by daogawa           #+#    #+#             */
/*   Updated: 2026/10/13 21:31:19 by daogawa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void ft_usleep(long duration_ms, t_params *params)
{
    long end_time_ms;
    long remaining_time_us;

    end_time_ms = get_current_time() + duration_ms;
    while (!is_stopped(params))
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
