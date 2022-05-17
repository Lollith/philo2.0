
#include "philo.h"


t_philo	*ft_lstnew(int content, t_rules *rules)
{
	t_philo	*new_elem;

	new_elem = (t_philo *) malloc (sizeof(t_philo) * 1);
	if (!new_elem)
		return (NULL);
	new_elem->num = (int) content;
	new_elem->state = (int*) ft_calloc(4, sizeof (int) ); // initialisatioon lors de la creation des maillons, + simples que initialisation apres
	new_elem->nb_meal = rules->nb_t_must_eat; // penser a initialiser mes donne apres la creation de la sturcture en .h de m liste.:wq
	new_elem->m_state = (pthread_mutex_t*) ft_calloc(4, sizeof(pthread_mutex_t));
	if (!new_elem->m_state)
		return (NULL); //0
	int i = 0;
	while (i < 4) // attention initialisation dune variable et non dun tabeleau entier
	{
		if(pthread_mutex_init(&new_elem->m_state[i], NULL))// tab dc pas de &
			return (NULL);
		i++;
	}	
	new_elem->next = NULL;
	return (new_elem);
}


t_philo	*ft_lstlast(t_philo *lst)//renvoi le dernier eleemt cree de la list
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back(t_philo **alst, t_philo *new)// ajoute new a la fin
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

void ft_lstclear(t_philo **lst) // free mes malloc ds ma liste chainee
{
	t_philo *tmp;

	tmp = NULL;
	if (lst)
	{
		while(*lst)
		{
			tmp = (*lst)->next;
			free((*lst)->state);
			free((*lst)->m_state);
			free(*lst);
	
			*lst = tmp;
		}
	}

}

int list_philo(t_rules *rules, t_philo **philo) // a free

{
	int i;
	t_philo *l_new_p; // creation dun nouveau maillon

	*philo = NULL;
	i = 0;

	while (i < rules->nb_philo) // creer des maillon et les coller
	{
		l_new_p = ft_lstnew(i, rules); // ajoute mon contenu ici ma structure philo // initialise a la creation
		if (!l_new_p)
		{
			printf("error l_new"); // ft_lstclear
			return (1);
		}
		l_new_p->rules = rules;
		ft_lstadd_back(philo, l_new_p); // colle new derriere mon dernier element ici le1

		i++; // ici je met mea structure rules dans ma liste chainnee dc je peut incrementer ma liste chainee ,  facilement tout em envoyant mes rules avec ma liste chainee ds la fct de crea des thread
	}
	return 0;
}