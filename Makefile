# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ncotte <marvin@42lausanne.ch>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/12 11:39:15 by ncotte            #+#    #+#              #
#    Updated: 2023/01/04 21:41:52 by shalimi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS_FILES		:=	utils/free.c		utils/sort.c		utils/get_commands.c \
					utils/ft_trim.c		utils/error.c \
					env/var_value.c		env/init.c			env/valid_var_name.c\
					builtins/cd.c		builtins/echo.c		builtins/export_one_var.c \
					builtins/env.c		builtins/exit.c		builtins/export.c \
					builtins/pwd.c		builtins/unset.c \
					parse/set_fd.c		parse/get_string.c	parse/handle_output.c \
					parse/handle_var.c	parse/handle_line.c	parse/handle_input.c \
					parse/parse.c		parse/quote_utils.c parse/handle_tilde.c \
					parse/handle_quote.c \
					exec/path.c			exec/signals.c		exec/exec_builtin.c \
					exec/close_wait.c	exec/main_exec.c \
					main.c

SRCS_DIR		:= ./srcs/

SRCS			:= $(addprefix $(SRCS_DIR),$(SRCS_FILES))

OBJS_FILES		:= $(SRCS_FILES:.c=.o)

OBJS_DIR		:= ./bin/

OBJS_SUB_DIR	:= $(OBJS_DIR)builtins $(OBJS_DIR)utils $(OBJS_DIR)parse $(OBJS_DIR)exec $(OBJS_DIR)env

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
