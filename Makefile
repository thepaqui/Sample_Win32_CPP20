# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/06 18:10:48 by thepaqui          #+#    #+#              #
#    Updated: 2023/10/06 18:10:48 by thepaqui         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= winGUI

CC= c++

CCFLAGS=	-std=c++20 -mwindows

SRCS=	main.cpp

OBJS= $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CCFLAGS) $(OBJS) -o $(NAME)

%.o : %.cpp
	$(CC) $(CCFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
