# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cafabre <cafabre@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/09 01:04:32 by cafabre           #+#    #+#              #
#    Updated: 2025/10/17 08:57:20 by cafabre          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

INCLUDE = -I includes

FILES =	main.c\
		routine/eating.c\
		routine/philo.c\
		src/args.c\
		src/init_philos.c\
		src/init_prog.c\
		src/cleanup.c\
		src/threads.c\
		src/monitor.c\
		src/utils.c\
		utils/print_utils.c

CC = cc

CFLAGS = -Wall -Wextra -Werror -MMD -g3 -pthread $(INCLUDE)

OBJ_DIR = .obj/

OBJ = $(addprefix $(OBJ_DIR), $(FILES:.c=.o))
DEPD = $(addprefix $(OBJ_DIR), $(FILES:.c=.d))

all: $(NAME)

$(NAME): $(OBJ)
		$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
		
$(OBJ_DIR)%.o: %.c Makefile
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
	
-include $(DEPD)

.PHONY: all clean fclean re

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)
	
re: fclean all