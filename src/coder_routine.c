/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daogawa <daogawa@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 06:39:51 by daogawa           #+#    #+#             */
/*   Updated: 2026/10/13 23:11:47 by daogawa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	is_neighbour(t_coder *rival, t_coder *coder)
{
	return (rival->left_dongle == coder->right_dongle
		|| rival->right_dongle == coder->left_dongle);
}

static bool	is_ahead(t_heap_request *rival, t_heap_request *self)
{
	if (rival->key != self->key)
		return (rival->key < self->key);
	return (rival->seq < self->seq);
}

int	is_top(t_coder *coder)
{
	int				index;
	t_heap			*heap;
	t_heap_request	*self;
	t_heap_request	*rival;

	heap = coder->param->wait_heap;
	index = find_heap(heap, coder);
	if (index == -1)
		return (false);
	self = &heap->requests[index];
	index = 0;
	while (index < heap->size)
	{
		rival = &heap->requests[index];
		if (is_neighbour(rival->coder, coder) && is_ahead(rival, self))
			return (false);
		index++;
	}
	return (true);
}

static void	*run_single_coder(t_coder *coder, t_params *param)
{
	print_status(param, TAKEN_DONGLE, coder->id);
	pthread_mutex_lock(&param->table_lock);
	while (!is_stopped(param))
		pthread_cond_wait(&coder->cond, &param->table_lock);
	pthread_mutex_unlock(&param->table_lock);
	return (NULL);
}

void	*coder_run(void *arg)
{
	t_coder		*coder;
	t_params	*param;

	coder = (t_coder *)arg;
	param = coder->param;
	if (param->n_coders == 1)
		return (run_single_coder(coder, param));
	if (coder->id % 2 != 0)
		ft_usleep(1, coder->param);
	while (!is_stopped(coder->param))
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
