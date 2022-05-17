/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agouet <agouet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 13:59:20 by agouet            #+#    #+#             */
/*   Updated: 2022/05/17 14:00:00 by agouet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	msg_error(char *error)
{
	write(1, error, ft_strlen (error));
	return (FAILURE);
}

int	msg_perror(char *origin)
{
	perror(origin);
	exit (EXIT_FAILURE);
}
