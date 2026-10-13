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

void	free_heap(t_heap *hp)
{
	if (hp)
	{
		free(hp->requests);
		free(hp);
	}
}

void	clean_params(t_params *param)
{
	if (param)
	{
		pthread_mutex_destroy(&param->table_lock);
		pthread_mutex_destroy(&param->stop_lock);
		pthread_mutex_destroy(&param->print_lock);
		free_heap(param->wait_heap);
	}
}
