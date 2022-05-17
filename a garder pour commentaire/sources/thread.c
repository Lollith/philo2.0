/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agouet <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 16:31:24 by agouet            #+#    #+#             */
/*   Updated: 2022/04/20 16:38:52 by agouet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *routine_philo(void *arg) // recup i, comme numero de philo
{
	int num = 0;
	t_philo *philo;
	philo = (t_philo *)arg;
	int one_die;	
	num = philo->num;
	//printf("philo %d cree\n ", num);
	int left = num;//creer des variables propre a chaque thread
	int right = (left + 1) % philo->rules->nb_philo;
	if (left == right)
		{
			printf("0 ");
			printf("%d", num);
			printf(" has taken a fork\n");
			pthread_mutex_lock(&philo->rules->m_one_die);// bloqueer one die car ds ma routine, ondie en lecture0
			philo->rules->one_die = 1;
			pthread_mutex_unlock(&philo->rules->m_one_die);// bloqueer one die car ds ma routine, ondie en lecture0
			usleep(philo->rules->t_eat * 1000);
			dying(&num, philo->rules, philo);
			return NULL;
		}
	if (philo->num % 2 == 0) // si pair , vont attendre , permet d imposer un rythme a mes thread plutot que aleatire 
		usleep(philo->rules->t_eat * 1000);
	pthread_mutex_lock(&philo->rules->m_one_die);// bloqueer one die car ds ma routine, ondie en lecture0
	one_die = philo->rules->one_die;
	pthread_mutex_unlock(&philo->rules->m_one_die);// bloqueer one die car ds ma routine, ondie en lecture0
	while (!one_die && philo->rules->all_eat != 0)
	{
		if (left < right )				 // eviter les deadlock,  lorsauun thread doit reserver +ieurs mutex en mem tps, importence de respecter un ordre stricte
		{
			pthread_mutex_lock(&philo->rules->m_fork[left]);
			//printf("philo %d prend guche  : %d\n", num, left);
			pthread_mutex_lock(&philo->rules->m_fork[right]);
			//printf("philo %d prend droite : %d\n", num, right);
		}
		else
		{
			pthread_mutex_lock(&philo->rules->m_fork[right]);
			//printf("philo %d prend droite : %d\n", num, right);
			pthread_mutex_lock(&philo->rules->m_fork[left]);
			//printf("philo %d prend guche : %d\n", num, left);
		}
		eating(&num, philo->rules, philo);
		pthread_mutex_unlock(&philo->rules->m_fork[left]); // if err => erreur
		//printf("philo %d pose guche  : %d\n", num, left);
		pthread_mutex_unlock(&philo->rules->m_fork[right]);
		//printf("philo %d pose droite  : %d\n", num, right);
		
			sleeping(&num, philo->rules, philo);
			thinking(&num, philo->rules, philo);
	pthread_mutex_lock(&philo->rules->m_one_die);// bloqueer one die car ds ma routine, ondie en lecture0
	one_die = philo->rules->one_die;
	pthread_mutex_unlock(&philo->rules->m_one_die);// bloqueer one die car ds ma routine, ondie en lecture0
	}
	return (NULL);// permet de finir le thread
}

int create_thread_p(t_rules *rules, t_philo *philo)
{
	int i = 0;
	pthread_t id;
	t_philo *temp;

	temp = philo; // je copie mon philo pour grder mon begin sur philo, car temp va avancer pour creer mes thread
	while (i < rules->nb_philo && temp)
	{
		if (pthread_create(&rules->th_id[i], NULL, &routine_philo, temp) != 0 ) // success =0
			return (msg_error("error pthread_create\n")); // 0
		temp = temp->next;
		// soit faire un tab de philo avec i++ soit ici on avance dans notre structure

		i++;
	}
	if (pthread_create(&id, NULL, &reaper, philo) != 0) 
		return (msg_error("error pthread_create\n")); // 0
	
	i = 0;
	while (i < rules->nb_philo)  
	{
		pthread_join(rules->th_id[i], NULL);
		i++;
	}
	pthread_join(id, NULL);
	return 1;
}

void *reaper(void *arg)
{

	t_philo *philo;
	philo = (t_philo *)arg;
	t_philo *temp;
	int state[4];
	int state0;
	int one_die;
	int all_eat;
		int i;
// importance de mutex ma variable en ecriture ms aussi enlecture, car va check ma variable qui doit pas bouger a ce moment
	
	
	while(1)
	{
		temp = philo;
		pthread_mutex_lock(&philo->rules->m_one_die);// bloqueer one die car ds ma routine, ondie en lecture0
		one_die = philo->rules->one_die;
		all_eat = philo->rules->all_eat;
		pthread_mutex_unlock(&philo->rules->m_one_die);
		
		 //   ma mutex one die marche differement des fork, je agrde une variable
		if (one_die || all_eat == 0)
			return NULL;
		
		while (temp && temp->num < temp->rules->nb_philo) // tant que mon pointeur begin philo existe, sinon next++ et sort ;
		{
			i = 1;
			while(i <= 3) // 3 etats et pas de 0
			{

				pthread_mutex_lock(&temp->m_state[i]);// bloqueer one die car ds ma routine, ondie en lecture0
				state[i] = temp->state[i];
				//printf("[%d\n", state[i]);
				//usleep( 900); //attention prend du tps  ne pas mettre entre les mutex
				pthread_mutex_unlock(&temp->m_state[i]);// bloqueer one die car ds ma routine, ondie en lecture0
				pthread_mutex_lock(&temp->m_state[0]);// bloqueer one die car ds ma routine, ondie en lecture0
				state0 = temp->state[0];
				pthread_mutex_unlock(&temp->m_state[0]);// bloqueer one die car ds ma routine, ondie en lecture0
				
				//printf ("reaper check etats %d du num %d\n", i, temp->num  );
			//	printf("reaper check %d\n", temp->num);
				//printf ("state %d time %u\n",i, temp->state[i] -temp->state[1]); // si mon tps entre etat actuel et le debut du repas sest sup u tdie
				//while(state[i] < state[0]) // doit pas etre negatif, sinon compare le sleep du tour  davant
			//		i = (i + 1) % 2;
				//printf ("|state %d %u ms\n",i, temp->state[i]-temp->state[1]); // si mon tps entre etat actuel et le debut du repas sest sup u tdie
				//printf ("state 1 %u ms\n", temp->state[1]); // si mon tps entre etat actuel et le debut du repas sest sup u tdie
				//printf ("state i %d %u ms|\n",i, temp->state[i]); // si mon tps entre etat actuel et le debut du repas sest sup u tdie
					// si mon tps entre etat actuel et le debut du repas sest sup u tdie
				if (state[i] >= state0 && state[i] - state0 >  temp->rules->t_die ) // si mon tps entre etat actuel et le debut du repas sest sup u tdie
						{	
							one_die = 1;
							pthread_mutex_lock(&philo->rules->m_one_die);// bloqueer one die car ds ma routine, ondie en lecture0
							 philo->rules->one_die = one_die; // attention renvoyer ma variable ds ma structure generale
							pthread_mutex_unlock(&philo->rules->m_one_die);
							//printf("1 mort ici doit0000000000000000000000000 s afficher en moins de 10 ms\n");
							dying(&temp->num, temp->rules, temp);
							return NULL;
						}
				i++;
			}	
		
			temp = temp->next;
		}
	}
}