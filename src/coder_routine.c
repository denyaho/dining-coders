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

#include "codexion.h"

int is_top(t_coder *coder)
{
	int index;
	t_heap *heap;
	t_params *param;
	t_coder *rival_coder;
	t_heap_request coder_request;

	param = coder->param;
	heap = param->wait_heap;

	index = find_heap(heap, coder);
	if (index == -1)
		return false;
	coder_request = heap->requests[index];
	index = 0;
	while (index < heap->size)
	{
		rival_coder = heap->requests[index].coder;
		if (rival_coder->left_dongle == coder->right_dongle 
			|| rival_coder->right_dongle == coder->left_dongle)
		{
			if (heap->requests[index].key < coder_request.key)
				return false;
			else if(heap->requests[index].key == coder_request.key)
				if (heap->requests[index].seq < coder_request.seq)
					return false;
		}
		index++;
	}
	return true;
}

bool is_used_dongle(t_coder *coder)
{
	return (coder->left_dongle->in_use || coder->right_dongle->in_use);
}

bool is_available_dongle(t_coder *coder)
{
	t_params *param;
	
	param = coder->param;
	return (get_process_time(param) >= coder->left_dongle->available_at 
		&& get_process_time(param) >= coder->right_dongle->available_at);
}



int get_dongle(t_coder *coder)
{
	t_params *param;
	param = coder->param;
	struct timespec ts;

	clock_gettime(CLOCK_REALTIME, &ts);
	pthread_mutex_lock(&param->table_lock);
	schedule_heap(coder);
	while (!is_top(coder) || !is_available_dongle(coder) || is_used_dongle(coder))
	{
		if (is_stopped(param))
			break;
		if (!is_available_dongle(coder))
			pthread_cond_timedwait(&coder->cond, &param->table_lock, get_shorter_cooldown_dongle(&ts, coder));
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

void signal_to_adjacent_coder(t_params *param, int id)
{
	if (param->n_coders == 1)
		return;
	if (id == 0)
		pthread_cond_signal(&param->coders[param->n_coders - 1].cond);
	else
		pthread_cond_signal(&param->coders[id - 1].cond);
	if (id == param->n_coders - 1)
		pthread_cond_signal(&param->coders[0].cond);
	else
		pthread_cond_signal(&param->coders[id + 1].cond);
}

int release_dongle(t_coder *coder)
{
	t_params *param;
	int id;

	id = coder->id;

	param = coder->param;
	pthread_mutex_lock(&param->table_lock);
	coder->left_dongle->in_use = 0;
	coder->left_dongle->available_at = get_process_time(param) + param->dongle_cooldown;
	coder->right_dongle->in_use = 0;
	coder->right_dongle->available_at = get_process_time(param) + param->dongle_cooldown;
	signal_to_adjacent_coder(param, id);
	pthread_mutex_unlock(&param->table_lock);
	return (0);
}

void do_debug(t_coder *coder)
{
	if (is_stopped(coder->param))
		return;
	print_status(coder->param, DEBUGGING, coder->id);
	ft_usleep(coder->param->time_to_debug, coder->param);
}

void do_refactor(t_coder *coder)
{
	if (is_stopped(coder->param))
		return;
	print_status(coder->param, REFACTORING, coder->id);
	ft_usleep(coder->param->time_to_refactor, coder->param);
}

void do_compile(t_coder *coder)
{
	t_params *param;
	param = coder->param;

	if (is_stopped(param))
		return;
	pthread_mutex_lock(&coder->lock);
	coder->last_compile_start = get_process_time(param);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->lock);
	print_status(coder->param, COMPILING, coder->id);
	ft_usleep(coder->param->time_to_compile, coder->param);
}
void *coder_run(void *arg)
{
    t_coder *coder;
	t_params *param;
    coder = (t_coder *)arg;
	param = coder->param;

	if (param->n_coders == 1)
	{
		print_status(coder->param, TAKEN_DONGLE, coder->id);
		pthread_mutex_lock(&param->table_lock);
		while( !is_stopped(param))
			pthread_cond_wait(&coder->cond, &param->table_lock);
		pthread_mutex_unlock(&param->table_lock);
		return (NULL);
	}
	if (coder->id % 2 != 0)
		ft_usleep(1, coder->param);
	while(!is_stopped(coder->param))
	{
		if (get_dongle(coder))
			return (NULL);
		do_compile(coder);
		release_dongle(coder);
		do_debug(coder);
		do_refactor(coder);
	}
	return (NULL);
}

