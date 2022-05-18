/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agouet <agouet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 13:33:09 by agouet            #+#    #+#             */
/*   Updated: 2022/05/18 16:22:30 by agouet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	initiate(t_rules *rules)
{
	int	siz_m_fork;
	int	i;

	siz_m_fork = sizeof (pthread_mutex_t);
	rules->one_die = 0;
	rules->all_eat = rules->nb_philo;
	rules->th_id = (pthread_t *) ft_calloc(rules->nb_philo, sizeof (pthread_t));
	rules->m_fork = (pthread_mutex_t *) ft_calloc(rules->nb_philo, siz_m_fork);
	if (!rules->m_fork || !rules->th_id)
		return (msg_error("malloc"));
	i = 0;
	while (i < rules->nb_philo)
	{
		if (pthread_mutex_init(&rules->m_fork[i], NULL))
			return (msg_error("mutex_init"));
		i++;
	}
	if (pthread_mutex_init(&rules->m_display, NULL))
		return (msg_error("mutex_init"));
	if (pthread_mutex_init(&rules->m_one_die, NULL))
		return (msg_error("mutex_init"));
	rules->time_ini = get_time();
	return (SUCCESS);
}

int	ft_check_digit( int ac, char **av)
{
	int	i;
	int	j;

	i = 1;
	while (i < ac)
	{
		j = 0;
		if (av[i][j] == '-')
			return (msg_error("Error: Positive number only\n"));
		if (av[i][j] == '+')
			j++;
		while (av[i][j])
		{
			if (!ft_isdigit(av[i][j]))
				return (msg_error("Error: Positive number only\n"));
			j++;
		}
		i++;
	}
	return (SUCCESS);
}

void	ft_input(t_rules *rules, char **argv)
{
	rules->nb_philo = ft_atoi(argv[1]);
	rules->t_eat = ft_atoi(argv[3]);
	rules->t_sleep = ft_atoi(argv[4]);
	rules->t_die = ft_atoi(argv[2]);
	if (argv[5])
		rules->nb_t_must_eat = ft_atoi(argv[5]);
	else
		rules->nb_t_must_eat = -1;
}

void	finisher(t_philo *philo)
{
	int		i;
	t_philo	*temp;

	temp = philo;
	i = 0;
	while (i < philo->rules->nb_philo)
	{
		pthread_mutex_destroy(&philo->rules->m_fork[i]);
		i++;
	}
	i = 0;
	while (i < 3)
	{
		pthread_mutex_destroy(&philo->m_state[i]);
		i++;
	}
	pthread_mutex_destroy(&philo->rules->m_display);
	pthread_mutex_destroy(&philo->rules->m_one_die);
	free(philo->rules->th_id);
	free(philo->rules->m_fork);
	ft_lstclear(&philo);
	printf("Fin!\n");
}

int	main(int argc, char **argv)
{
	t_rules	rules;
	t_philo	*philo;

	if (argc != 5 && argc != 6)
	{
		msg_error("Arguments:| N | t_die | t_eat | t_sleep | [num_t_eat]|\n");
		return (1);
	}
	ft_input(&rules, argv);
	if (!ft_check_digit(argc, argv))
		return (1);
	if (!initiate(&rules))
	{
		free(rules.th_id);
		free(rules.m_fork);
		return (1);
	}
	if (!list_philo(&rules, &philo))
	{
		free(rules.th_id);
		free(rules.m_fork);
		ft_lstclear(&philo);
		return (1);
	}
	if(!create_thread_p(&rules, philo))
	{
		finisher(philo);
		return(1);
	}
	finisher(philo);
	return (0);
}
