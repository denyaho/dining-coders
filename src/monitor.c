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

void	*monitor_run(void *arg)
{
	t_params		*param;
	struct timespec	ts;
	t_coder			*coder;

	param = (t_params *)arg;
	coder = param->coders;

	if (coder->compile_count >= param->n_compiles_required)
	{
		pthread_mutex_lock(&param->stop_lock);
		param->stopped = 1;
		pthread_mutex_unlock(&param->stop_lock);
		return NULL;
	}
	if (check_burnout(coder) == 1)
	{
		pthread_mutex_lock(&param->stop_lock);
		param->stopped = 1;
		pthread_mutex_unlock(&param->stop_lock);
		return NULL;
	}
}
