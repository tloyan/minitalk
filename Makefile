# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thloyan <thloyan@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/27 14:19:35 by thloyan           #+#    #+#              #
#    Updated: 2023/05/25 16:47:06 by thloyan          ###   ########.fr        #
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

SRCS_CLIENT = client/main.c client/utils.c
SRCS_SERVER = server/main.c server/utils.c

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
	$(CC) $(CFLAGS) -o $(CLIENT) $(OBJS_CLIENT) -I${INCLUDES}/client

$(SERVER): $(OBJS_SERVER)
	$(CC) $(CFLAGS) -o $(SERVER) $(OBJS_SERVER) -I${INCLUDES}/server

$(OBJDIR):
	mkdir -p $(OBJDIR)/client
	mkdir -p $(OBJDIR)/server

$(OBJDIR)/$(CLIENT)/%.o: srcs/$(CLIENT)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I${INCLUDES}/client

$(OBJDIR)/$(SERVER)/%.o: srcs/$(SERVER)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I${INCLUDES}/server

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(CLIENT) $(SERVER)

re: fclean all

.PHONY: all clean fclean re