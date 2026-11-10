/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daogawa <daogawa@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 03:24:42 by daogawa           #+#    #+#             */
/*   Updated: 2026/10/04 03:49:38 by daogawa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void enqueue_request(t_coder *coder)
{
    t_table *table;
    t_heap_request req;
    table = coder->table;

    if (table->scheduler == FIFO) {
        req.key = table->next_seq;
    } else {
        req.key = coder->last_compile_start + table->time_to_burnout;
    }
    req.seq = table->next_seq++;
    req.coder = coder;
    heap_push(table->wait_heap, req);
}

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

int	has_priority(t_coder *coder)
{
	int				index;
	t_heap			*heap;
	t_heap_request	*self;
	t_heap_request	*rival;

	heap = coder->table->wait_heap;
	index = heap_find(heap, coder);
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
