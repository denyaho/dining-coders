#include "codexion.h"


static int _is_valid_number(char str[])
{
    unsigned int i;

    if (strlen(str) == 0){
        return 1;
    }
    i = 0;
    while (i < strlen(str)){
        if (str[i] < '0' || str[i] > '9'){
            return 1;
        }
        i++;
    }
    return 0;
}

static long _ft_strtol(char str[])
{
    long num;
    unsigned int i;

    i = 0;
    num = 0;
    while (i < strlen(str)) {
        if (num > (LONG_MAX - (str[i] - '0')) / 10) {
            return -1;
        }
        num = num * 10 + (str[i] - '0');
        i++;
    }
    return num;
}

static int _parse_scheduler(char str[], t_scheduler *scheduler) {
    if (strcmp(str, "fifo") == 0){
        *scheduler = FIFO;
    } else if (strcmp(str, "edf") == 0) {
        *scheduler = EDF;
    } else {
        return 1;
    }
    return 0;
}

static int _init_params(t_params *p) {
    p->stopped = 0;
    p->seq = 0;
    if (pthread_mutex_init(&p->table_lock, NULL) != 0) {
        return 1;
    }
    p->wait_heap = make_heap(p->n_coders);
    if (p->wait_heap == NULL) {
        return 1;
    }
    if (pthread_mutex_init(&p->stop_lock, NULL) != 0) {
        free(p->wait_heap);
        pthread_mutex_destroy(&p->table_lock);
        return 1;
    }
    if (pthread_mutex_init(&p->print_lock, NULL) != 0) {
        free(p->wait_heap);
        pthread_mutex_destroy(&p->stop_lock);
        pthread_mutex_destroy(&p->table_lock);
        return 1;
    }
    return 0;
}

int parse_arg(int argc, char** argv, t_params *p) 
{
    int i;

    i = 1;
    while (i < argc - 1) {
        if (_is_valid_number(argv[i]) != 0) {
            return 1;
        }
        if (_ft_strtol(argv[i]) == -1) {
            return 1;
        }
        i++;
    }
    p->n_coders = _ft_strtol(argv[1]);
    p->time_to_burnout = _ft_strtol(argv[2]);
    p->time_to_compile = _ft_strtol(argv[3]);
    p->time_to_debug = _ft_strtol(argv[4]);
    p->time_to_refactor = _ft_strtol(argv[5]);
    p->n_compiles_required = _ft_strtol(argv[6]);
    p->dongle_cooldown = _ft_strtol(argv[7]);
    if (_parse_scheduler(argv[8], &p->scheduler) != 0){
        return 1;
    }
    return (_init_params(p));
}
