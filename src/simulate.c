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

static void join_created_coders(t_table *table, int n_created)
{
	int index;

	set_stopped(table);
	index = 0;
	while (index < n_created)
	{
		pthread_join(table->coders[index].thread, NULL);
		index++;
	}
}

int	run_simulation(t_table *table)
{
	int			index;
	pthread_t	monitor_thread;

	if (init_dongles(table) != 0)
		return (1);
	if (init_coders(table) != 0)
	{
		free(table->dongles);
		return (1);
	}
	index = 0;
	while (index < table->n_coders)
	{
		if (pthread_create(&table->coders[index].thread, NULL, coder_routine,
			&table->coders[index]) != 0)
			{
				table->thread_create_count = index;
				join_created_coders(table, index);
				return (1);
			}
		index++;
	}
	if (pthread_create(&monitor_thread, NULL,
		monitor_routine, table) != 0)
		{
			join_created_coders(table, table->n_coders);
			return (1);
		}
	pthread_join(monitor_thread, NULL);
	index = 0;
	while (index < table->n_coders)
	{
		if (pthread_join(table->coders[index].thread, NULL))
            return (0);
		index++;
	}	
	return (0);
}
