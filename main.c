/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 11:42:08 by rrouille          #+#    #+#             */
/*   Updated: 2023/05/25 16:18:37 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "philosophers.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct s_philo_state
{
	int		nbr_of_philo;
	int		nbr_of_fork;
	int		max_eat;
	bool	right_fork_free;
	bool	left_fork_free;
	bool	is_sleeping;
	bool	is_thinking;
	bool	is_eating;
}	t_philo_state;

void	init(int ac, char **av)
{
	if (ac == 6)
		t_philo_state.max_eat = atoi(av[ac - 1]);
	if (ac == 5)
		t_philo_state.max_eat = -1;
	t_philo_state.nbr_of_philo = atoi(av[2]);
	t_philo_state.nbr_of_fork = atoi(av[2]);
	t_philo_state.right_fork_free = true;
	t_philo_state.left_fork_free = true;
}

bool	check_int(char *cur_arg_value)
{
	while (*cur_arg_value)
	{
		if (!(*cur_arg_value >= '0' && *cur_arg_value <= '9'))
			return (false);
		cur_arg_value++;
	}
	return (true);
}

bool	check_args(int ac, char **av)
{
	int		cur_arg_nbr;
	char	*cur_arg_value;

	cur_arg_nbr = 0;
	while (++cur_arg_nbr != ac)
	{
		cur_arg_value = av[cur_arg_nbr];
		if (!check_int(cur_arg_value))
			return (false);
	}
	return (true);
}

bool	check_validity(int ac, char **av)
{
	if (ac == 5 || ac == 6)
		return (check_args(ac, av));
	return (false);
}

int	main(int ac, char **av)
{
	if (!check_validity(ac, av))
		printf("NOK\n");
	else
	{
		printf("OK\n");
		init(ac, av);
	}
}
