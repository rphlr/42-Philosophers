/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 15:33:06 by rrouille          #+#    #+#             */
/*   Updated: 2023/06/28 16:45:14 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# define ERR_INVALID_ARGS "⚠️\tOopsie daisy, we've got a snafu with some \
argument(s)! Give 'em a once-over and give it another go. Good luck! 😄\n"
# define USAGE "⚠️\tUsage: ./philosopher number_of_philosophers time_to_die \
time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n"
# define TOOK_FORK "🍴\t%ld\t[%d] has taken a fork.\n"
# define EATING "😋\t%ld\t[%d] is eating.\n"
# define SLEEPING "🛌🏼\t%ld\t[%d] is sleeping.\n"
# define THINKING "💭\t%ld\t[%d] is thinking.\n"
# define _DEAD "⚰️\t%ld\t[%d] died.\n"
# define MAX_THREAD "⚠️\tWarning: too much threads...\n"
# define STOP_SIMULATION "✅\tBrace yourselves, folks! The philosophers have \
decided to take a break from their feast. They've devoured their meals a grand \
total of [%d] time(s).\n"

// FORMATING
# define RED "\e[31m"
# define GREEN "\e[32m"
# define YELLOW "\e[33m"
# define BLUE "\e[34m"
# define MAGENTA "\e[35m"
# define CYAN "\e[36m"
# define WHITE "\e[37m"
# define RESET "\e[0m"
# define BOLD "\e[1m"
# define ITALIC "\e[3m"
# define UNDERLINE "\e[4m"

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

enum				e_state
{
	WAIT_START,
	WAIT_EAT,
	EAT_,
	SLEEP_,
	THINK_,
	DEAD_
};

enum				e_conditions
{
	CONTINUE,
	EAT_REACHED,
	DEAD
};

typedef struct s_running
{
	int				total_philosophers;
	int				time_until_death;
	int				time_to_eat;
	int				time_to_sleep;
	int				eating_count;
	int				error;
}					t_running;

typedef struct s_utils
{
	pthread_mutex_t	stop_mutex;
	int				should_stop;
	long int		start_time;
	int				has_finished;
	long int		time_until_death;
	void			*philosophers;
}					t_utils;

typedef struct s_philo
{
	int				id;
	long int		last_meal_time;
	long int		current_time;
	int				n_eat;
	t_running		running;
	t_utils			*utils;
	int				thread_id;
	int				fork_id;
	int				state;
	pthread_mutex_t	fork_mutex;
	pthread_mutex_t	time_mtx;
	pthread_mutex_t	n_eat_mutex;
	pthread_t		thread;
	void			*next;
	void			*prev;
}					t_philo;

void				init(int ac, char **av);
void				*life_cycle(void *p);
long int			get_time(void);
void				sleep_ms(long int ms, t_philo *philosophers);
long int			sec_to_ms(long int sec);
long int			usec_to_ms(long int microseconde);
long int			get_time_diff(long int start, long int end);
void				overseer(t_philo *philosophers);
long int			get_rel_time(long int start_time);
int					should_stop(t_philo *philosophers);
void				free_philosopher(t_philo *philosophers);
void				free_philosophers(t_philo *philosophers);
void				_eat(t_philo *philosophers);
void				_sleep(t_philo *philosophers);
void				_think(t_philo *philosophers);
void				_die(t_philo *philosophers);
int					can_pick_fork(t_philo *philosophers,
						pthread_mutex_t *fork_mutex);
int					can_eat(t_philo *philosophers);
void				print_status(t_philo *philosophers, char *status);
t_utils				create_utils(void);
t_running			create_config(int ac, char **av);
int					ft_atoi(const char *str);
void				*ft_calloc(size_t count, size_t size);
int					ft_isdigit(int number);
int					ft_isspace(int c);
int					ft_strlen(char *str);
void				create_thread(t_philo *philosophers);
t_philo				*last_philo(t_philo *philosophers);
t_philo				*create_philos(t_philo *prev, t_running running, int i,
						t_utils *utils);
t_running			create_running(int ac, char **av);
int					ft_strcmp(char *s1, char *s2);

#endif