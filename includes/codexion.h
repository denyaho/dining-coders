#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

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
} t_params;

int parse_arg(int argc, char** argv, t_params *p);