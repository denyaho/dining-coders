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

static void	init_one_dongle(t_params *param, int index)
{
	t_dongle	*dongle;

	dongle = &param->dongles[index];
	dongle->id = index;
	dongle->in_use = 0;
	dongle->available_at = 0;
	dongle->param = param;
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
		init_one_dongle(param, index);
		index++;
	}
	return (0);
}
