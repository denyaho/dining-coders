#include <stdio.h>
#include <includes/codexion.h>
#include <stdlib.h>
#include <limits.h>


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
    }
    return num;
}

int _parse_scheduler(char str[]);

int parse_arg(int argc, char** argv, t_params *p) 
{

}
