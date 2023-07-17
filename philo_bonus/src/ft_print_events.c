/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_events.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nesdebie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 14:15:42 by nesdebie          #+#    #+#             */
/*   Updated: 2023/07/17 13:25:16 by nesdebie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	print_action(t_philo *philo, int end_flag, int status)
{
	sem_wait(philo->sem_write);
	if (status == DIED)
		printf("%ld %d %s\n", get_time_in_ms() - philo->rules->start_time,
			philo->id + 1, "died");
	else if (status == EATING)
		printf("%ld %d %s\n", get_time_in_ms() - philo->rules->start_time,
			philo->id + 1, "is eating");
	else if (status == SLEEPING)
		printf("%ld %d %s\n", get_time_in_ms() - philo->rules->start_time,
			philo->id + 1, "is sleeping");
	else if (status == THINKING)
		printf("%ld %d %s\n", get_time_in_ms() - philo->rules->start_time,
			philo->id + 1, "is thinking");
	else if (status == GOT_FORK_1 || status == GOT_FORK_2)
		printf("%ld %d %s\n", get_time_in_ms() - philo->rules->start_time,
			philo->id + 1, "has taken a fork");
	if (!end_flag)
		sem_post(philo->sem_write);
}
