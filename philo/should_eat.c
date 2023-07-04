/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   should_eat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 18:31:57 by rrouille          #+#    #+#             */
/*   Updated: 2023/07/03 15:44:07 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	should_eat(t_philo *philosophers)
{
	philosophers->state = EAT_;
	print_status(philosophers, YELLOW""EATING""RESET);
	pthread_mutex_lock(&philosophers->time_mtx);
	philosophers->last_meal_time = get_time();
	pthread_mutex_unlock(&philosophers->time_mtx);
	if (philosophers->running.eating_count > 0)
	{
		pthread_mutex_lock(&philosophers->n_eat_mutex);
		philosophers->n_eat += 1;
		pthread_mutex_unlock(&philosophers->n_eat_mutex);
	}
	sleep_ms(philosophers->running.time_to_eat, philosophers);
}
