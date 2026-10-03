/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daogawa <daogawa@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 03:28:36 by daogawa           #+#    #+#             */
/*   Updated: 2026/10/04 05:58:37 by daogawa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>

typedef enum e_scheduler
{
	FIFO,
	EDF
}	t_scheduler;

typedef struct s_params
{
	int				n_coders;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	int				n_compiles_required;
	long			dongle_cooldown;
	int				stopped;
	pthread_mutex_t	stop_lock;
	pthread_mutex_t	print_lock;
	t_scheduler		scheduler;
	t_dongle		*dongles;
	t_coder			*coders;
}	t_params;

typedef struct s_dongle
{
	int				id;
	int				in_use; //0 -> not in use 1 -> in use
	long			avaiable_at;
	pthread_mutex_t	dongle_lock;
	pthread_cond_t	cond;
	t_params		*param;
}	t_dongle;
// avaiable_at and in_use are mutex protected

typedef struct s_coder
{
	int				id;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	long			last_compile_time; // monitor thread will check this to determine if the coder is starving
	int				compile_count; // monitor thread will check
	pthread_t		thread;
	pthread_mutex_t	coder_lock;

	t_params		*param;
}	t_coder;

typedef struct s_timespec
{
	time_t	tv_sec;
	long	tv_nsec;
}	t_timespec;

int	parse_arg(int argc, char **argv, t_params *p);
int		init_dongles(t_params *param);
void	free_dongles(t_params *param);
void	free_coders(t_params *param);
int		init_coders(t_params *param);
int		run_simulate(t_params *param);
void	*coder_run(void *arg);
void	*monitor_run(void *arg);
