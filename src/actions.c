/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daogawa <daogawa@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 03:24:42 by daogawa           #+#    #+#             */
/*   Updated: 2026/10/04 03:49:38 by daogawa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	do_compile(t_coder *coder)
{
	t_table	*table;

	table = coder->table;
	if (is_stopped(table))
		return ;
	pthread_mutex_lock(&coder->lock);
	coder->last_compile_start = get_process_time(table);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->lock);
	print_status(coder->table, COMPILING, coder->id);
	ft_usleep(coder->table->time_to_compile, coder->table);
}

void	do_debug(t_coder *coder)
{
	if (is_stopped(coder->table))
		return ;
	print_status(coder->table, DEBUGGING, coder->id);
	ft_usleep(coder->table->time_to_debug, coder->table);
}

void	do_refactor(t_coder *coder)
{
	if (is_stopped(coder->table))
		return ;
	print_status(coder->table, REFACTORING, coder->id);
	ft_usleep(coder->table->time_to_refactor, coder->table);
}
