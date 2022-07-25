/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launcher.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nedebies <nedebies@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 17:41:15 by nedebies          #+#    #+#             */
/*   Updated: 2022/07/25 12:13:53 by nedebies         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_eats(t_philosopher *philo)
{
	t_rules	*rules;

	rules = philo->rules;
	pthread_mutex_lock(&(rules->forks[philo->left_fork_id]));
	action_print(rules, philo->id, "has taken a fork");
	pthread_mutex_lock(&(rules->forks[philo->right_fork_id]));
	action_print(rules, philo->id, "has taken a fork");
	pthread_mutex_lock(&(rules->meal_check));
	action_print(rules, philo->id, "is eating");
	philo->t_last_meal = timestamp();
	pthread_mutex_unlock(&(rules->meal_check));
	smart_sleep(rules->time_to_eat, rules);
	(philo->x_ate)++;
	pthread_mutex_unlock(&(rules->forks[philo->left_fork_id]));
	pthread_mutex_unlock(&(rules->forks[philo->right_fork_id]));
}

static void	*p_thread(void *void_philosopher)
{
	int				i;
	t_philosopher	*philo;
	t_rules			*rules;

	i = 0;
	philo = (t_philosopher *)void_philosopher;
	rules = philo->rules;
	if (philo->id % 2)
		usleep(15000);
	while (!(rules->dead))
	{
		philo_eats(philo);
		if (rules->all_ate)
			break ;
		action_print(rules, philo->id, "is sleeping");
		smart_sleep(rules->time_to_sleep, rules);
		action_print(rules, philo->id, "is thinking");
		i++;
	}
	return (NULL);
}

static void	exit_launcher(t_rules *rules, t_philosopher *philo)
{
	int	i;

	i = -1;
	while (++i < rules->number_of_philosophers)
		pthread_join(philo[i].thread_id, NULL);
	i = -1;
	while (++i < rules->number_of_philosophers)
		pthread_mutex_destroy(&(rules->forks[i]));
	pthread_mutex_destroy(&(rules->writing));
}

static void	death_checker(t_rules *r, t_philosopher *p)
{
	int	i;

	while (!(r->all_ate))
	{
		i = -1;
		while (++i < r->number_of_philosophers && !(r->dead))
		{
			pthread_mutex_lock(&(r->meal_check));
			if (timestamp() - p[i].t_last_meal > r->time_to_die)
			{
				action_print(r, i, "died");
				r->dead = 1;
			}
			pthread_mutex_unlock(&(r->meal_check));
			usleep(100);
		}
		if (r->dead)
			break ;
		i = 0;
		while (r->number_of_philo_meals != -1 && i < r->number_of_philosophers
			&& p[i].x_ate >= r->number_of_philo_meals)
			i++;
		if (i == r->number_of_philosophers)
			r->all_ate = 1;
	}
}

int	launcher(t_rules *rules)
{
	int				i;
	t_philosopher	*philo;

	i = 0;
	philo = rules->phi;
	rules->first_timestamp = timestamp();
	while (i < rules->number_of_philosophers)
	{
		if (pthread_create(&(philo[i].thread_id), NULL, p_thread, &(philo[i])))
			return (0);
		philo[i].t_last_meal = timestamp();
		i++;
	}
	death_checker(rules, rules->phi);
	exit_launcher(rules, philo);
	return (1);
}
