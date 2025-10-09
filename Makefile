# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cafabre <camille.fabre003@gmail.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/09 01:04:32 by cafabre           #+#    #+#              #
#    Updated: 2025/10/10 01:35:15 by cafabre          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

INCLUDE = -Iincludes

FILES =	main.c\
		utils.c

CC = cc

CFLAGS = -Wall -Wextra -Werror -MMD -g3 -pthread \
			-I includes

FILE = $(FILES)

OBJ_DIR = .obj/

OBJ = $(addprefix $(OBJ_DIR), $(FILE:=.c=.o))
DEPD = $(addprefix $(OBJ_DIR), $(FILE:.c=.d))

all: $(NAME)

$(NAME): $(OBJ)
		$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
		
$(OBJ_DIR)%.o: %.c Makefile
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
	
-include $(DEPD)

.PHONY: FORCE
FORCE:

clean:
	$(MAKE) -C clean
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C fclean
	rm -rf $(NAME)
	
re: fclean all

.PHONY: all clean fclean re