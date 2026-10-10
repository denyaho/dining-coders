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
	t_params	*param;
	long		last_compile_time;
	long		burnout_time;

	pthread_mutex_lock(&coder->lock);
	last_compile_time = coder->last_compile_start;
	param = coder->param;
	burnout_time = param->time_to_burnout;
	pthread_mutex_unlock(&coder->lock);

	if (last_compile_time + burnout_time < get_process_time(param))
		return (1);
	return (0);
}

int is_stopped(t_params *param)
{
	int	stopped;

	pthread_mutex_lock(&param->stop_lock);
	stopped = param->stopped;
	pthread_mutex_unlock(&param->stop_lock);
	return	stopped;
}

void set_stopped(t_params *param)
{
	t_coder		*coder;
	int index;
	
	index = 0;
	pthread_mutex_lock(&param->stop_lock);
	param->stopped = 1;
	pthread_mutex_unlock(&param->stop_lock);
	
	pthread_mutex_lock(&param->table_lock);
	while (index < param->n_coders)
	{
		coder = &param->coders[index];
		pthread_cond_signal(&coder->cond);
		index++;
	}
	pthread_mutex_unlock(&param->table_lock);
}

static int all_compiled(t_params *param)
{
	int compile_count;
	int index;
	t_coder			*coder;

	index = 0;
	while (index < param->n_coders)
	{
		coder = &param->coders[index];
		pthread_mutex_lock(&coder->lock);
		compile_count = coder->compile_count;
		pthread_mutex_unlock(&coder->lock);
		if (compile_count < param->n_compiles_required)
			return (0);
		index++;
	}
	return (1);
}

void	*monitor_run(void *arg)
{
	t_params		*param;
	t_coder			*coder;
	int 			index;

	
	param = (t_params *)arg;
	coder = param->coders;
	index = 0;
	while (1)
	{
		index = 0;
		while (index < param->n_coders)
		{
			if (check_burnout(&coder[index]))
			{
				print_status(param, BURNED_OUT, coder[index].id);
				set_stopped(param);
				return NULL;
			}
			index++;
		}
		if (all_compiled(param))
		{
			set_stopped(param);
			return NULL;
		}
		ft_usleep(1, param); // Sleep for 1 millisecond to prevent busy waiting
	}
}
