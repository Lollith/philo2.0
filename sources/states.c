/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   states.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agouet <agouet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 16:47:34 by agouet            #+#    #+#             */
/*   Updated: 2022/05/17 18:13:56 by agouet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	taking_fork(int *pt_num, t_rules *rules, t_philo *philo)
{
	int	one_die;
	int	state0;

	pthread_mutex_lock(&philo->rules->m_one_die);
	one_die = philo->rules->one_die;
	pthread_mutex_unlock(&philo->rules->m_one_die);
	if (!one_die)
	{
		pthread_mutex_lock(&philo->m_state[0]);
		philo->state[0] = get_time() - philo->rules->time_ini;
		state0 = philo->state[0];
		pthread_mutex_unlock(&philo->m_state[0]);
		pthread_mutex_lock(&rules->m_display);
		printf("%u %d has taken a fork\n", state0, *pt_num);
		printf("%u %d has taken a fork\n", state0, *pt_num);
		pthread_mutex_unlock(&rules->m_display);
	}
}

void	eating(int *pt_num, t_rules *rules, t_philo *philo)
{
	int	state0;
	int	one_die;

	pthread_mutex_lock(&philo->rules->m_one_die);
	one_die = philo->rules->one_die;
	pthread_mutex_unlock(&philo->rules->m_one_die);
	pthread_mutex_lock(&philo->m_state[0]);
	state0 = philo->state[0];
	pthread_mutex_unlock(&philo->m_state[0]);
	if (!one_die)
	{
		pthread_mutex_lock(&rules->m_display);
		printf("%u %d is eating\n", state0, *pt_num);
		pthread_mutex_unlock(&rules->m_display);
	}
	pthread_mutex_lock(&philo->rules->m_one_die);
	philo->nb_meal --;
	if (philo->nb_meal == 0)
		philo->rules->all_eat --;
	pthread_mutex_unlock(&philo->rules->m_one_die);
	usleep(rules->t_eat * 1000);
}

void	sleeping(int *pt_num, t_rules *rules, t_philo *philo)
{
	int	state1;
	int	one_die;

	pthread_mutex_lock(&philo->rules->m_one_die);
	one_die = philo->rules->one_die;
	pthread_mutex_unlock(&philo->rules->m_one_die);
	if (!one_die)
	{
		pthread_mutex_lock(&philo->m_state[1]);
		philo->state[1] = get_time() - philo->rules->time_ini;
		state1 = philo->state[1];
		pthread_mutex_unlock(&philo->m_state[1]);
	}	
	if (!one_die)
	{
		pthread_mutex_lock(&rules->m_display);
		printf("%u %d is sleeping\n", state1, *pt_num);
		pthread_mutex_unlock(&rules->m_display);
	}
	usleep(rules->t_sleep * 1000);
}

void	thinking(int *pt_num, t_rules *rules, t_philo *philo)
{
	int	state2;
	int	one_die;

	pthread_mutex_lock(&philo->rules->m_one_die);
	one_die = philo->rules->one_die;
	pthread_mutex_unlock(&philo->rules->m_one_die);
	if (!one_die)
	{
		pthread_mutex_lock(&philo->m_state[2]);
		philo->state[2] = get_time() - philo->rules->time_ini;
		state2 = philo->state[2];
		pthread_mutex_unlock(&philo->m_state[2]);
	}	
	if (!one_die)
	{
		pthread_mutex_lock(&rules->m_display);
		printf("%u %d is thinking\n", state2, *pt_num);
		pthread_mutex_unlock(&rules->m_display);
	}
	pthread_mutex_lock(&philo->m_state[3]);
	philo->state[3] = get_time() - philo->rules->time_ini;
	pthread_mutex_unlock(&philo->m_state[3]);
}

void	dying(int *pt_num, t_rules *rules, t_philo *philo)
{
	int	one_die;

	pthread_mutex_lock(&philo->rules->m_one_die);
	one_die = philo->rules->one_die;
	pthread_mutex_unlock(&philo->rules->m_one_die);
	if (one_die == 1)
	{
		pthread_mutex_lock(&rules->m_display);
		printf("%ld ", get_time() - philo->rules->time_ini);
		printf("%d", *pt_num);
		printf(" died\n");
		pthread_mutex_unlock(&rules->m_display);
	}
}
