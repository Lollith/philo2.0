/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agouet <agouet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 11:43:41 by agouet            #+#    #+#             */
/*   Updated: 2022/05/17 11:58:29 by agouet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*ft_lstnew(int content, t_rules *rules)
{
	t_philo	*new_elem;
	int		i;
	int		size_mutex;

	size_mutex = sizeof (pthread_mutex_t);
	new_elem = (t_philo *) malloc (sizeof(t_philo) * 1);
	if (!new_elem)
		return (NULL);
	new_elem->num = (int) content;
	new_elem->nb_meal = rules->nb_t_must_eat;
	new_elem->state = (int *) ft_calloc (4, sizeof (int));
	new_elem->m_state = (pthread_mutex_t *) ft_calloc(4, size_mutex);
	if (!new_elem->m_state || !new_elem->state)
		return (NULL);
	i = 0;
	while (i < 4)
	{
		if (pthread_mutex_init(&new_elem->m_state[i], NULL))
			return (NULL);
		i++;
	}	
	new_elem->next = NULL;
	return (new_elem);
}

t_philo	*ft_lstlast(t_philo *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back(t_philo **alst, t_philo *new)
{
	t_philo	*last;

	if (!(*alst))
		*alst = new;
	else
	{
		last = ft_lstlast(*alst);
		last->next = new;
	}
	new->next = NULL;
}

void	ft_lstclear(t_philo **lst)
{
	t_philo	*tmp;

	tmp = NULL;
	if (lst)
	{
		while (*lst)
		{
			tmp = (*lst)->next;
			free((*lst)->state);
			free((*lst)->m_state);
			free(*lst);
			*lst = tmp;
		}
	}
}

int	list_philo(t_rules *rules, t_philo **philo)
{
	int		i;
	t_philo	*l_new_p;

	*philo = NULL;
	i = 0;
	while (i < rules->nb_philo)
	{
		l_new_p = ft_lstnew(i, rules);
		if (!l_new_p)
			return (msg_error("Error: list"));
		l_new_p->rules = rules;
		ft_lstadd_back(philo, l_new_p);
		i++;
	}
	return (SUCCESS);
}
