/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 15:33:06 by rrouille          #+#    #+#             */
/*   Updated: 2023/06/05 15:33:08 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef enum e_status
{
	IS_SLEEPING,
	IS_THINKING,
	IS_EATING
}	t_status;

typedef struct s_philos
{
	int				curr_philo;
	bool			right_fork_free;
	bool			left_fork_free;
	t_status		status;
	struct s_philos	*prev;
	struct s_philos	*next;
}	t_philos;

typedef struct s_state
{
	int			nbr_of_philo;
	int			nbr_of_fork;
	int			max_eat;
	int			time_to_eat;
	int			time_to_sleep;
	int			time_to_die;
}	t_state;