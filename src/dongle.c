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

#include "includes/codexion.h"

static int	init_one_dongle(t_params *param, int index)
{
	t_dongle	*dongle;

	dongle = &param->dongles[index];
	if (pthread_mutex_init(&dongle->dongle_lock, NULL) != 0)
		return (1);
	if (pthread_cond_init(&dongle->cond, NULL) != 0)
	{
		pthread_mutex_destroy(&dongle->dongle_lock);
		return (1);
	}
	dongle->id = index;
	dongle->in_use = 0;
	dongle->avaiable_at = 0;
	dongle->param = param;
	return (0);
}

static void	destroy_dongles(t_params *param, int count)
{
	int	index;

	index = 0;
	while (index < count)
	{
		pthread_mutex_destroy(&param->dongles[index].dongle_lock);
		pthread_cond_destroy(&param->dongles[index].cond);
		index++;
	}
}

void	free_dongles(t_params *param)
{
	destroy_dongles(param, param->n_coders);
	free(param->dongles);
	param->dongles = NULL;
}

int	init_dongles(t_params *param)
{
	int			index;
	t_dongle	*dongles;

	dongles = malloc(sizeof(t_dongle) * param->n_coders);
	if (!dongles)
		return (1);
	param->dongles = dongles;
	index = 0;
	while (index < param->n_coders)
	{
		if (init_one_dongle(param, index) != 0)
		{
			destroy_dongles(param, index);
			free(dongles);
			param->dongles = NULL;
			return (1);
		}
		index++;
	}
	return (0);
}
