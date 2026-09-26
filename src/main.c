#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef enum e_scheduler {
    FIFO,
    EDF
} t_scheduler;

typedef struct s_params
{
    int n_coders;
    long time_to_burnout;
    long time_to_debug;
    long time_to_refactor;
    int n_compiles_required;
    long dongle_cooldown;
    t_scheduler scheduler;
} t_params;

int main(int argc, char** argv) 
{
    if (argc != 8) {
        char err_message[] = "Insufficient number of arguments\n";
        write(1, err_message, strlen(err_message));
    }

    return 0;
}