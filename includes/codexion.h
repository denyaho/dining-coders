#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

typedef enum e_scheduler {
    FIFO,
    EDF
} t_scheduler;

typedef struct s_params
{
    int n_coders;
    long time_to_burnout;
    long time_to_compile;
    long time_to_debug;
    long time_to_refactor;
    int n_compiles_required;
    long dongle_cooldown;
    t_scheduler scheduler;

    t_dongle *dongles;
    t_coder *coders;
} t_params;

typedef struct s_dongle
{
    int id;
    int in_use; //0 -> not in use 1 -> in use
    long avaiable_at; 
    pthread_mutex_t dongle_lock;
    pthread_cond_t cond;

    t_params *param;
} t_dongle;

typedef struct s_coder 
{
    int id;
    t_dongle *left_dongle;
    t_dongle *right_dongle;
    long last_compile_time;
    int compile_count;
    pthread_t thread;

    t_params *param;
} t_coder;


int parse_arg(int argc, char** argv, t_params *p);