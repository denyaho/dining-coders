#include <includes/codexion.h>


int _is_valid_number(char str[])
{
    int i;

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

long _ft_strtol(char str[])
{
    long num;
    int i;

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

int _parse_scheduler(char str[], t_scheduler *scheduler) {
    if (strcmp(str, "fifo") == 0){
        *scheduler = FIFO;
    } else if (strcmp(str, "edf") == 0) {
        *scheduler = EDF;
    } else {
        return 1;
    }
    return 0;
}

int parse_arg(int argc, char** argv, t_params *p) 
{
    int i;

    i = 1;
    while (i < argc - 1) {
        printf("input argument is %s\n", argv[i]);
        if (_is_valid_number(argv[i]) != 0) {
            return 1;
        }
        if (_ft_strtol(argv[i]) == -1) {
            return 1;
        }
        i++;
    }
    printf("number is %ld\n", _ft_strtol(argv[1]));
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
    return 0;
}
