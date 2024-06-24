# **************************************************************************** #  
#                                                                              #  
#                                                         :::      ::::::::    #  
#    Makefile                                           :+:      :+:    :+:    #  
#                                                     +:+ +:+         +:+      #  
#    By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+         #  
#                                                 +#+#+#+#+#+   +#+            #  
#    Created: 2024/01/22 16:28:38 by ggalon            #+#    #+#              #  
#    Updated: 2024/03/30 09:24:16 by ggalon           ###   ########.fr        #  
#                                                                              #  
# **************************************************************************** #  


# DIRECTORIES ==================================================================  

SRCS_DIR	=	src/

OBJS_DIR	=	.obj/

INCL_DIR	=	inc/

# FILES ========================================================================  

NAME	=	webserv

FILE_C	=	webserv config utils ConfigFile Server Location Socket Request Printer

FILE_H	=	webserv ConfigFile Server Location Socket Request Printer

SRCS	=	$(addsuffix .cpp, $(addprefix $(SRCS_DIR), $(FILE_C)))

OBJS	=	$(addsuffix .o, $(addprefix $(OBJS_DIR), $(FILE_C)))

INCL	=	$(addsuffix .hpp, $(addprefix $(INCL_DIR), $(FILE_H)))

# COMMANDS =====================================================================  

CC			=	c++

CC_FLAGS	=	-Wall -Wextra -Werror -g3 -std=c++98 -I ./inc

# RULES ========================================================================  

all:
		@ mkdir -p $(OBJS_DIR)
		@ echo "\n${BBlue}Compilation of exercice source files...${NC}"
		@ $(MAKE) --no-print-directory $(NAME)
		@ echo "\n${BGreen}Exercice Ready !${NC}\n"

$(NAME): $(OBJS)
		@ echo "\n${BCyan}Creating the executable...${NC}"
		$(CC) $(CC_FLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp $(INCL) Makefile
		$(CC) $(CC_FLAGS) -c $< -o $@

clean:
		@ echo "\n${BRed}Exercice binary deletion...${NC}"
		rm -rf $(OBJS_DIR)
		@ echo

fclean:
		@ echo "\n${BRed}Exercice deletion...${NC}"
		rm -rf $(OBJS_DIR)
		rm -f $(NAME)
		@ echo

re: fclean all

.PHONY: all clean fclean re

# COLORS =======================================================================  

# Reset
NC=\033[0m

# Regular
Black=\033[0;90m
Red=\033[0;91m
Green=\033[0;92m
Yellow=\033[0;93m
Blue=\033[0;94m
Purple=\033[0;95m
Cyan=\033[0;96m
White=\033[0;97m

# Bold
BBlack=\033[1;90m
BRed=\033[1;91m
BGreen=\033[1;92m
BYellow=\033[1;93m
BBlue=\033[1;94m
BPurple=\033[1;95m
BCyan=\033[1;96m
BWhite=\033[1;97m

# Background
BGBlack=\033[0;100m
BGRed=\033[0;101m
BGGreen=\033[0;102m
BGYellow=\033[0;103m
BGBlue=\033[0;104m
BGPurple=\033[0;105m
BGCyan=\033[0;106m
BGWhite=\033[0;107m