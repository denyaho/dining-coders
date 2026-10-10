#include "codexion.h"

void print_status(t_params *param, t_print_status status, int coder_id)
{

    pthread_mutex_lock(&param->print_lock);
    switch (status)
    {
        case TAKEN_DONGLE:
            printf("%lld %d has taken a dongle\n", get_current_time(param), coder_id);
            break;
        case COMPILING:
            printf("%lld %d is compiling\n", get_current_time(param), coder_id);
            break;
        case DEBUGGING:
            printf("%lld %d is debugging\n", get_current_time(param), coder_id);
            break;
        case REFACTORING:
            printf("%lld %d is refactoring\n", get_current_time(param), coder_id);
            break;
        case BURNED_OUT:
            printf("%lld %d has burned out\n", get_current_time(param), coder_id);
            break;
    }
	pthread_mutex_unlock(&param->print_lock);
}