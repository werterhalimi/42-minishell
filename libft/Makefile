# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/12 11:39:15 by ncotte            #+#    #+#              #
#    Updated: 2022/10/28 16:26:38 by ncotte           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS			=	ft_atoi.c		ft_bzero.c		ft_calloc.c \
					ft_isalnum.c	ft_isalpha.c	ft_isascii.c \
					ft_isdigit.c	ft_isprint.c	ft_itoa.c \
					ft_memchr.c		ft_memcmp.c		ft_memcpy.c \
					ft_memmove.c	ft_memset.c		ft_putchar_fd.c \
					ft_putendl_fd.c	ft_putnbr_fd.c	ft_putstr_fd.c \
					ft_split.c		ft_strchr.c		ft_strdup.c \
					ft_striteri.c	ft_strjoin.c	ft_strlcat.c \
					ft_strlcpy.c	ft_strlen.c		ft_strmapi.c \
					ft_strncmp.c	ft_strnstr.c	ft_strrchr.c \
					ft_strtrim.c	ft_substr.c		ft_tolower.c \
					ft_toupper.c \
					ft_lstadd_back.c	ft_lstadd_front.c	ft_lstclear.c \
					ft_lstdelone.c		ft_lstiter.c		ft_lstlast.c \
					ft_lstmap.c			ft_lstnew.c			ft_lstsize.c \
					 \
					ft_printf.c		malloc_nbr.c		malloc_char.c \
					t_str.c			ft_uitoa.c			ft_hextoa.c \
					 \
					get_next_line_bonus.c	get_next_line_utils_bonus.c

OBJS			= $(SRCS:.c=.o)

CC				= gcc
CFLAGS			= -Wall -Wextra -Werror -I.
RM				= rm -f
LIB				= ar rcs
DEBUG			= -g3 -fsanitize=address

NAME			= libft.a

all:			$(NAME)

.c.o:			
				$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

$(NAME):		$(OBJS)
				$(LIB) $(NAME) $(OBJS)

clean:
				$(RM) $(OBJS)

fclean:			clean
				$(RM) $(NAME)

re:				fclean $(NAME)

test:			all
				$(CC) $(CFLAGS) -L. -lft -o test main.c $(DEBUG)

debug:			CFLAGS += $(DEBUG)
debug:			$(NAME)

.PHONY:			all clean fclean re test debug
