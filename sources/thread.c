/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agouet <agouet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 16:31:24 by agouet            #+#    #+#             */
/*   Updated: 2022/05/17 18:28:04 by agouet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_fork_eat(int *pt_left, int *pt_right, int *pt_num, t_philo *philo)
{
	int	left;
	int	right;
	int	num;

	left = *pt_left;
	right = *pt_right;
	num = *pt_num;
	if (philo->num % 2 == 0)
		usleep(philo->rules->t_eat * 1000);
	if (left < right)
	{
		pthread_mutex_lock(&philo->rules->m_fork[left]);
		pthread_mutex_lock(&philo->rules->m_fork[right]);
	}
	else
	{
		pthread_mutex_lock(&philo->rules->m_fork[right]);
		pthread_mutex_lock(&philo->rules->m_fork[left]);
	}
	taking_fork(&num, philo->rules, philo);
	eating(&num, philo->rules, philo);
	pthread_mutex_unlock(&philo->rules->m_fork[left]);
	pthread_mutex_unlock(&philo->rules->m_fork[right]);
}

void	*routine_philo(void *arg)
{
	t_philo	*philo;
	int		one_die;
	int		num;
	int		left;
	int		right;

	philo = (t_philo *)arg;
	if (philo->rules->nb_philo == 1)
	{
		one_philo(&philo->rules->nb_philo, philo);
		return (NULL);
	}
	num = philo->num + 1;
	left = num;
	right = (left + 1) % philo->rules->nb_philo;
	one_die = lck_ulck(philo->rules->m_one_die, philo->rules->one_die);
	while (!one_die && philo->rules->all_eat != 0)
	{
		check_fork_eat(&left, &right, &num, philo);
		sleeping(&num, philo->rules, philo);
		thinking(&num, philo->rules, philo);
		one_die = lck_ulck(philo->rules->m_one_die, philo->rules->one_die);
	}
	return (NULL);
}

void	*reaper(void *arg)
{
	t_philo	*temp;
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		temp = philo;
		if (!check_die_meal(philo))
			return (NULL);
		if(!check_t_die(temp, philo))
			return (NULL);
	}
}

int	check_t_die(t_philo *temp, t_philo *philo)
{
	int	i;
	int	die;
	int	state[4];
	int	state_diff;

	while (temp && temp->num < temp->rules->nb_philo)
	{
		i = 1;
		while (i <= 3)
		{
			state_diff = state[i] - state[0];
			state[i] = lck_ulck(temp->m_state[i], temp->state[i]);
			state[0] = lck_ulck(temp->m_state[0], temp->state[0]);
			if (state[i] >= state[0] && state_diff > temp->rules->t_die)
			{
				die = 1;
				philo->rules->one_die = lck_ulck(philo->rules->m_one_die, die);
				dying(&temp->num, temp->rules, temp);
				return (FAILURE);
			}
			i++;
		}
		temp = temp->next;
	}
	return (SUCCESS);
}

int	check_die_meal(t_philo *philo)
{
	int	one_die;
	int	all_eat;

	one_die = lck_ulck(philo->rules->m_one_die, philo->rules->one_die);
	all_eat = lck_ulck(philo->rules->m_one_die, philo->rules->all_eat);
	=>printf("all meat%d\n", all_eat); // PB ICI
	if (one_die || all_eat == 0)
		return (FAILURE);
	return (SUCCESS);
}
