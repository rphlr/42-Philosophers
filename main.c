/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 11:42:08 by rrouille          #+#    #+#             */
/*   Updated: 2023/06/08 15:33:40 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*philosopher_action(void *ag)
{
	t_philosophers	*philosopher;

	philosopher = (t_philosophers *) ag;
	while (!philosopher->death)
	{
		philosopher->status = IS_THINKING;

		philosopher->status = IS_SLEEPING;

		philosopher->status = IS_EATING;
		pthread_mutex_lock(philosopher->left_fork);
		pthread_mutex_lock(philosopher->right_fork);


		pthread_mutex_unlock(philosopher->left_fork);
		pthread_mutex_unlock(philosopher->right_fork);
		if (philosopher->max_eat > 0 && philosopher->curr_philo >= \
		philosopher->max_eat)
		{
			philosopher->max_eat_reached = true;
			break ;
		}
	}
	return (NULL);
}

int	ft_atoi(const char *str)
{
	int	result;
	int	is_minus;

	result = 0;
	is_minus = 1;
	while (*str == '\t' || *str == '\n' || *str == '\v' || *str == '\f'
		|| *str == '\r' || *str == ' ')
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			is_minus *= -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result *= 10;
		result += *str++ - '0';
	}
	return (result * is_minus);
}

int	initiate_philosophers(int ac, char **av)
{
	int				i;
	int				max_eat;
	t_state			state;
	t_philosophers	*philosopher;
	pthread_t		*thread;

	max_eat = -1;
	if (ac == 6)
		max_eat = ft_atoi(av[ac - 1]);
	i = -1;
	state.forks = malloc(state.nbr_of_fork * sizeof(pthread_mutex_t));
	while (++i < state.nbr_of_fork)
		pthread_mutex_init(&state.forks[i], NULL);
	philosopher = malloc(state.nbr_of_philo * sizeof(t_philosophers));
	i = -1;
	while (++i < state.nbr_of_philo)
	{
		philosopher[i].curr_philo = i;
		philosopher[i].max_eat = max_eat;
		philosopher[i].death = false;
		philosopher[i].max_eat_reached = false;
		philosopher[i].status = IS_THINKING;
		philosopher[i].right_fork = &state.forks[i];
		philosopher[i].left_fork = &state.forks[(i + 1) % state.nbr_of_fork];
		philosopher[i].next = &philosopher[(i + 1) % state.nbr_of_philo];
	}
	thread = malloc(state.nbr_of_philo * sizeof(pthread_t));
	i = -1;
	while (++i < state.nbr_of_philo)
		pthread_create(&thread[i], NULL, philosopher_action, &philosopher[i]);
	i = -1;
	while (++i < state.nbr_of_philo)
		pthread_join(thread[i], NULL);
	free(state.forks);
	free(philosopher);
	free(thread);
	return (0);
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
		initiate_philosophers(ac, av);
	}
}
