/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 13:20:35 by rrouille          #+#    #+#             */
/*   Updated: 2023/06/28 12:41:02 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
