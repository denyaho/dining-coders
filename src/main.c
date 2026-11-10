/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daogawa <daogawa@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 03:24:42 by daogawa           #+#    #+#             */
/*   Updated: 2026/10/04 03:49:38 by daogawa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

char ARG_ERR_MESSAGE[] = "Insufficient number of aruguments\n";
char PARSE_ERR_MESSAGE[] = "Invalid arguments included\n";

static void set_start_time(t_table *table)
{
	long start_time;
	start_time = get_current_time();
	table->start_time = start_time;
}

int	main(int argc, char	**argv)
{
	t_table	table;
	set_start_time(&table);
	if (argc != 9)
	{
		write(1, ARG_ERR_MESSAGE, strlen(ARG_ERR_MESSAGE));
		return (1);
	}
	if (parse_arg(argc, argv, &table) != 0)
	{
		write(1, PARSE_ERR_MESSAGE, strlen(PARSE_ERR_MESSAGE));
		return (1);
	}
	if (run_simulation(&table))
	{
		write(2, "Simulation ended\n", strlen("Simulation ended\n"));
		clean_table(&table);
		return (1);
	}
	clean_table(&table);
	destroy_coders(&table, table.thread_create_count);
	free(table.coders);
	free(table.dongles);

    return 0;
}
