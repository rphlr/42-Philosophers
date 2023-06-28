/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 13:20:35 by rrouille          #+#    #+#             */
/*   Updated: 2023/06/28 18:18:15 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	should_stop(t_philo *philosophers)
{
	int				should_stop_value;
	long int		time_cpy;
	int				n_eat;

	pthread_mutex_lock(&philosophers->utils->stop_mutex);
	pthread_mutex_lock(&philosophers->time_mtx);
	time_cpy = philosophers->last_meal_time;
	pthread_mutex_unlock(&philosophers->time_mtx);
	pthread_mutex_lock(&philosophers->n_eat_mutex);
	n_eat = philosophers->n_eat;
	pthread_mutex_unlock(&philosophers->n_eat_mutex);
	if (!((get_time() - time_cpy)
			< philosophers->running.time_until_death))
		philosophers->utils->should_stop = DEAD;
	else if ((n_eat > philosophers->running.eating_count
			&& philosophers->running.eating_count > 1))
		philosophers->utils->should_stop = EAT_REACHED;
	should_stop_value = philosophers->utils->should_stop;
	pthread_mutex_unlock(&philosophers->utils->stop_mutex);
	return (should_stop_value);
}

void	print_status(t_philo *philosophers, char *status)
{
	int	should_stop_value;

	should_stop(philosophers);
	pthread_mutex_lock(&philosophers->utils->stop_mutex);
	should_stop_value = philosophers->utils->should_stop;
	pthread_mutex_unlock(&philosophers->utils->stop_mutex);
	if (should_stop_value != CONTINUE
		&& ft_strcmp(status, RED""_DEAD""RESET))
		return ;
	else if (!ft_strcmp(status, RED""_DEAD""RESET))
		should_stop_value = DEAD;
	printf(status, get_rel_time(philosophers->utils->start_time),
		philosophers->id);
	pthread_mutex_lock(&philosophers->utils->stop_mutex);
	should_stop_value = philosophers->utils->should_stop;
	pthread_mutex_unlock(&philosophers->utils->stop_mutex);
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
	int			should_stop_value;

	start_time = get_time();
	timer_ms = get_time_diff(start_time, get_time());
	pthread_mutex_lock(&philosophers->utils->stop_mutex);
	should_stop_value = philosophers->utils->should_stop;
	pthread_mutex_unlock(&philosophers->utils->stop_mutex);
	while (timer_ms < ms)
	{
		if (should_stop_value)
			return ;
		timer_ms = get_time_diff(start_time, get_time());
		usleep(ms / 10);
	}
}
