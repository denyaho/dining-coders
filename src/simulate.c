/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daogawa <daogawa@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 00:00:00 by daogawa           #+#    #+#             */
/*   Updated: 2026/10/04 03:54:43 by daogawa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void fail_thread_creation(t_params *param, int i)
{
	int index;

	set_stopped(param);
	index = 0;
	while (index < i)
	{
		pthread_join(param->coders[index].thread, NULL);
		index++;
	}
}


int	run_simulate(t_params *param)
{
	int			index;
	pthread_t	monitor_thread;

	if (init_dongles(param) != 0)
		return (1);
	if (init_coders(param) != 0)
	{
		free(param->dongles);
		return (1);
	}
	index = 0;
	while (index < param->n_coders)
	{
		if (pthread_create(&param->coders[index].thread, NULL, coder_run,
			&param->coders[index]) != 0)
			{
				param->thread_create_count = index;
				fail_thread_creation(param, index);
				return (1);
			}
		index++;
	}
	if (pthread_create(&monitor_thread, NULL,
		monitor_run, param) != 0)
		{
			fail_thread_creation(param, param->n_coders);
			return (1);
		}
	pthread_join(monitor_thread, NULL);
	index = 0;
	while (index < param->n_coders)
	{
		if (pthread_join(param->coders[index].thread, NULL))
            return (0);
		index++;
	}	
	return (0);
}
