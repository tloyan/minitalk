# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thloyan <thloyan@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/27 14:19:35 by thloyan           #+#    #+#              #
#    Updated: 2023/01/27 15:56:12 by thloyan          ###   ########.fr        #
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
CFLAGS = -g -Wall -Wextra -Werror
OBJDIR = objs

OBJDIR_CLIENT = $(OBJDIR)/client
PATH_CLIENT = srcs/client
SRCS_CLIENT = main.c
OBJS_CLIENT = $(addprefix $(OBJDIR_CLIENT)/, $(SRCS_CLIENT:.c=.o))

OBJDIR_SERVER = $(OBJDIR)/server
PATH_SERVER = srcs/server
SRCS_SERVER = main.c
OBJS_SERVER = $(addprefix $(OBJDIR_SERVER)/, $(SRCS_SERVER:.c=.o))

OBJDIR_UTILS = $(OBJDIR)/utils
PATH_UTILS = srcs/utils
SRCS_UTILS = test.c
OBJS_UTILS = $(addprefix $(OBJDIR_UTILS)/, $(SRCS_UTILS:.c=.o))

################################################################################
#                                                                              #
#                                   RULES                                      #
#                                                                              #
################################################################################

all: $(NAME)

$(NAME): $(UTILS) $(CLIENT) $(SERVER)

$(OBJDIR):
	mkdir $(OBJDIR)

clean: clean_$(UTILS) clean_$(CLIENT) clean_$(SERVER)
	rm -rf $(OBJDIR)

fclean: clean fclean_$(CLIENT) fclean_$(SERVER)

re: fclean all

.PHONY: all clean fclean re

################################################################################
#                                                                              #
#                                   CLIENT                                     #
#                                                                              #
################################################################################

$(CLIENT): $(OBJDIR) $(OBJDIR_CLIENT) $(OBJS_CLIENT)
	$(CC) $(CFLAGS) -o $(CLIENT) $(OBJS_CLIENT) $(OBJS_UTILS)

$(OBJDIR_CLIENT):
	mkdir $(OBJDIR_CLIENT)

$(OBJDIR_CLIENT)/%.o: $(PATH_CLIENT)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I${INCLUDES}

clean_$(CLIENT):
	rm -f $(OBJS_CLIENT)
	rm -rf $(OBJDIR_CLIENT)

fclean_$(CLIENT):
	rm -f $(CLIENT)

.PHONY: clean_$(CLIENT) fclean_$(CLIENT)

################################################################################
#                                                                              #
#                                   SERVER                                     #
#                                                                              #
################################################################################

$(SERVER): $(OBJDIR) $(OBJDIR_SERVER) $(OBJS_SERVER)
	$(CC) $(CFLAGS) -o $(SERVER) $(OBJS_SERVER) $(OBJS_UTILS)

$(OBJDIR_SERVER):
	mkdir $(OBJDIR_SERVER)

$(OBJDIR_SERVER)/%.o: $(PATH_SERVER)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I${INCLUDES}

clean_$(SERVER):
	rm -f $(OBJS_SERVER)
	rm -rf $(OBJDIR_SERVER)

fclean_$(SERVER):
	rm -f $(SERVER)

.PHONY: clean_$(SERVER) fclean_$(SERVER)

################################################################################
#                                                                              #
#                                    UTILS                                     #
#                                                                              #
################################################################################

$(UTILS): $(OBJDIR) $(OBJDIR_UTILS) $(OBJS_UTILS)

$(OBJDIR_UTILS):
	mkdir $(OBJDIR_UTILS)

$(OBJDIR_UTILS)/%.o: $(PATH_UTILS)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I${INCLUDES}

clean_$(UTILS):
	rm -f $(OBJS_UTILS)
	rm -rf $(OBJDIR_UTILS)

.PHONY: $(UTILS) clean_$(UTILS)
