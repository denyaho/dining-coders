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

#include "includes/codexion.h"

static void	set_dongles(t_params *param, int i)
{
	param->coders[i].left_dongle = &param->dongles[i];
	if (i == param->n_coders - 1)
		param->coders[i].right_dongle = &param->dongles[0];
	else
		param->coders[i].right_dongle = &param->dongles[i + 1];
}

static void	init_one_coders(t_params *param, int i)
{
	t_coder		*coder;
	pthread_t	coder_thread;
	pthread_mutex_t lock;

	coder = &param->coders[i];
	coder->id = i;
	coder->last_compile_time = 0;
	coder->compile_count = 0;
	coder->thread = coder_thread;
	coder->coder_lock = lock;
	coder->param = param;
	set_dongles(param, i);
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
		init_one_coders(param, index);
		index++;
	}
	return (0);
}
