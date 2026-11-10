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

static void	*run_single_coder(t_coder *coder, t_table *table)
{
	print_status(table, TAKEN_DONGLE, coder->id);
	pthread_mutex_lock(&table->table_lock);
	while (!is_stopped(table))
		pthread_cond_wait(&coder->cond, &table->table_lock);
	pthread_mutex_unlock(&table->table_lock);
	return (NULL);
}

void	*coder_routine(void *arg)
{
	t_coder		*coder;
	t_table	*table;

	coder = (t_coder *)arg;
	table = coder->table;
	if (table->n_coders == 1)
		return (run_single_coder(coder, table));
	if (coder->id % 2 != 0)
		ft_usleep(1, coder->table);
	while (!is_stopped(coder->table))
	{
		if (take_dongles(coder))
			return (NULL);
		do_compile(coder);
		release_dongles(coder);
		do_debug(coder);
		do_refactor(coder);
	}
	return (NULL);
}
