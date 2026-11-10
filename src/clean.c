/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daogawa <daogawa@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 21:30:58 by daogawa           #+#    #+#             */
/*   Updated: 2026/10/13 21:42:21 by daogawa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_dongles(t_table *table)
{
	if (table->dongles)
		free(table->dongles);
}

void	destroy_coders(t_table *table, int count)
{
	int	index;

	index = 0;
	while (index < count)
	{
		pthread_mutex_destroy(&table->coders[index].lock);
		pthread_cond_destroy(&table->coders[index].cond);
		index++;
	}
}

void	clean_table(t_table *table)
{
	if (table)
	{
		pthread_mutex_destroy(&table->table_lock);
		pthread_mutex_destroy(&table->stop_lock);
		pthread_mutex_destroy(&table->print_lock);
		heap_free(table->wait_heap);
	}
}
