/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 11:42:08 by rrouille          #+#    #+#             */
/*   Updated: 2023/06/09 14:05:40 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "philosophers.h"

// # include <stdio.h>
// # include <stdbool.h>
// # include <stdlib.h>
// # include <pthread.h>

// typedef enum e_status
// {
// 	IS_SLEEPING,
// 	IS_THINKING,
// 	IS_EATING,
//     IS_DEAD
// }	t_status;

// typedef struct s_philosopher
// {
// 	int						curr_philo;
// 	int						max_eat;
// 	bool					death;
// 	bool					max_eat_reached;
// 	pthread_mutex_t			*left_fork;
// 	pthread_mutex_t			*right_fork;
// 	t_status				status;
// 	struct s_philosopher	*next;
// 	pthread_mutex_t			*print_lock;
// 	t_state					*state;
// }	t_philosophers;

// typedef struct s_state
// {
// 	int						nbr_of_philo;
// 	int						nbr_of_fork;
// 	int						time_to_eat;
// 	int						time_to_sleep;
// 	int						time_to_die;
// 	pthread_mutex_t			*forks;
// 	pthread_mutex_t			*print_lock;
// }	t_state;

// void	print_message(t_philosophers *philosopher, char *message)
// {
// 	pthread_mutex_lock(philosopher->print_lock);
// 	printf("%d %s\n", philosopher->curr_philo + 1, message);
// 	pthread_mutex_unlock(philosopher->print_lock);
// }

// void	*philosopher_action(void *ag)
// {
// 	t_philosophers	*philosopher;

// 	philosopher = (t_philosophers *) ag;
// 	while (!philosopher->death)
// 	{
// 		print_message(philosopher, "is thinking");
// 		philosopher->status = IS_THINKING;

// 		philosopher->status = IS_SLEEPING;
// 		usleep(philosopher->state->time_to_sleep * 1000);

// 		philosopher->status = IS_EATING;
// 		pthread_mutex_lock(philosopher->left_fork);
// 		print_message(philosopher, "has taken a fork");
// 		pthread_mutex_lock(philosopher->right_fork);
// 		print_message(philosopher, "has taken a fork");

// 		usleep(philosopher->state->time_to_eat * 1000);

// 		pthread_mutex_unlock(philosopher->left_fork);
// 		pthread_mutex_unlock(philosopher->right_fork);
// 		if (philosopher->max_eat > 0 && philosopher->curr_philo >= \
// 		philosopher->max_eat)
// 		{
// 			philosopher->max_eat_reached = true;
// 			break ;
// 		}
// 	}
// 	return (NULL);
// }

// int	ft_atoi(const char *str)
// {
// 	int	result;
// 	int	is_minus;

// 	result = 0;
// 	is_minus = 1;
// 	while (*str == '\t' || *str == '\n' || *str == '\v' || *str == '\f'
// 		|| *str == '\r' || *str == ' ')
// 		str++;
// 	if (*str == '+' || *str == '-')
// 	{
// 		if (*str == '-')
// 			is_minus *= -1;
// 		str++;
// 	}
// 	while (*str >= '0' && *str <= '9')
// 	{
// 		result *= 10;
// 		result += *str++ - '0';
// 	}
// 	return (result * is_minus);
// }

// // int	initiate_philosophers(int ac, char **av)
// // {
// // 	int				i;
// // 	int				max_eat;
// // 	t_state			state;
// // 	t_philosophers	*philosopher;
// // 	pthread_t		*thread;

// // 	max_eat = -1;
// // 	if (ac == 6)
// // 		max_eat = ft_atoi(av[ac - 1]);
// // 	i = -1;
// // 	state.forks = malloc(state.nbr_of_fork * sizeof(pthread_mutex_t));
// // 	while (++i < state.nbr_of_fork)
// // 		pthread_mutex_init(&state.forks[i], NULL);
// // 	philosopher = malloc(state.nbr_of_philo * sizeof(t_philosophers));
// // 	i = -1;
// // 	while (++i < state.nbr_of_philo)
// // 	{
// // 		philosopher[i].curr_philo = i;
// // 		philosopher[i].max_eat = max_eat;
// // 		philosopher[i].death = false;
// // 		philosopher[i].max_eat_reached = false;
// // 		philosopher[i].status = IS_THINKING;
// // 		philosopher[i].right_fork = &state.forks[i];
// // 		philosopher[i].left_fork = &state.forks[(i + 1) % state.nbr_of_fork];
// // 		philosopher[i].next = &philosopher[(i + 1) % state.nbr_of_philo];
// // 	}
// // 	thread = malloc(state.nbr_of_philo * sizeof(pthread_t));
// // 	i = -1;
// // 	while (++i < state.nbr_of_philo)
// // 		pthread_create(&thread[i], NULL, philosopher_action, &philosopher[i]);
// // 	i = -1;
// // 	while (++i < state.nbr_of_philo)
// // 		pthread_join(thread[i], NULL);
// // 	free(state.forks);
// // 	free(philosopher);
// // 	free(thread);
// // 	return (0);
// // }

// int	initiate_philosophers(int ac, char **av)
// {
// 	int				i;
// 	int				max_eat;
// 	t_state			state;
// 	t_philosophers	*philosopher;
// 	pthread_t		*thread;

// 	max_eat = -1;
// 	if (ac == 6)
// 		max_eat = ft_atoi(av[ac - 1]);
// 	state.nbr_of_philo = ft_atoi(av[1]);
// 	state.nbr_of_fork = state.nbr_of_philo;
// 	state.time_to_eat = ft_atoi(av[2]);
// 	state.time_to_sleep = ft_atoi(av[3]);
// 	state.time_to_die = ft_atoi(av[4]);

// 	state.forks = malloc(state.nbr_of_fork * sizeof(pthread_mutex_t));
// 	state.print_lock = malloc(sizeof(pthread_mutex_t));
// 	pthread_mutex_init(state.print_lock, NULL);
// 	i = -1;
// 	while (++i < state.nbr_of_fork)
// 		pthread_mutex_init(&state.forks[i], NULL);

// 	philosopher = malloc(state.nbr_of_philo * sizeof(t_philosophers));
// 	i = -1;
// 	while (++i < state.nbr_of_philo)
// 	{
// 		philosopher[i].curr_philo = i;
// 		philosopher[i].max_eat = max_eat;
// 		philosopher[i].death = false;
// 		philosopher[i].max_eat_reached = false;
// 		philosopher[i].status = IS_THINKING;
// 		philosopher[i].right_fork = &state.forks[i];
// 		philosopher[i].left_fork = &state.forks[(i + 1) % state.nbr_of_fork];
// 		philosopher[i].next = &philosopher[(i + 1) % state.nbr_of_philo];
// 		philosopher[i].state = &state;
// 		philosopher[i].print_lock = state.print_lock;
// 	}

// 	thread = malloc(state.nbr_of_philo * sizeof(pthread_t));
// 	i = -1;
// 	while (++i < state.nbr_of_philo)
// 		pthread_create(&thread[i], NULL, philosopher_action, &philosopher[i]);

// 	i = -1;
// 	while (++i < state.nbr_of_philo)
// 		pthread_join(thread[i], NULL);

// 	free(state.forks);
// 	free(philosopher);
// 	free(thread);
// 	return (0);
// }

// bool	check_int(char *cur_arg_value)
// {
// 	while (*cur_arg_value)
// 	{
// 		if (!(*cur_arg_value >= '0' && *cur_arg_value <= '9'))
// 			return (false);
// 		cur_arg_value++;
// 	}
// 	return (true);
// }

// bool	check_args(int ac, char **av)
// {
// 	int		cur_arg_nbr;
// 	char	*cur_arg_value;

// 	cur_arg_nbr = 0;
// 	while (++cur_arg_nbr != ac)
// 	{
// 		cur_arg_value = av[cur_arg_nbr];
// 		if (!check_int(cur_arg_value))
// 			return (false);
// 	}
// 	return (true);
// }

// bool	check_validity(int ac, char **av)
// {
// 	if (ac == 5 || ac == 6)
// 		return (check_args(ac, av));
// 	return (false);
// }

// int	main(int ac, char **av)
// {
// 	if (!check_validity(ac, av))
// 		printf("NOK\n");
// 	else
// 	{
// 		printf("OK\n");
// 		initiate_philosophers(ac, av);
// 	}
// }


//#include <stdio.h>
//#include <stdbool.h>
//#include <stdlib.h>
//#include <pthread.h>
//#include <unistd.h>
//
//typedef enum		e_status
//{
//	IS_SLEEPING,
//	IS_THINKING,
//	IS_EATING,
//	IS_DEAD
//}	t_status;
//
//typedef struct		s_state
//{
//	int				num_philosophers;
//	int				time_to_die;
//	int				time_to_eat;
//	int				time_to_sleep;
//	int				num_meals;
//	bool			stop_simulation;
//	pthread_mutex_t	*forks;
//	pthread_mutex_t	print_lock;
//}	t_state;
//
//typedef struct		s_philosopher
//{
//	int				id;
//	int				left_fork;
//	int				right_fork;
//	int				eat_count;
//	t_status		status;
//	pthread_mutex_t	*forks;
//	pthread_mutex_t	*print_lock;
//	pthread_t		thread;
//	t_state			*state;
//	pthread_mutex_t eat_count_lock;
//}	t_philosopher;
//
//void	print_message(t_philosopher *philosopher, char *message)
//{
//	pthread_mutex_lock(&philosopher->state->print_lock);
//	printf("[%d] %s\n", philosopher->id, message);
//	pthread_mutex_unlock(&philosopher->state->print_lock);
//}
//
//void	take_forks(t_philosopher *philosopher)
//{
//	pthread_mutex_lock(&philosopher->forks[philosopher->left_fork]);
//	print_message(philosopher, "has taken left fork");
//	pthread_mutex_lock(&philosopher->forks[philosopher->right_fork]);
//	print_message(philosopher, "has taken right fork");
//}
//
//void	put_forks(t_philosopher *philosopher)
//{
//	pthread_mutex_unlock(&philosopher->forks[philosopher->left_fork]);
//	pthread_mutex_unlock(&philosopher->forks[philosopher->right_fork]);
//}
//
//void	eat(t_philosopher *philosopher)
//{
//	philosopher->status = IS_EATING;
//	print_message(philosopher, "is eating");
//	pthread_mutex_lock(&philosopher->eat_count_lock);
//	philosopher->eat_count++;
//	pthread_mutex_unlock(&philosopher->eat_count_lock);
//	usleep(philosopher->state->time_to_eat * 1000);
//}
//
//void	sleep_think(t_philosopher *philosopher)
//{
//	philosopher->status = IS_SLEEPING;
//	print_message(philosopher, "is sleeping");
//	usleep(philosopher->state->time_to_sleep * 1000);
//	philosopher->status = IS_THINKING;
//	print_message(philosopher, "is thinking");
//}
//
//void	*philosopher_action(void *arg)
//{
//	t_philosopher	*philosopher;
//	t_state			*state;
//	t_philosopher	*left_philosopher;
//	t_philosopher	*right_philosopher;
//	bool			all_meals_completed;
//
//	philosopher = (t_philosopher *)arg;
//	state = philosopher->state;
//	left_philosopher = &philosopher[(philosopher->id + state->num_philosophers - 1) % state->num_philosophers];
//	right_philosopher = &philosopher[(philosopher->id + 1) % state->num_philosophers];
//	while (!state->stop_simulation)
//	{
//		pthread_mutex_lock(&philosopher->eat_count_lock);
//		all_meals_completed = (philosopher->eat_count >= state->num_meals);
//		pthread_mutex_unlock(&philosopher->eat_count_lock);
//		if (philosopher->id % 2 == 0)
//		{
//			if (philosopher->id == 0)
//			{
//				take_forks(right_philosopher);
//				take_forks(philosopher);
//			}
//			else
//			{
//				take_forks(philosopher);
//				take_forks(right_philosopher);
//			}
//		}
//		else {
//			take_forks(left_philosopher);
//			take_forks(philosopher);
//		}
//		if (state->num_meals > 0 && philosopher->eat_count >= state->num_meals)
//			break;
//		sleep_think(philosopher);
//	}
//	philosopher->status = IS_DEAD;
//	return (NULL);
//}
//
//int	main(int argc, char **argv)
//{
//	if (argc != 5 && argc != 6)
//	{
//		printf("Usage: ./philosophers num_philosophers time_to_die time_to_eat time_to_sleep [num_meals]\n");
//		return (1);
//	}
//	t_state state;
//	state.num_philosophers = atoi(argv[1]);
//	state.time_to_die = atoi(argv[2]);
//	state.time_to_eat = atoi(argv[3]);
//	state.time_to_sleep = atoi(argv[4]);
//	state.num_meals = (argc == 6) ? atoi(argv[5]) : -1;
//	state.stop_simulation = false;
//	state.forks = calloc(state.num_philosophers, sizeof(pthread_mutex_t));
//	for (int i = 0; i < state.num_philosophers; i++) {
//		pthread_mutex_init(&state.forks[i], NULL);
//	}
//	pthread_mutex_init(&state.print_lock, NULL);
//	t_philosopher *philosophers = malloc(state.num_philosophers * sizeof(t_philosopher));
//	for (int i = 0; i < state.num_philosophers; i++)
//	{
//		pthread_mutex_init(&philosophers[i].eat_count_lock, NULL);
//		philosophers[i].id = i;
//		philosophers[i].left_fork = i;
//		philosophers[i].right_fork = (i + 1) % state.num_philosophers;
//		philosophers[i].eat_count = 0;
//		philosophers[i].status = IS_THINKING;
//		philosophers[i].forks = state.forks;
//		philosophers[i].print_lock = &state.print_lock;
//		philosophers[i].state = &state;
//		pthread_create(&philosophers[i].thread, NULL, philosopher_action, &philosophers[i]);
//	}
//	if (state.num_meals > 0)
//	{
//		while (!state.stop_simulation)
//		{
//			bool	all_meals_completed = true;
//			for (int i = 0; i < state.num_philosophers; i++)
//			{
//				if (philosophers[i].eat_count < state.num_meals)
//				{
//					all_meals_completed = false;
//					break;
//				}
//			}
//			if (all_meals_completed) {
//				usleep(state.time_to_die * 1000);
//				state.stop_simulation = true;
//			}
//		}
//	}
//	else
//	{
//		usleep(state.time_to_die * 1000);
//		state.stop_simulation = true;
//	}
//	for (int i = 0; i < state.num_philosophers; i++)
//		pthread_join(philosophers[i].thread, NULL);
//	for (int i = 0; i < state.num_philosophers; i++)
//		pthread_mutex_destroy(&state.forks[i]);
//	pthread_mutex_destroy(&state.print_lock);
//	free(state.forks);
//	free(philosophers);
//	return (0);
//}



#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

typedef enum		e_status
{
	IS_SLEEPING,
	IS_THINKING,
	IS_EATING,
	IS_DEAD
}	t_status;

typedef struct		s_state
{
	int				num_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_meals;
	bool			stop_simulation;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
}	t_state;

typedef struct		s_philosopher
{
	int				id;
	int				left_fork;
	int				right_fork;
	int				eat_count;
	t_status		status;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*print_lock;
	pthread_t		thread;
	t_state			*state;
	pthread_mutex_t eat_count_lock;
}	t_philosopher;

void	put_forks(t_philosopher *philosopher);

void	print_message(t_philosopher *philosopher, char *message)
{
	pthread_mutex_lock(&philosopher->state->print_lock);
	printf("[%d] %s\n", philosopher->id, message);
	pthread_mutex_unlock(&philosopher->state->print_lock);
}

void	take_forks(t_philosopher *philosopher)
{
	int first_fork = rand() % 2;
	bool success = false;

	if (first_fork == 0) {
		if (pthread_mutex_trylock(&philosopher->forks[philosopher->left_fork]) == 0) {
			print_message(philosopher, "has taken left fork");
			if (pthread_mutex_trylock(&philosopher->forks[philosopher->right_fork]) == 0)
			{
				print_message(philosopher, "has taken right fork");
				success = true;
			}
			else
				pthread_mutex_unlock(&philosopher->forks[philosopher->left_fork]);
		}
	} else {
		if (pthread_mutex_trylock(&philosopher->forks[philosopher->right_fork]) == 0) {
			print_message(philosopher, "has taken right fork");
			if (pthread_mutex_trylock(&philosopher->forks[philosopher->left_fork]) == 0) {
				print_message(philosopher, "has taken left fork");
				success = true;
			} else {
				pthread_mutex_unlock(&philosopher->forks[philosopher->right_fork]);
			}
		}
	}

	// If we didn't successfully grab the forks, release any locks we have acquired and try again later
	if (!success) {
//		put_forks(philosopher);
//		usleep(philosopher->state->time_to_sleep * 1000);
	}
}

void	put_forks(t_philosopher *philosopher)
{
	pthread_mutex_unlock(&philosopher->forks[philosopher->left_fork]);
	pthread_mutex_unlock(&philosopher->forks[philosopher->right_fork]);
}

void	eat(t_philosopher *philosopher)
{
	philosopher->status = IS_EATING;
	print_message(philosopher, "is eating");
	pthread_mutex_lock(&philosopher->eat_count_lock);
	philosopher->eat_count++;
	pthread_mutex_unlock(&philosopher->eat_count_lock);
	usleep(philosopher->state->time_to_eat * 1000);
}

void	sleep_think(t_philosopher *philosopher)
{
	philosopher->status = IS_SLEEPING;
	print_message(philosopher, "is sleeping");
	usleep(philosopher->state->time_to_sleep * 1000);
	philosopher->status = IS_THINKING;
	print_message(philosopher, "is thinking");
}

void	*philosopher_action(void *arg)
{
	t_philosopher	*philosopher;
	t_state			*state;
	t_philosopher	*left_philosopher;
	t_philosopher	*right_philosopher;
	bool			all_meals_completed;

	philosopher = (t_philosopher *)arg;
	state = philosopher->state;
	left_philosopher = &philosopher[(philosopher->id + state->num_philosophers - 1) % state->num_philosophers];
	right_philosopher = &philosopher[(philosopher->id + 1) % state->num_philosophers];
	// Simulation loop
	while (true) {
		// Check if all meals have been completed
		pthread_mutex_lock(&philosopher->eat_count_lock);
		all_meals_completed = (philosopher->eat_count >= state->num_meals) && (state->num_meals != -1);
		pthread_mutex_unlock(&philosopher->eat_count_lock);
		if (all_meals_completed) {
			state->stop_simulation = true;
			break;
		}

		// Take forks and eat
		take_forks(philosopher);
		if (!state->stop_simulation) {
			if (pthread_mutex_trylock(&philosopher->eat_count_lock) == 0) {
				if (philosopher->eat_count >= state->num_meals && state->num_meals != -1) {
					state->stop_simulation = true;
				}
				pthread_mutex_unlock(&philosopher->eat_count_lock);
			}
			if (!state->stop_simulation) {
				eat(philosopher);
				put_forks(philosopher);
			}
		}

		// Sleep and think
		if (!state->stop_simulation) {
			sleep_think(philosopher);
		}
	}

	return (NULL);
}

int		main(int argc, char **argv)
{
	t_state			state;
	t_philosopher	*philosophers;
	pthread_mutex_t	*forks;
	int				i;

	if (argc < 5 || argc > 6) {
		printf("Usage: ./philo num_philosophers time_to_die time_to_eat time_to_sleep [num_meals]\n");
		return (1);
	}

	// Initialize state
	state.num_philosophers = atoi(argv[1]);
	state.time_to_die = atoi(argv[2]);
	state.time_to_eat = atoi(argv[3]);
	state.time_to_sleep = atoi(argv[4]);
	state.num_meals = (argc == 6) ? atoi(argv[5]) : -1;
	state.stop_simulation = false;

	// Initialize forks
	forks = malloc(sizeof(pthread_mutex_t) * state.num_philosophers);
	for (i = 0; i < state.num_philosophers; i++) {
		pthread_mutex_init(&forks[i], NULL);
	}

	// Initialize philosophers
	philosophers = malloc(sizeof(t_philosopher) * state.num_philosophers);
	for (i = 0; i < state.num_philosophers; i++) {
		philosophers[i].id = i + 1;
		philosophers[i].left_fork = i;
		philosophers[i].right_fork = (i + 1) % state.num_philosophers;
		philosophers[i].eat_count = 0;
		philosophers[i].status = IS_THINKING;
		philosophers[i].forks = forks;
		philosophers[i].print_lock = &state.print_lock;
		philosophers[i].state = &state;
		pthread_mutex_init(&philosophers[i].eat_count_lock, NULL);
	}

	// Start philosophers
	for (i = 0; i < state.num_philosophers; i++) {
		pthread_create(&philosophers[i].thread, NULL, philosopher_action, &philosophers[i]);
	}

	// Wait for philosophers to finish
	for (i = 0; i < state.num_philosophers; i++) {
		pthread_join(philosophers[i].thread, NULL);
	}

	// Cleanup
	free(forks);
	free(philosophers);

	return (0);
}
