
#include "philo.h"

int	msg_error(char *error)
{
	write(1, error, ft_strlen (error));
	return(FAILURE); //0
}

int	msg_perror(char *origin)
{
	perror(origin);
	exit(EXIT_FAILURE);
}
