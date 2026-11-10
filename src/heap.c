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

void heap_push(t_heap *heap, t_heap_request req)
{
    int tail_index;
    int parent_index;

    if (heap_is_full(heap))
        return;
    heap->size++;
    tail_index = heap->size - 1;
    while (tail_index > 0)
    {
        parent_index = (tail_index - 1 ) / 2;
        if (heap->requests[parent_index].key <= req.key) {
            break;
        }
        heap->requests[tail_index] = heap->requests[parent_index];
        tail_index = parent_index;
    }
    heap->requests[tail_index] = req;
    return;
}

static void sift_down(t_heap *heap, int index)
{
    int child;
    int right_child;
    t_heap_request item = heap->requests[index];

    while (index * 2 + 1 < heap->size)
    {
        child = index * 2 + 1;
        right_child = index * 2 + 2;
        if (right_child < heap->size
            && heap->requests[right_child].key < heap->requests[child].key)
            child = right_child;
        if (heap->requests[child].key >= item.key)
            break;
        heap->requests[index] = heap->requests[child];
        index = child;
    }
    heap->requests[index] = item;
}

static void sift_up(t_heap *heap, int index)
{
    int parent;
    t_heap_request item;

    item = heap->requests[index];
    while (index > 0)
    {
        parent = (index - 1) / 2;
        if (heap->requests[parent].key <= item.key)
            break;
        heap->requests[index] = heap->requests[parent];
        index = parent;
    }
    heap->requests[index] = item;
}

void heap_delete_at(t_heap *heap, int index)
{
    if (index < 0 || index >= heap->size)
        return;
    heap->requests[index] = heap->requests[heap->size - 1];
    heap->size--;
    if (heap->size == 0 || index == heap->size)
        return;
    sift_down(heap, index);
    sift_up(heap, index);
}
