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

typedef struct s_table t_table;
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
	t_table		*table;

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

	t_table		*table;
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
    int capacity;
	t_heap_request *requests;
}	t_heap;

typedef struct s_table
{
	int				n_coders;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	int				n_compiles_required;
	long			dongle_cooldown;
	int				stopped;
	int				next_seq;
	long			start_time;
	int				thread_create_count;

	t_heap			*wait_heap;
	pthread_mutex_t table_lock;
	pthread_mutex_t	stop_lock;
	pthread_mutex_t	print_lock;
	t_scheduler		scheduler;
	t_dongle		*dongles;
	t_coder			*coders;
}	t_table;


/* parse.c */
int				parse_arg(int argc, char **argv, t_table *table);

/* init.c / init_coders.c */
int				init_table(t_table *table);
int				init_dongles(t_table *table);
int				init_coders(t_table *table);

/* clean.c */
void			free_dongles(t_table *table);
void			destroy_coders(t_table *table, int count);
void			clean_table(t_table *table);

/* simulate.c / coder_routine.c / monitor.c */
int				run_simulation(t_table *table);
void			*coder_routine(void *arg);
void			*monitor_routine(void *arg);

/* actions.c */
void			do_compile(t_coder *coder);
void			do_debug(t_coder *coder);
void			do_refactor(t_coder *coder);

/* dongle.c */
bool			dongles_in_use(t_coder *coder);
bool			dongles_cooled_down(t_coder *coder);
void			signal_neighbours(t_table *table, int id);
int				take_dongles(t_coder *coder);
int				release_dongles(t_coder *coder);

/* scheduler.c */
void			enqueue_request(t_coder *coder);
int				has_priority(t_coder *coder);

/* heap.c / heap_utils.c */
void			heap_push(t_heap *heap, t_heap_request req);
void			heap_delete_at(t_heap *heap, int index);
t_heap			*heap_create(int n);
void			heap_free(t_heap *heap);
bool			heap_is_full(t_heap *heap);
bool			heap_is_empty(t_heap *heap);
int				heap_find(t_heap *heap, t_coder *coder);

/* stop.c */
int				is_stopped(t_table *table);
void			set_stopped(t_table *table);

/* print.c */
void			print_status(t_table *table, t_print_status status,
					int coder_id);

/* time.c */
long long		get_current_time(void);
long long		get_process_time(t_table *table);
struct timespec	*get_dongles_ready_time(struct timespec *ts, t_coder *coder);
void			ft_usleep(long duration_ms, t_table *table);
