#include <includes/codexion.h>
#include <pthread.h>

char ARG_ERR_MESSAGE[] = "Insufficient number of aruguments\n";
char PARSE_ERR_MESSAGE[] = "Invalid arguments included\n";

int main(int argc, char** argv) 
{
    if (argc != 9) {
        write(1, ARG_ERR_MESSAGE, strlen(ARG_ERR_MESSAGE));
        return 1;
    }
    t_params params;
    if (parse_arg(argc, argv, &params) != 0)
    {
        write(1, PARSE_ERR_MESSAGE, strlen(PARSE_ERR_MESSAGE));
        return 1;
    }

    pthread_t thread[params.n_coders];

    printf("n_coder is %d\n", params.n_coders);
    printf("time_to_burnout is %ld\n", params.time_to_burnout);
    printf("time_to_compile is %ld\n", params.time_to_compile);
    printf("time_to_debug is %ld\n", params.time_to_debug);
    printf("time_to_refactor is %ld\n", params.time_to_refactor);
    printf("n_compiles_request is %ld\n", params.n_compiles_required);
    printf("dongle_cooldown is %ld\n", params.dongle_cooldown);
    printf("scheduler is %d\n", params.scheduler);

    
    return 0;
}