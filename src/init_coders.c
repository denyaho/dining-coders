/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_coders.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daogawa <daogawa@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 03:24:42 by daogawa           #+#    #+#             */
/*   Updated: 2026/10/04 03:49:38 by daogawa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	assign_dongles(t_table *table, int i)
{
	table->coders[i].left_dongle = &table->dongles[i];
	if (i == table->n_coders - 1)
		table->coders[i].right_dongle = &table->dongles[0];
	else
		table->coders[i].right_dongle = &table->dongles[i + 1];
}

static int	init_one_coder(t_table *table, int i)
{
	t_coder		*coder;

	coder = &table->coders[i];
	coder->id = i;
	coder->last_compile_start = 0;
	coder->compile_count = 0;
	if (pthread_mutex_init(&coder->lock, NULL) != 0)
		return (1);
	if (pthread_cond_init(&coder->cond, NULL) != 0)
	{
		pthread_mutex_destroy(&coder->lock);
		return (1);
	}
	coder->table = table;
	assign_dongles(table, i);
	return (0);
}

int	init_coders(t_table *table)
{
	int		index;
	t_coder	*coders;

	coders = malloc(sizeof(t_coder) * table->n_coders);
	if (!coders)
		return (1);
	table->coders = coders;
	index = 0;
	while (index < table->n_coders)
	{
		if (init_one_coder(table, index) != 0)
		{
			destroy_coders(table, index);
			return (1);
		}
		index++;
	}
	return (0);
}
