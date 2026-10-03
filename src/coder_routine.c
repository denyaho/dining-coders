/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daogawa <daogawa@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 06:39:51 by daogawa           #+#    #+#             */
/*   Updated: 2026/10/04 07:15:15 by daogawa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"


int get_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->dongle_lock);
	dongle->in_use = 1;
	pthread_mutex_unlock(&dongle->dongle_lock);
	return (0);
}

void *coder_run(void *arg)
{

    t_coder *coder;
    coder = (t_coder *)arg;

	while (!is_stopped(coder->param))
	{
		if (get_dongle(coder->left_dongle) == 0)
		{
			// Coder's routine actions go here
		}
	}
}
