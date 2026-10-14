/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daogawa <daogawa@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 21:31:04 by daogawa           #+#    #+#             */
/*   Updated: 2026/10/13 21:31:05 by daogawa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

#define LIMIT 2



void heap_push(t_heap *hp, t_heap_request req)
{
    int tail_index;
    int parent_index;

    if (heap_is_full(hp))
        return;
    hp->size++;
    tail_index = hp->size - 1;
    while (tail_index > 0)
    {
        parent_index = (tail_index - 1 ) / 2;
        if (hp->requests[parent_index].key <= req.key) {
            break;
        }
        hp->requests[tail_index] = hp->requests[parent_index];
        tail_index = parent_index;
    }
    hp->requests[tail_index] = req;
    return;
}

void schedule_heap(t_coder *coder)
{
    t_params *param;
    t_heap_request req;
    param = coder->param;

    if (param->scheduler == FIFO) {
        req.key = param->seq;
    } else {
        req.key = coder->last_compile_start + param->time_to_burnout;
    }
    req.seq = param->seq++;
    req.coder = coder;
    heap_push(param->wait_heap, req);
}

void shift_down(t_heap *hp, int index)
{
    int child1;
    int child2;
    t_heap_request temp = hp->requests[index];

    while (index * 2 + 1 < hp->size)
    {
        child1 = index * 2 + 1;
        child2 = index * 2 + 2;
        if (child2 < hp->size
            && hp->requests[child2].key < hp->requests[child1].key)
            child1 = child2;
        if (hp->requests[child1].key >= temp.key)
            break;
        hp->requests[index] = hp->requests[child1];
        index = child1;
    }
    hp->requests[index] = temp;
}

void shift_up(t_heap *hp, int index)
{
    int parent;
    t_heap_request temp;

    temp = hp->requests[index];
    while (index > 0)
    {
        parent = (index - 1) / 2;
        if (hp->requests[parent].key <= temp.key)
            break;
        hp->requests[index] = hp->requests[parent];
        index = parent;
    }
    hp->requests[index] = temp;
}

void heap_delete_at(t_heap *hp, int index)
{
    if (index < 0 || index >= hp->size)
        return;
    hp->requests[index] = hp->requests[hp->size - 1];
    hp->size--;
    if (hp->size == 0 || index == hp->size)
        return;
    shift_down(hp, index);
    shift_up(hp, index);
}
