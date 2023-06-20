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
		if (!should_stop(philosophers) && philosophers->running.total_philosophers != 1)
			print_status(philosophers, BLUE""TOOK_FORK""RESET);
		return (1);
	}
	return (0);
}

void	_eat(t_philo *philosophers)
{
	t_philo	*next;

	next = philosophers->next;
	if (can_pick_fork(philosophers, &philosophers->fork_mutex)
		&& can_pick_fork(philosophers, &next->fork_mutex))
	{
		if (!should_stop(philosophers))
		{
			philosophers->state = EAT_;
			print_status(philosophers, YELLOW""EATING""RESET);
			philosophers->last_meal_time = get_time();
			if (philosophers->running.eating_count > 0)
				philosophers->n_eat += 1;
			sleep_ms(philosophers->running.time_to_eat, philosophers);
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
		sleep_ms(philosophers->running.time_to_sleep, philosophers);
	}
}

void	_die(t_philo *philosophers)
{
	philosophers->utils->should_stop = DEAD;
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

t_running	create_running(int ac, char **av)
{
	t_running	running;

	running.error = 0;
	running.total_philosophers = ft_atoi(av[1]);
	running.time_until_death = ft_atoi(av[2]);
	running.time_to_eat = ft_atoi(av[3]);
	running.time_to_sleep = ft_atoi(av[4]);
	if (running.time_to_sleep < 1 || running.time_to_eat < 1
		|| running.total_philosophers < 1 || running.time_until_death < 1)
		running.error = 1;
	if (ac == 6)
	{
		running.eating_count = ft_atoi(av[5]);
		if (running.eating_count < 1)
			running.error = 1;
	}
	else
		running.eating_count = 0;
	return (running);
}

t_philo	*create_philos(t_philo *prev, t_running running,
	int i, t_utils *utils)
{
	t_philo	*philosophers;

	philosophers = (t_philo *)ft_calloc(sizeof(t_philo), 1);
	if (!philosophers)
		return (NULL);
	philosophers->id = i + 1;
	philosophers->running = running;
	philosophers->last_meal_time = get_time();
	philosophers->utils = utils;
	philosophers->n_eat = 0;
	if (philosophers->id % 2)
		philosophers->state = WAIT_START;
	else
		philosophers->state = WAIT_EAT;
	pthread_mutex_init(&philosophers->fork_mutex, NULL);
	philosophers->prev = prev;
	if (++i < running.total_philosophers)
		philosophers->next = create_philos(philosophers, running, i, utils);
	else
		philosophers->next = NULL;
	return (philosophers);
}

void	create_thread(t_philo *philosophers)
{
	while (philosophers->id < philosophers->running.total_philosophers)
	{
		pthread_create(&philosophers->thread, NULL, &life_cycle, philosophers);
		philosophers = philosophers->next;
	}
	pthread_create(&philosophers->thread, NULL, &life_cycle, philosophers);
}

t_philo	*last_philo(t_philo *philosophers)
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
	t_philo		*philosophers;
	t_running	running;
	t_utils		utils;

	running = create_running(ac, av);
	if (running.error)
	{
		printf(RED""BOLD""UNDERLINE""ERR_INVALID_ARGS""RESET);
		return ;
	}
	utils = create_utils();
	if (running.total_philosophers >= 87353)
	{
		printf(RED""BOLD""UNDERLINE""MAX_THREAD""RESET);
		return ;
	}
	philosophers = create_philos(NULL, running, 0, &utils);
	philosophers = last_philo(philosophers);
	philosophers->utils->start_time = get_time();
	while (philosophers->id != 1)
		philosophers = philosophers->next;
	create_thread(philosophers);
	if (running.total_philosophers == 1)
	{
		print_status(philosophers, BLUE""TOOK_FORK""RESET);
		sleep_ms(running.time_until_death, philosophers);
		_die(philosophers);
	}
	else
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
	if (philosophers->utils->should_stop != CONTINUE
		&& ft_strcmp(status, RED""_DEAD""RESET))
		return ;
	else if (!ft_strcmp(status, RED""_DEAD""RESET))
		philosophers->utils->should_stop = DEAD;
	printf(status, get_rel_time(philosophers->utils->start_time),
		philosophers->id);
}

void	free_philosopher(t_philo *philosophers)
{
	pthread_mutex_destroy(&philosophers->fork_mutex);
	if (philosophers->running.total_philosophers > 1)
		pthread_join(philosophers->thread, NULL);
	if (philosophers)
		free(philosophers);
	philosophers = NULL;
}

void	free_philosophers(t_philo *philosophers)
{
	t_philo	*iter;
	int		total_philosophers;

	if (philosophers->running.total_philosophers == 1)
	{
		free_philosopher(philosophers);
		return ;
	}
	total_philosophers = philosophers->running.total_philosophers + 1;
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
		sleep_ms(philosophers->running.time_to_eat / 2, philosophers);
	while (1)
	{
		if (!should_stop(philosophers))
		{
			_eat(philosophers);
			if (should_stop(philosophers))
				break ;
			_sleep(philosophers);
			if (should_stop(philosophers))
				break ;
			_think(philosophers);
		}
		else
			break ;
	}
	return (NULL);
}

int	should_stop(t_philo *philosophers)
{
	if (!((get_time() - philosophers->last_meal_time)
			< philosophers->running.time_until_death))
		philosophers->utils->should_stop = DEAD;
	else if ((philosophers->n_eat > philosophers->running.eating_count
			&& philosophers->running.eating_count > 1))
		philosophers->utils->should_stop = EAT_REACHED;
	return (philosophers->utils->should_stop);
}

void	overseer(t_philo *philosophers)
{
	while (philosophers->id != 1)
		philosophers = philosophers->next;
	while (1)
	{
		if (philosophers->utils->should_stop == DEAD)
		{
			_die(philosophers);
			break ;
		}
		else if (philosophers->utils->should_stop == EAT_REACHED)
		{
			printf(GREEN""ITALIC""UNDERLINE""STOP_SIMULATION""RESET,
				philosophers->running.eating_count);
			break ;
		}
		philosophers = philosophers->next;
	}
	free_philosophers(philosophers);
}

t_utils	create_utils(void)
{
	t_utils	utils;

	utils.should_stop = 0;
	utils.has_finished = 0;
	utils.start_time = 0;
	utils.time_until_death = 0;
	return (utils);
}

long int	get_time(void)
{
	long int		time;
	long int		sec;
	long int		ms;
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	sec = sec_to_ms(current_time.tv_sec);
	ms = usec_to_ms(current_time.tv_usec);
	time = sec + ms;
	return (time);
}

void	sleep_ms(long int ms, t_philo *philosophers)
{
	long int	start_time;
	long int	timer_ms;

	start_time = get_time();
	timer_ms = get_time_diff(start_time, get_time());
	while (timer_ms < ms)
	{
		if (should_stop(philosophers))
		{
			philosophers->utils->should_stop = DEAD;
			return ;
		}
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

long int	usec_to_ms(long int ms)
{
	return (ms / 1000);
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
