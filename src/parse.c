/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daogawa <daogawa@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 21:31:09 by daogawa           #+#    #+#             */
/*   Updated: 2026/10/13 21:31:10 by daogawa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int is_valid_number(char str[])
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

static long ft_strtol(char str[])
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

static int parse_scheduler(char str[], t_scheduler *scheduler) {
    if (strcmp(str, "fifo") == 0){
        *scheduler = FIFO;
    } else if (strcmp(str, "edf") == 0) {
        *scheduler = EDF;
    } else {
        return 1;
    }
    return 0;
}

int parse_arg(int argc, char** argv, t_table *table)
{
    int i;

    i = 1;
    while (i < argc - 1) {
        if (is_valid_number(argv[i]) != 0) {
            return 1;
        }
        if (ft_strtol(argv[i]) == -1) {
            return 1;
        }
        i++;
    }
    table->n_coders = ft_strtol(argv[1]);
    table->time_to_burnout = ft_strtol(argv[2]);
    table->time_to_compile = ft_strtol(argv[3]);
    table->time_to_debug = ft_strtol(argv[4]);
    table->time_to_refactor = ft_strtol(argv[5]);
    table->n_compiles_required = ft_strtol(argv[6]);
    table->dongle_cooldown = ft_strtol(argv[7]);
    if (parse_scheduler(argv[8], &table->scheduler) != 0){
        return 1;
    }
    return (init_table(table));
}
