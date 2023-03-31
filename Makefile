# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thloyan <thloyan@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/27 14:19:35 by thloyan           #+#    #+#              #
#    Updated: 2023/03/31 11:27:50 by thloyan          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
#                                                                              #
#                                   VARIABLES                                  #
#                                                                              #
################################################################################

NAME = minitalk

CLIENT = client
SERVER = server
UTILS =	utils

INCLUDES = includes

CC = cc
CFLAGS = -g -Wall -Wextra -Werror -MMD -MP
OBJDIR = .build

SRCS_UTILS	=	utils/test.c
SRCS_CLIENT = 	$(SRCS_UTILS) client/main.c
SRCS_SERVER = 	$(SRCS_UTILS) server/main.c

OBJS_CLIENT = $(addprefix $(OBJDIR)/, $(SRCS_CLIENT:.c=.o))
OBJS_SERVER = $(addprefix $(OBJDIR)/, $(SRCS_SERVER:.c=.o))

OBJECTS = $(OBJS_CLIENT) $(OBJS_SERVER)
DEPS := $(OBJECTS:.o=.d)

SRCS = $(SRCS_CLIENT) $(SRCS_SERVER)

################################################################################
#                                                                              #
#                                   RULES                                      #
#                                                                              #
################################################################################

all: $(NAME)

$(NAME): $(OBJDIR) $(CLIENT) $(SERVER)

$(CLIENT): $(OBJS_CLIENT)
	$(CC) $(CFLAGS) -o $(CLIENT) $(OBJS_CLIENT)

$(SERVER): $(OBJS_SERVER)
	$(CC) $(CFLAGS) -o $(SERVER) $(OBJS_SERVER)

$(OBJDIR):
	mkdir -p $(OBJDIR)/client
	mkdir -p $(OBJDIR)/server
	mkdir -p $(OBJDIR)/utils

$(OBJDIR)/%.o: srcs/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I${INCLUDES}

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(CLIENT) $(SERVER)

re: fclean all

.PHONY: all clean fclean re