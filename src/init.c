/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daogawa <daogawa@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 03:24:42 by daogawa           #+#    #+#             */
/*   Updated: 2026/10/04 03:49:38 by daogawa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int init_table(t_table *table) {
    table->stopped = 0;
    table->next_seq = 0;
    table->thread_create_count = 0;
    table->wait_heap = heap_create(table->n_coders);
    if (table->wait_heap == NULL)
        return 1;
    if (pthread_mutex_init(&table->table_lock, NULL) != 0) {
        heap_free(table->wait_heap);
        return 1;
    }
    if (pthread_mutex_init(&table->stop_lock, NULL) != 0) {
        heap_free(table->wait_heap);
        pthread_mutex_destroy(&table->table_lock);
        return 1;
    }
    if (pthread_mutex_init(&table->print_lock, NULL) != 0) {
        heap_free(table->wait_heap);
        pthread_mutex_destroy(&table->stop_lock);
        pthread_mutex_destroy(&table->table_lock);
        return 1;
    }
    return 0;
}

static void	init_one_dongle(t_table *table, int index)
{
	t_dongle	*dongle;

	dongle = &table->dongles[index];
	dongle->id = index;
	dongle->in_use = 0;
	dongle->available_at = 0;
	dongle->table = table;
}

int	init_dongles(t_table *table)
{
	int			index;
	t_dongle	*dongles;

	dongles = malloc(sizeof(t_dongle) * table->n_coders);
	if (!dongles)
		return (1);
	table->dongles = dongles;
	index = 0;
	while (index < table->n_coders)
	{
		init_one_dongle(table, index);
		index++;
	}
	return (0);
}
