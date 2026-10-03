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

#include "includes/codexion.h"



int	run_simulate(t_params *param)
{
	int			index;
	pthread_t	monitor_thread;

	if (init_dongles(param) != 0)
		return (1);
	if (init_coders(param) != 0)
		return (1);
	index = 0;
	while (index < param->n_coders)
	{
		if (pthread_create(&param->coders[index].thread, NULL, coder_run,
			&param->coders[index] != 0))
				return (1);
		index++;
	}
	index = 0;
	if (pthread_create(&monitor_thread, NULL,
		monitor_run, &param) != 0)
		return (1);
	pthread_join(monitor_thread, NULL);
	while (index < param->n_coders)
	{
		pthread_join(param->coders[index].thread, NULL);
		index++;
	}
	return (0);
}
