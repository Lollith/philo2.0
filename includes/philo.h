/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agouet <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 14:00:36 by agouet            #+#    #+#             */
/*   Updated: 2022/04/21 15:29:43 by agouet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>//exit
# include <sys/time.h>

# define FAILURE 0
# define SUCCESS 1


typedef struct 		s_rules
{
	int				nb_t_must_eat;
	int				t_eat;
	int				t_sleep;
	int				t_die;
	int				nb_philo;
	long int		time_ini;
	pthread_mutex_t *m_fork;
	pthread_mutex_t m_display;// besoin de controler le display sinon melange de tt les messages
	pthread_t		*th_id; // tab pid th pour chaque philo
	int				one_die;
	pthread_mutex_t	m_one_die; // ne pas faire une mutex generale pour 2 var, car si loc ma mutex va bloquer a mes endroit de lock en meme tps => pas inetressant 
}					t_rules;

typedef struct		s_philo 
{
	int				num;
	int 			nb_meal;
	int 			*state;
	pthread_mutex_t	*m_state; // ne pas faire une mutex generale pour 2 var, car si loc ma mutex va bloquer a mes endroit de lock en meme tps => pas inetressant 
	t_rules			*rules;
	struct s_philo	* next;
}					t_philo;

/*----------------------------------------------------------------------------*/
/*									MAIN									  */
/*----------------------------------------------------------------------------*/
int			initiate(t_rules *rules);
void		finisher(t_philo *philo);
int			ft_check_digit( int ac, char **av);
void		ft_input(t_rules *rules, char **argv);

/*----------------------------------------------------------------------------*/
/*									UTILS									  */
/*----------------------------------------------------------------------------*/
int			ft_isdigit(int c);
size_t		ft_strlen(const char *s);
int			ft_atoi(const char *nptr);
void		*ft_memset(void *s, int c, size_t n);
void		*ft_calloc(size_t nmemb, size_t size);

/*----------------------------------------------------------------------------*/
/*									LIST_PHILO									  */
/*----------------------------------------------------------------------------*/
int			list_philo(t_rules *rules, t_philo **philo);
t_philo		*ft_lstnew(int content, t_rules *rules);
t_philo		*ft_lstlast(t_philo *lst);
void		ft_lstadd_back(t_philo **alst, t_philo *new);
void		ft_lstclear(t_philo **lst);
/*----------------------------------------------------------------------------*/
/*									THREAD									  */
/*----------------------------------------------------------------------------*/
void		*routine_philo(void *arg);
void		*reaper(void *arg);
int 		create_thread_p(t_rules *rules, t_philo *philo);

/*----------------------------------------------------------------------------*/
/*									STATES									  */
/*----------------------------------------------------------------------------*/
long int	get_time(void);
void 		eating( int *pt_num, t_rules *rules, t_philo *philo);
void 		sleeping(int *pt_num, t_rules *rules, t_philo *philo);
void		thinking(int *pt_num, t_rules *rules, t_philo *philo);
void		dying(int *pt_num, t_rules *rules, t_philo *philo);

/*----------------------------------------------------------------------------*/
/*									ERRORS									  */
/*----------------------------------------------------------------------------*/
int			msg_perror(char *origin);
int			msg_error(char *error);
#endif
