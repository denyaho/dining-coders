/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daogawa <daogawa@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 03:33:56 by daogawa           #+#    #+#             */
/*   Updated: 2026/10/04 06:37:24 by daogawa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	set_dongles(t_params *param, int i)
{
	param->coders[i].left_dongle = &param->dongles[i];
	if (i == param->n_coders - 1)
		param->coders[i].right_dongle = &param->dongles[0];
	else
		param->coders[i].right_dongle = &param->dongles[i + 1];
}

static int	init_one_coders(t_params *param, int i)
{
	t_coder		*coder;

	coder = &param->coders[i];
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
	coder->param = param;
	set_dongles(param, i);
	return (0);
}

void destroy_coders(t_params *param, int count)
{
	int index;

	index = 0;
	while (index < count)
	{
		pthread_mutex_destroy(&param->coders[index].lock);
		pthread_cond_destroy(&param->coders[index].cond);
		index++;
	}
}


int	init_coders(t_params *param)
{
	int		index;
	t_coder	*coders;

	coders = malloc(sizeof(t_coder) * param->n_coders);
	if (!coders)
		return (1);
	param->coders = coders;
	index = 0;
	while (index < param->n_coders)
	{
		if (init_one_coders(param, index) != 0)
		{
			destroy_coders(param, index);
			return (1);
		}
		index++;
	}
	return (0);
}
