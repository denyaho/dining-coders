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

#include "includes/codexion.h"

static int	check_burnout(t_coder *coder)
{
	long		last_compile_time;
	long		burnout_time;
	t_params	*param;

	pthread_mutex_lock(&coder->coder_lock);
	last_compile_time = coder->last_compile_time;
	pthread_mutex_unlock(&coder->coder_lock);

	param = coder->param;
	burnout_time = param->time_to_burnout;
	if (last_compile_time + burnout_time < get_current_time())
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
	pthread_mutex_lock(&param->stop_lock);
	param->stopped = 1;
	pthread_mutex_unlock(&param->stop_lock);
}

int all_compiled(t_params *param)
{
	int compile_count;
	int	required_compile_count;
	int index;

	index = 0;
	while (index < param->n_coders)
	{
		pthread_mutex_lock(&param->coders[index].coder_lock);
		compile_count = param->coders[index].compile_count;
		pthread_mutex_unlock(&param->coders[index].coder_lock);
		if (compile_count < param->n_compiles_required)
			return (0);
		index++;
	}
	return (1);
}

void	*monitor_run(void *arg)
{
	t_params		*param;
	struct timespec	ts;
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
			if (check_burnout(&coder[index]) == 1)
			{
				set_stopped(param);
				return NULL;
			}
			index++;
		}
		if (all_compiled(param) == 1)
		{
			set_stopped(param);
			return NULL;
		}
		usleep(1000); // Sleep for 1 millisecond to prevent busy waiting
	}
}
