/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daogawa <daogawa@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 00:00:00 by daogawa           #+#    #+#             */
/*   Updated: 2026/10/04 00:00:00 by daogawa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	dongles_in_use(t_coder *coder)
{
	return (coder->left_dongle->in_use || coder->right_dongle->in_use);
}

bool	dongles_cooled_down(t_coder *coder)
{
	t_table	*table;

	table = coder->table;
	return (get_process_time(table) >= coder->left_dongle->available_at
		&& get_process_time(table) >= coder->right_dongle->available_at);
}

void	signal_neighbours(t_table *table, int id)
{
	if (table->n_coders == 1)
		return ;
	if (id == 0)
		pthread_cond_signal(&table->coders[table->n_coders - 1].cond);
	else
		pthread_cond_signal(&table->coders[id - 1].cond);
	if (id == table->n_coders - 1)
		pthread_cond_signal(&table->coders[0].cond);
	else
		pthread_cond_signal(&table->coders[id + 1].cond);
}

int	take_dongles(t_coder *coder)
{
	t_table	*table;
	struct		timespec ts;

	table = coder->table;
	clock_gettime(CLOCK_REALTIME, &ts);
	pthread_mutex_lock(&table->table_lock);
	enqueue_request(coder);
	while (!has_priority(coder) || !dongles_cooled_down(coder)
		|| dongles_in_use(coder))
	{
		if (is_stopped(table))
			break ;
		if (!dongles_cooled_down(coder))
			pthread_cond_timedwait(&coder->cond, &table->table_lock,
				get_dongles_ready_time(&ts, coder));
		else
			pthread_cond_wait(&coder->cond, &table->table_lock);
	}
	heap_delete_at(table->wait_heap, heap_find(table->wait_heap, coder));
	if (is_stopped(table))
	{
		pthread_mutex_unlock(&table->table_lock);
		return (1);
	}
	coder->left_dongle->in_use = 1;
	coder->right_dongle->in_use = 1;
	pthread_mutex_unlock(&table->table_lock);
	print_status(coder->table, TAKEN_DONGLE, coder->id);
	return (0);
}

int	release_dongles(t_coder *coder)
{
	t_table	*table;
	int			id;

	id = coder->id;

	table = coder->table;
	pthread_mutex_lock(&table->table_lock);
	coder->left_dongle->in_use = 0;
	coder->left_dongle->available_at
		= get_process_time(table) + table->dongle_cooldown;
	coder->right_dongle->in_use = 0;
	coder->right_dongle->available_at
		= get_process_time(table) + table->dongle_cooldown;
	signal_neighbours(table, id);
	pthread_mutex_unlock(&table->table_lock);
	return (0);
}
