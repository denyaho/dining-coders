/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daogawa <daogawa@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 22:10:16 by daogawa           #+#    #+#             */
/*   Updated: 2026/10/13 22:26:46 by daogawa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <time.h>

void	do_refactor(t_coder *coder)
{
	if (is_stopped(coder->param))
		return ;
	print_status(coder->param, REFACTORING, coder->id);
	ft_usleep(coder->param->time_to_refactor, coder->param);
}

void	do_compile(t_coder *coder)
{
	t_params	*param;

	param = coder->param;
	if (is_stopped(param))
		return ;
	pthread_mutex_lock(&coder->lock);
	coder->last_compile_start = get_process_time(param);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->lock);
	print_status(coder->param, COMPILING, coder->id);
	ft_usleep(coder->param->time_to_compile, coder->param);
}

void	do_debug(t_coder *coder)
{
	if (is_stopped(coder->param))
		return ;
	print_status(coder->param, DEBUGGING, coder->id);
	ft_usleep(coder->param->time_to_debug, coder->param);
}

int	release_dongle(t_coder *coder)
{
	t_params	*param;
	int			id;

	id = coder->id;

	param = coder->param;
	pthread_mutex_lock(&param->table_lock);
	coder->left_dongle->in_use = 0;
	coder->left_dongle->available_at
		= get_process_time(param) + param->dongle_cooldown;
	coder->right_dongle->in_use = 0;
	coder->right_dongle->available_at
		= get_process_time(param) + param->dongle_cooldown;
	signal_to_adjacent_coder(param, id);
	pthread_mutex_unlock(&param->table_lock);
	return (0);
}

int	get_dongle(t_coder *coder)
{
	t_params	*param;
	struct		timespec ts;

	param = coder->param;
	clock_gettime(CLOCK_REALTIME, &ts);
	pthread_mutex_lock(&param->table_lock);
	schedule_heap(coder);
	while (!is_top(coder) || !is_available_dongle(coder)
		|| is_used_dongle(coder))
	{
		if (is_stopped(param))
			break ;
		if (!is_available_dongle(coder))
			pthread_cond_timedwait(&coder->cond, &param->table_lock,
				get_shorter_cooldown_dongle(&ts, coder));
		else
			pthread_cond_wait(&coder->cond, &param->table_lock);
	}
	heap_delete_at(param->wait_heap, find_heap(param->wait_heap, coder));
	if (is_stopped(param))
	{
		pthread_mutex_unlock(&param->table_lock);
		return (1);
	}
	coder->left_dongle->in_use = 1;
	coder->right_dongle->in_use = 1;
	pthread_mutex_unlock(&param->table_lock);
	print_status(coder->param, TAKEN_DONGLE, coder->id);
	return (0);
}
