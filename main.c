/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 21:13:14 by dgloriod          #+#    #+#             */
/*   Updated: 2023/06/17 08:01:35 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int ac, char **av)
{
	if (ac < 5 || ac > 6)
	{
		printf(RED""BOLD""UNDERLINE""USAGE""RESET);
		return (0);
	}
	init(ac, av);
	return (0);
}

int	can_pick_fork(t_philo *philosophers, pthread_mutex_t *fork_mutex)
{
	if (!fork_mutex)
		return (0);
	if (!pthread_mutex_lock(fork_mutex))
	{
		if (!should_stop(philosophers))
			print_status(philosophers, BLUE""TOOK_FORK""RESET);
		return (1);
	}
	return (0);
}

int	can_eat(t_philo *philosophers)
{
	return (philosophers->n_eat < philosophers->config.eating_count);
}

void	_eat(t_philo *philosophers)
{
	t_philo	*next;

	next = philosophers->next;
	if (can_pick_fork(philosophers, &philosophers->fork_mutex) && \
	can_pick_fork(philosophers, &next->fork_mutex))
	{
		if (!should_stop(philosophers))
		{
			philosophers->state = EAT_;
			print_status(philosophers, YELLOW""EATING""RESET);
			philosophers->last_meal_time = get_time();
			if (philosophers->config.eating_count > 0)
				philosophers->n_eat += 1;
			sleep_ms(philosophers->config.time_to_eat);
		}
	}
	pthread_mutex_unlock(&philosophers->fork_mutex);
	pthread_mutex_unlock(&next->fork_mutex);
}

void	_sleep(t_philo *philosophers)
{
	if (!should_stop(philosophers))
	{
		philosophers->state = SLEEP_;
		print_status(philosophers, MAGENTA""SLEEPING""RESET);
		sleep_ms(philosophers->config.time_to_sleep);
	}
}

void	_die(t_philo *philosophers)
{
	philosophers->table->should_stop = DEAD;
	usleep(1000);
	print_status(philosophers, RED""_DEAD""RESET);
}

void	_think(t_philo *philosophers)
{
	if (!should_stop(philosophers))
	{
		philosophers->state = THINK_;
		print_status(philosophers, CYAN""THINKING""RESET);
	}
}

t_philo_config	create_config(int ac, char **av)
{
	t_philo_config	config;

	config.error = 0;
	config.total_philosophers = ft_atoi(av[1]);
	config.time_until_death = ft_atoi(av[2]);
	config.time_to_eat = ft_atoi(av[3]);
	config.time_to_sleep = ft_atoi(av[4]);
	if (config.time_to_sleep < 1 || config.time_to_eat < 1
		|| config.total_philosophers < 1 || config.time_until_death < 1)
		config.error = 1;
	if (ac == 6)
	{
		config.eating_count = ft_atoi(av[5]);
		if (config.eating_count < 1)
			config.error = 1;
	}
	else
		config.eating_count = 0;
	return (config);
}

t_philo	*create_philos(t_philo *prev, t_philo_config config, \
int i, t_table *table)
{
	t_philo	*philosophers;

	philosophers = (t_philo *)ft_calloc(sizeof(t_philo), 1);
	if (!philosophers)
		return (NULL);
	philosophers->id = i + 1;
	philosophers->config = config;
	philosophers->last_meal_time = get_time();
	philosophers->table = table;
	philosophers->n_eat = 0;
	if (philosophers->id % 2)
		philosophers->state = WAIT_START;
	else
		philosophers->state = WAIT_EAT;
	pthread_mutex_init(&philosophers->fork_mutex, NULL);
	philosophers->prev = prev;
	if (++i < config.total_philosophers)
		philosophers->next = create_philos(philosophers, config, i, table);
	else
		philosophers->next = NULL;
	return (philosophers);
}

void	create_thread(t_philo *philosophers)
{
	while (philosophers->id < philosophers->config.total_philosophers)
	{
		pthread_create(&philosophers->thread, NULL, &life_cycle, philosophers);
		philosophers = philosophers->next;
	}
	pthread_create(&philosophers->thread, NULL, &life_cycle, philosophers);
}

t_philo	*save_last(t_philo *philosophers)
{
	t_philo	*last;

	last = philosophers;
	while (last->next)
		last = last->next;
	last->next = philosophers;
	philosophers->prev = last;
	return (philosophers);
}

void	init(int ac, char **av)
{
	t_philo			*philosophers;
	t_philo_config	config;
	t_table			table;

	config = create_config(ac, av);
	if (config.error)
	{
		printf(RED""BOLD""UNDERLINE""ERR_INVALID_ARGS""RESET);
		return ;
	}
	table = create_table();
	philosophers = create_philos(NULL, config, 0, &table);
	philosophers = save_last(philosophers);
	philosophers->table->start_time = get_time();
	while (philosophers->id != 1)
		philosophers = philosophers->next;
	create_thread(philosophers);
	overseer(philosophers);
}

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 && *s2)
	{
		if (*s1++ - *s2++)
			return (*--s1 - *--s2);
	}
	return (*s1 - *s2);
}

void	print_status(t_philo *philosophers, char *status)
{
	if (philosophers->table->should_stop != CONTINUE && ft_strcmp(status, RED""_DEAD""RESET))
		return;
	else if (!ft_strcmp(status, RED""_DEAD""RESET))
		philosophers->table->should_stop = DEAD;
	printf(status, get_rel_time(philosophers->table->start_time), philosophers->id);
}

void	free_philosopher(t_philo *philosophers)
{
	pthread_mutex_destroy(&philosophers->fork_mutex);
	pthread_join(philosophers->thread, NULL);
	if (philosophers)
		free(philosophers);
	philosophers = NULL;
}

void	free_philosophers(t_philo *philosophers)
{
	t_philo	*iter;
	int		total_philosophers;

	total_philosophers = philosophers->config.total_philosophers + 1;
	while (--total_philosophers && philosophers)
	{
		if (philosophers->next)
			iter = philosophers->next;
		free_philosopher(philosophers);
		if (iter)
			philosophers = iter;
	}
}

void	*life_cycle(void *p)
{
	t_philo	*philosophers;

	philosophers = (t_philo *) p;
	if (philosophers->state == WAIT_START)
		sleep_ms(philosophers->config.time_to_eat / 2);
	while (1)
	{
		if (!should_stop(philosophers))
		{
			_eat(philosophers);
			_sleep(philosophers);
			_think(philosophers);
		}
		else
			break ;
	}
	return (NULL);
}

int	should_stop(t_philo *philosophers)
{
	if (!((get_time() - philosophers->last_meal_time) <
		philosophers->config.time_until_death))
		philosophers->table->should_stop = DEAD;
	else if ((philosophers->n_eat >= philosophers->config.eating_count &&
	philosophers->config.eating_count >= 1))
		philosophers->table->should_stop = EAT_REACHED;
	else
		philosophers->table->should_stop = CONTINUE;
	return (philosophers->table->should_stop);
}

void	overseer(t_philo *philosophers)
{
	while (philosophers->id != 1)
		philosophers = philosophers->next;
	while (1)
	{
		if (philosophers->table->should_stop == DEAD)
		{
			_die(philosophers);
			break ;
		}
		else if (philosophers->table->should_stop == EAT_REACHED)
		{
			usleep(1000);
			printf(GREEN""ITALIC""UNDERLINE""STOP_SIMULATION""RESET,
				   philosophers->config.eating_count);
			break ;
		}
		philosophers = philosophers->next;
	}
	free_philosophers(philosophers);
}

t_table	create_table(void)
{
	t_table	table;

	table.should_stop = 0;
	table.has_finished = 0;
	table.start_time = 0;
	table.time_until_death = 0;
	return (table);
}

long int	get_time(void)
{
	long int		time;
	long int		sec;
	long int		microseconde;
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	sec = sec_to_ms(current_time.tv_sec);
	microseconde = usec_to_ms(current_time.tv_usec);
	time = sec + microseconde;
	return (time);
}

void	sleep_ms(long int ms)
{
	long int	start_time;
	long int	timer_ms;

	start_time = get_time();
	timer_ms = get_time_diff(start_time, get_time());
	while (timer_ms < ms)
	{
		timer_ms = get_time_diff(start_time, get_time());
		usleep(ms / 10);
	}
}

long int	get_rel_time(long int start_time)
{
	return (get_time() - start_time);
}

long int	sec_to_ms(long int sec)
{
	return (sec * 1000);
}

long int	usec_to_ms(long int microseconde)
{
	return (microseconde / 1000);
}

long int	get_time_diff(long int start, long int end)
{
	return (end - start);
}

int	ft_atoi(const char *str)
{
	long long	nbr;
	int			i;
	int			is_negative;

	nbr = 0;
	i = 0;
	is_negative = 1;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			is_negative = -1;
		i++;
	}
	while (ft_isdigit(str[i]))
	{
		nbr *= 10;
		nbr += (int) str[i] - '0';
		i++;
	}
	return (nbr * is_negative);
}

void	*ft_calloc(size_t count, size_t size)
{
	char	*calloc;
	size_t	total;
	int		i;

	i = 0;
	total = count * size;
	calloc = malloc(total);
	if (!calloc)
		return (0);
	while (total--)
		calloc[i++] = '\0';
	return ((void *) calloc);
}

int	ft_isdigit(int number)
{
	return (number >= '0' && number <= '9');
}

int	ft_isspace(int c)
{
	return (c == '\t' || c == '\n' || c == '\v'
			|| c == '\f' || c == '\r' || c == ' ');
}

int	ft_strlen(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		;
	return (i);
}
