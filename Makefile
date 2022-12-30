# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/12 11:39:15 by ncotte            #+#    #+#              #
#    Updated: 2022/12/30 19:16:11 by shalimi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS_FILES		:=	builtins/env.c		parse/parse.c		builtins/pwd.c \
					builtins/unset.c	utils/error.c		utils/free.c \
					env/init.c			main.c				exec/exec_builtin.c \
					exec/path.c			builtins/export.c 	exec/signals.c \
					exec/main_exec.c	builtins/echo.c		exec/close_wait.c \
					env/var_value.c		builtins/exit.c		utils/array_copy.c \
					utils/sort.c		builtins/cd.c		utils/remove_char.c \
					utils/ft_trim.c

SRCS_DIR		:= ./srcs/

SRCS			:= $(addprefix $(SRCS_DIR),$(SRCS_FILES))

OBJS_FILES		:= $(SRCS_FILES:.c=.o)

OBJS_DIR		:= ./bin/

OBJS_SUB_DIR	:=	$(OBJS_DIR)builtins	$(OBJS_DIR)utils $(OBJS_DIR)parse $(OBJS_DIR)exec $(OBJS_DIR)env

OBJS			:= $(addprefix $(OBJS_DIR),$(OBJS_FILES))

LIBFT_DIR		:= ./libft

INC_DIR			:= ./inc/
INC				:= minishell.h
HEADERS 		:= $(addprefix $(INC_DIR),$(INC)) $(HOME)/.brew/opt/readline/include

CC				:= @gcc
CFLAGS			:= -g -Wall -Wextra -Werror -I $(INC_DIR) -I $(HOME)/.brew/opt/readline/include
RM				:= @rm -f
LIB				:= -L $(LIBFT_DIR) -L $(HOME)/.brew/opt/readline/lib -lft -lreadline
DEBUG			:= -g3 -fsanitize=address
UNIT			:= ""
NAME			:= minishell

all:			$(NAME)

${OBJS_DIR}%.o:	${SRCS_DIR}%.c $(HEADERS)
				@mkdir -p $(OBJS_DIR) $(OBJS_SUB_DIR)
				@echo "\033[1;32mCompiling $<\033[0m"  #Ajout de la ligne pour afficher le pourcentage de compilation en couleur.
				$(CC) $(UNIT) $(CFLAGS) -c $< -o $@

$(NAME):		$(OBJS)
				@make -sC $(LIBFT_DIR) all
				$(CC) $(CFLAGS) $(LIB) -o $(NAME) $(OBJS)

unit:			clean		
				echo salut
				make UNIT="-D UNIT"

clean:
				$(RM) $(OBJS)
				$(RM) -r $(OBJS_SUB_DIR) $(OBJS_DIR)
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

.PHONY:			all clean fclean re leaks run debug test unit
