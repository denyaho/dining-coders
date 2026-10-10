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

typedef struct s_params t_params;
typedef struct s_dongle t_dongle;
typedef struct s_coder t_coder;

typedef enum e_print_status
{
	TAKEN_DONGLE,
	COMPILING,
	DEBUGGING,
	REFACTORING,
	BURNED_OUT,
}	t_print_status;

typedef enum e_scheduler
{
	FIFO,
	EDF
}	t_scheduler;

typedef struct s_dongle
{
	int				id;
	int				in_use; //0 -> not in use 1 -> in use
	long			available_at;
	t_params		*param;

}	t_dongle;
// avaiable_at and in_use are mutex protected

typedef struct s_coder
{
	int				id;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	long			last_compile_start; // monitor thread will check this to determine if the coder is starving
	int				compile_count; // monitor thread will check
	pthread_mutex_t lock;
	pthread_t		thread;
	pthread_cond_t	cond;

	t_params		*param;
}	t_coder;

typedef struct s_heap_request
{
	long key;
	long seq;
	t_coder			*coder;
}	t_heap_request;

typedef struct s_heap
{
    int size;
    int limit;
	t_heap_request *requests;
}	t_heap;

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
	int				seq;
	long			start_time;
	int				thread_create_count;

	t_heap			*wait_heap;
	pthread_mutex_t table_lock;
	pthread_mutex_t	stop_lock;
	pthread_mutex_t	print_lock;
	t_scheduler		scheduler;
	t_dongle		*dongles;
	t_coder			*coders;
}	t_params;


typedef struct s_timespec
{
	time_t	tv_sec;
	long	tv_nsec;
}	t_timespec;

int	parse_arg(int argc, char **argv, t_params *p);
int		init_dongles(t_params *param);
void	free_dongles(t_params *param);
void	destroy_coders(t_params *param, int count);
int		init_coders(t_params *param);
int		run_simulate(t_params *param);
void	*coder_run(void *arg);
void	*monitor_run(void *arg);
t_heap_request heap_top(t_heap *hp);
int find_heap(t_heap *hp, t_coder *coder);
void schedule_heap(t_coder *coder);
bool heap_pop(t_heap *hp);
int is_stopped(t_params *param);
long long get_process_time(t_params *param);
long long get_current_time();
t_heap *make_heap(int n);
void print_status(t_params *param, t_print_status status, int coder_id);
struct timespec *get_shorter_cooldown_dongle(struct timespec *ts, t_coder *coder);
void heap_delete_at(t_heap *hp, int index);
void ft_usleep(long duration_sleep, t_params *params);
void clean_params(t_params *param);
void    free_heap(t_heap *hp);
void set_stopped(t_params *param);