/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daogawa <daogawa@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 03:24:42 by daogawa           #+#    #+#             */
/*   Updated: 2026/10/04 03:49:38 by daogawa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int is_stopped(t_table *table)
{
	int	stopped;

	pthread_mutex_lock(&table->stop_lock);
	stopped = table->stopped;
	pthread_mutex_unlock(&table->stop_lock);
	return	stopped;
}

void set_stopped(t_table *table)
{
	t_coder		*coder;
	int index;
	
	index = 0;
	pthread_mutex_lock(&table->stop_lock);
	table->stopped = 1;
	pthread_mutex_unlock(&table->stop_lock);
	
	pthread_mutex_lock(&table->table_lock);
	while (index < table->n_coders)
	{
		coder = &table->coders[index];
		pthread_cond_signal(&coder->cond);
		index++;
	}
	pthread_mutex_unlock(&table->table_lock);
}
