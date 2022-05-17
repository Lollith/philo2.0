/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   states.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agouet <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 16:47:34 by agouet            #+#    #+#             */
/*   Updated: 2022/04/20 16:48:16 by agouet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long int get_time(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000 + (tv.tv_usec / 1000)));
}

void eating(int *pt_num, t_rules *rules, t_philo *philo)
{
	int state0;
	int one_die;


	pthread_mutex_lock(&philo->rules->m_one_die);// bloqueer one die car ds ma routine, ondie en lecture0
	one_die= philo->rules->one_die; // attention renvoyer ma variable ds ma structure generale
	pthread_mutex_unlock(&philo->rules->m_one_die);

	if (!one_die)
	{
		pthread_mutex_lock(&philo->m_state[0]); // protege state et nnb meal aussi
		philo->state[0] = get_time() - philo->rules->time_ini;
		state0 =  philo->state[0]; // pour reduire la tille du mustex state
		
		pthread_mutex_unlock(&philo->m_state[0]); // protege state et nnb meal aussi

		
	}		
	//pthread_mutex_lock(&philo->rules->m_one_die);// bloqueer one die car ds ma routine, ondie en lecture0
	//one_die= philo->rules->one_die; // attention renvoyer ma variable ds ma structure generale
	//pthread_mutex_unlock(&philo->rules->m_one_die);

	if (!one_die)
	{
		pthread_mutex_lock(&rules->m_display); // mettre le moins de mutex ossible a linterieur
		printf("%u ", state0);
		printf("%d", *pt_num);
		printf(" has taken a fork\n");
		printf("%u ", state0);
		printf("%d", *pt_num);
		printf(" has taken a fork\n");
		pthread_mutex_unlock(&rules->m_display);
	}
	
//	pthread_mutex_lock(&philo->rules->m_one_die);// bloqueer one die car ds ma routine, ondie en lecture0
	//one_die= philo->rules->one_die; // attention renvoyer ma variable ds ma structure generale
	//pthread_mutex_unlock(&philo->rules->m_one_die);
	if (!one_die)
	{
		pthread_mutex_lock(&rules->m_display);
		printf("%u ", state0);
		printf("%d", *pt_num);
		printf(" is eating\n");
		//printf("philo %d a mange %d \n",*pt_num, philo->nb_meal);
		pthread_mutex_unlock(&rules->m_display);
	}
		usleep(rules->t_eat * 1000); //attention prend du tps  ne pas mettre entre les mutex
		pthread_mutex_lock(&philo->rules->m_one_die);// bloqueer one die car ds ma routine, ondie en lecture0
		philo->nb_meal --;
		if (philo->nb_meal == 0)
			philo->rules->all_eat --;
		pthread_mutex_unlock(&philo->rules->m_one_die);// bloqueer one die car ds ma routine, ondie en lecture0
}

void sleeping(int *pt_num, t_rules *rules, t_philo *philo)
{
	int state1;
	int one_die;

	pthread_mutex_lock(&philo->rules->m_one_die);// bloqueer one die car ds ma routine, ondie en lecture0
	one_die= philo->rules->one_die; // attention renvoyer ma variable ds ma structure generale
	pthread_mutex_unlock(&philo->rules->m_one_die);
	if (!one_die)
	{
		pthread_mutex_lock(&philo->m_state[1]); // protege state et nnb meal aussi
		philo->state[1] = get_time() - philo->rules->time_ini;
		state1 =  philo->state[1];
		pthread_mutex_unlock(&philo->m_state[1]); // protege state et nnb meal aussi
		
		
	}	
	//pthread_mutex_lock(&philo->rules->m_one_die);// bloqueer one die car ds ma routine, ondie en lecture0
	//one_die= philo->rules->one_die; // attention renvoyer ma variable ds ma structure generale
	//pthread_mutex_unlock(&philo->rules->m_one_die);
	if (!one_die)
	{
	pthread_mutex_lock(&rules->m_display);
		printf("%u ", state1);
		printf("%d", *pt_num);
		printf(" is sleeping\n");
		pthread_mutex_unlock(&rules->m_display);
	}
		usleep(rules->t_sleep * 1000);
}

void thinking(int *pt_num, t_rules *rules, t_philo *philo)
{
	int state2;
	int one_die;
	pthread_mutex_lock(&philo->rules->m_one_die);// bloqueer one die car ds ma routine, ondie en lecture0
	one_die= philo->rules->one_die; // attention renvoyer ma variable ds ma structure generale
	pthread_mutex_unlock(&philo->rules->m_one_die);
	if (!one_die)
	{
		pthread_mutex_lock(&philo->m_state[2]); // protege state et nnb meal aussi
		philo->state[2] = get_time() - philo->rules->time_ini;
		state2 =  philo->state[2];
		pthread_mutex_unlock(&philo->m_state[2]); // protege state et nnb meal aussi
	}	
//	pthread_mutex_lock(&philo->rules->m_one_die);// bloqueer one die car ds ma routine, ondie en lecture0
	//one_die= philo->rules->one_die; // attention renvoyer ma variable ds ma structure generale
//	pthread_mutex_unlock(&philo->rules->m_one_die);
	if (!one_die)
	{
		
	pthread_mutex_lock(&rules->m_display);
		printf("%u ", state2);
		printf("%d", *pt_num);
		printf(" is thinking\n");
		pthread_mutex_unlock(&rules->m_display);
	}
		pthread_mutex_lock(&philo->m_state[3]); //check dernier etat car si 4 3900 200 100 => doivent mourir cf visualiser
		philo->state[3] = get_time() - philo->rules->time_ini;
		pthread_mutex_unlock(&philo->m_state[3]); 
}

void dying(int *pt_num, t_rules *rules, t_philo *philo)
{
	int one_die;
	pthread_mutex_lock(&philo->rules->m_one_die);// bloqueer one die car ds ma routine, ondie en lecture0
	one_die= philo->rules->one_die; // attention renvoyer ma variable ds ma structure generale
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