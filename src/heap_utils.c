/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daogawa <daogawa@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 03:24:42 by daogawa           #+#    #+#             */
/*   Updated: 2026/10/04 03:49:38 by daogawa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_heap *heap_create(int n)
{
    t_heap *heap = malloc(sizeof(t_heap));
    if (heap != NULL)
    {
        heap->size = 0;
        heap->capacity = n;
        heap->requests = malloc(sizeof(t_heap_request) * n);
        if (heap->requests == NULL)
        {
            free(heap);
            return NULL;
        }
    }
    return heap;
}

void	heap_free(t_heap *heap)
{
	if (heap)
	{
		free(heap->requests);
		free(heap);
	}
}

bool heap_is_full(t_heap *heap)
{
    if (heap->size == heap->capacity)
    {
        return true;
    }
    return false;
}

bool heap_is_empty(t_heap *heap)
{
    if (heap->size == 0)
    {
        return true;
    }
    return false;
}

int heap_find(t_heap *heap, t_coder *coder)
{
    int index;

    index = 0;
    while (index < heap->size)
    {
        if (heap->requests[index].coder == coder)
            return index;
        index++;
    }
    return -1; // or some other error value
}
