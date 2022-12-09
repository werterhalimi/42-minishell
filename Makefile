# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/12 11:39:15 by ncotte            #+#    #+#              #
#    Updated: 2022/12/09 22:15:52 by shalimi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS_FILES		:=	builtins/env.c		builtins/export.c	builtins/pwd.c \
					builtins/unset.c	utils/error.c		utils/free.c \
					utils/init.c		main.c				parse/parse.c \
					utils/exec.c

SRCS_DIR		:= ./srcs/

SRCS			:= $(addprefix $(SRCS_DIR),$(SRCS_FILES))

OBJS_FILES		:= $(SRCS_FILES:.c=.o)

OBJS_DIR		:= ./bin/

OBJS_SUB_DIR	:=	$(OBJS_DIR)builtins	$(OBJS_DIR)utils $(OBJS_DIR)parse

OBJS			:= $(addprefix $(OBJS_DIR),$(OBJS_FILES))

LIBFT_DIR		:= ./libft

INC_DIR			:= ./inc/
INC				:= minishell.h
HEADERS 		:= $(addprefix $(INC_DIR),$(INC))

CC				:= @gcc
CFLAGS			:= -g -Wall -Wextra -Werror -I $(INC_DIR)
RM				:= @rm -f
LIB				:= -L $(LIBFT_DIR) -lft -lreadline
DEBUG			:= -g3 -fsanitize=address

NAME			:= minishell

all:			$(NAME)

${OBJS_DIR}%.o:	${SRCS_DIR}%.c $(HEADERS)
				@mkdir -p $(OBJS_DIR) $(OBJS_SUB_DIR)
				@echo "\033[1;32mCompiling $<\033[0m"  #Ajout de la ligne pour afficher le pourcentage de compilation en couleur.
				$(CC) $(CFLAGS) -c $< -o $@

$(NAME):		$(OBJS)
				@make -sC $(LIBFT_DIR) all
				$(CC) $(CFLAGS) $(LIB) -o $(NAME) $(OBJS)

clean:
				$(RM) $(OBJS)
				@rmdir $(OBJS_SUB_DIR) $(OBJS_DIR)
				$(RM) $(LIBFT_DIR)/*.o

fclean:			clean
				$(RM) $(NAME)
				$(RM) $(LIBFT_DIR)/libft.a

re:				fclean $(NAME)

run:			$(NAME)
				./$(NAME)

leaks:			$(NAME)
				leaks -atExit -- ./$(NAME)

debug:			CFLAGS += $(DEBUG)
debug:			$(OBJS)
				@make -C $(LIBFT_DIR) debug
				$(CC) $(CFLAGS) $(LIB) -o $(NAME) $(OBJS)

.PHONY:			all clean fclean re leaks run debugg
