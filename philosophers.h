/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 15:33:06 by rrouille          #+#    #+#             */
/*   Updated: 2023/06/08 15:33:05 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdbool.h>
# include <stdlib.h>
# include <pthread.h>

typedef enum e_status
{
	IS_SLEEPING,
	IS_THINKING,
	IS_EATING
}	t_status;

typedef struct s_philosopher
{
	int						curr_philo;
	int						max_eat;
	bool					death;
	bool					max_eat_reached;
	pthread_mutex_t			*left_fork;
	pthread_mutex_t			*right_fork;
	t_status				status;
	struct s_philosopher	*next;
}	t_philosophers;

typedef struct s_state
{
	int						nbr_of_philo;
	int						nbr_of_fork;
	int						time_to_eat;
	int						time_to_sleep;
	int						time_to_die;
	pthread_mutex_t			*forks;
}	t_state;

#endif