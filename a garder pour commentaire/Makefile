# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agouet <agouet@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/20 16:34:58 by agouet            #+#    #+#              #
#    Updated: 2022/05/17 14:30:23 by agouet           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #



NAME 		= philo

SRCS_PATH	= ./sources/

SRCS		= $(addprefix $(SRCS_PATH), main.c errors.c utils.c \
			  	list_philo.c thread.c states.c time.c)

OBJS		= $(SRCS:.c=.o)

DEPS		= $(SRCS:.c=.d)

CC 			= cc

CFLAGS		= -MMD -Wall -Wextra -Werror -g  

THREADF		= -lpthread -D_REENTRANT

INC			= -I ./includes

all:		$(NAME)

$(NAME):	$(OBJS)
		$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(THREADF)

%.o:		%.c
		$(CC) $(CFLAGS) -c $< -o $@ $(INC)

clean:		
		rm -f $(OBJS) $(DEPS)

fclean:		clean
		rm -f $(NAME)

re:			fclean all

.PHONY:		all re clean fclean

-include $(DEPS)
