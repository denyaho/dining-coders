/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daogawa <daogawa@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 06:39:51 by daogawa           #+#    #+#             */
/*   Updated: 2026/10/04 07:15:15 by daogawa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"


int get_dongle(t_dongle *dongle, t_coder *coder)
{
	t_params *param; 

	param = dongle->param;

	pthread_mutex_lock(&dongle->dongle_lock);
	schedule_heap(dongle, coder);
	while (dongle->in_use == 1 && !is_stopped(param) && get_current_time() < dongle->available_at)
		pthread_cond_wait(&dongle->cond, &dongle->dongle_lock);
	if (is_stopped(param))
	{
		pthread_mutex_unlock(&dongle->dongle_lock);
		return (0);
	}
	dongle->in_use = 1;
	pthread_mutex_unlock(&dongle->dongle_lock);
	return (1);
}

int release_dongle(t_dongle *dongle)
{
	t_params *param;

	param = dongle->param;
	pthread_mutex_lock(&dongle->dongle_lock);
	dongle->in_use = 0;
	dongle->available_at = get_current_time() + param->dongle_cooldown;
	pthread_cond_signal(&dongle->cond);
	pthread_mutex_unlock(&dongle->dongle_lock);
	return (0);
}

void do_debug(t_params *param)
{
	printf("debugging\n");
	usleep(param->time_to_debug);
}

void do_refactor(t_params *param)
{
	printf("refactoring\n");
	usleep(param->time_to_refactor);
}

void do_compile(t_coder *coder)
{
	printf("compiling\n");
	pthread_mutex_lock(&coder->coder_lock);
	coder->last_compile_start = get_current_time();
	coder->compile_count++;
	pthread_mutex_unlock(&coder->coder_lock);
	usleep(coder->param->time_to_compile);
}

void *coder_run(void *arg)
{

    t_coder *coder;
    coder = (t_coder *)arg;
	t_params *param;

	param = coder->param;
	while (!is_stopped(coder->param))
	{
		if (!get_dongle(coder->left_dongle, coder))
			return (NULL);
		if (!get_dongle(coder->right_dongle, coder))
		{
			release_dongle(coder->left_dongle);
			return (NULL);
		}
		do_compile(coder);
		release_dongle(coder->left_dongle);
		release_dongle(coder->right_dongle);
		do_debug(coder->param);
		do_refactor(coder->param);

	}

}

