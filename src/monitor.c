/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daogawa <daogawa@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 03:08:30 by daogawa           #+#    #+#             */
/*   Updated: 2026/10/04 06:38:05 by daogawa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_burnout(t_coder *coder)
{
	t_table	*table;
	long		last_compile_time;
	long		burnout_time;

	pthread_mutex_lock(&coder->lock);
	last_compile_time = coder->last_compile_start;
	table = coder->table;
	burnout_time = table->time_to_burnout;
	pthread_mutex_unlock(&coder->lock);

	if (last_compile_time + burnout_time < get_process_time(table))
		return (1);
	return (0);
}

static int all_compiled(t_table *table)
{
	int compile_count;
	int index;
	t_coder			*coder;

	index = 0;
	while (index < table->n_coders)
	{
		coder = &table->coders[index];
		pthread_mutex_lock(&coder->lock);
		compile_count = coder->compile_count;
		pthread_mutex_unlock(&coder->lock);
		if (compile_count < table->n_compiles_required)
			return (0);
		index++;
	}
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_table		*table;
	t_coder			*coder;
	int 			index;

	
	table = (t_table *)arg;
	coder = table->coders;
	index = 0;
	while (1)
	{
		index = 0;
		while (index < table->n_coders)
		{
			if (check_burnout(&coder[index]))
			{
				print_status(table, BURNED_OUT, coder[index].id);
				set_stopped(table);
				return NULL;
			}
			index++;
		}
		if (all_compiled(table))
		{
			set_stopped(table);
			return NULL;
		}
		ft_usleep(1, table); // Sleep for 1 millisecond to prevent busy waiting
	}
}
