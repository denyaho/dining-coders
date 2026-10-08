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

int	main(int argc, char	**argv)
{
	t_params	params;

	if (argc != 9)
	{
		write(1, ARG_ERR_MESSAGE, strlen(ARG_ERR_MESSAGE));
		return (1);
	}
	if (parse_arg(argc, argv, &params) != 0)
	{
		write(1, PARSE_ERR_MESSAGE, strlen(PARSE_ERR_MESSAGE));
		return (1);
	}
	if (run_simulate(&params))
	{
		write(1, "Simulation ended\n", strlen("Simulation ended\n"));
	}
    return 0;
}
