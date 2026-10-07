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

int is_top(t_coder *coder)
{
	int index;
	t_heap *heap;
	t_params *param;
	t_coder *rival_coder;
	t_heap_request coder_request;

	param = coder->param;
	heap = param->wait_heap;

	coder_request = find_heap(heap, coder);
	index = 0;
	while (index < heap->size)
	{
		rival_coder = heap->requests[index].coder;
		if (rival_coder->left_dongle == coder->right_dongle || rival_coder->right_dongle == coder->left_dongle)
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

int is_used_dongle(t_coder *coder)
{
	return (coder->left_dongle->in_use || coder->right_dongle->in_use);
}

int is_available_dongle(t_coder *coder)
{
	return (get_current_time() > coder->left_dongle->available_at 
		&& get_current_time() > coder->right_dongle->available_at);
}

int get_dongle(t_coder *coder)

{
	t_params *param;
	param = coder->param;

	pthread_mutex_lock(&param->table_lock);
	schedule_heap(coder);
	while (!is_top(coder) || !is_available_dongle(coder) || is_used_dongle(coder))
		pthread_cond_wait(&coder->cond, &param->table_lock);
	heap_pop(param->wait_heap);
	if (is_stopped(param))
	{
		pthread_mutex_unlock(&param->table_lock);
		return (0);
	}
	coder->left_dongle->in_use = 1;
	coder->right_dongle->in_use = 1;
	pthread_mutex_unlock(&param->table_lock);
	printf("%d %d has taken a dongle\n", get_current_time(), coder->id);
	return (1);
}


int release_dongle(t_coder *coder)
{
	t_params *param;

	param = coder->param;
	pthread_mutex_lock(&param->table_lock);
	coder->left_dongle->in_use = 0;
	coder->left_dongle->available_at = get_current_time() + param->dongle_cooldown;
	pthread_cond_broadcast(&coder->left_dongle->cond);

	coder->right_dongle->in_use = 0;
	coder->right_dongle->available_at = get_current_time() + param->dongle_cooldown;
	pthread_cond_signal(&coder->right_dongle->cond);
	pthread_mutex_unlock(&param->table_lock);
	return (0);
}

void do_debug(t_coder *coder)
{
	printf("%d %d is debugging\n", get_current_time(), coder->id);
	usleep(coder->param->time_to_debug);
}

void do_refactor(t_coder *coder)
{
	printf("%d %d is refactoring\n", get_current_time(), coder->id);
	usleep(coder->param->time_to_refactor);
}

void do_compile(t_coder *coder)
{
	t_params *param;
	param = coder->param;

	pthread_mutex_lock(&param->table_lock);
	coder->last_compile_start = get_current_time();
	coder->compile_count++;
	pthread_mutex_unlock(&param->table_lock);
	printf("%d %d is compiling\n", get_current_time(), coder->id);
	usleep(coder->param->time_to_compile);
}

void *coder_run(void *arg)
{

    t_coder *coder;
    coder = (t_coder *)arg;
	t_params *param;

	param = coder->param;
	while (!is_stopped(coder->param))
	{
		if (!get_dongle(coder))
			return (NULL);
		do_compile(coder);
		release_dongle(coder);
		do_debug(coder);
		do_refactor(coder);
	}
}

