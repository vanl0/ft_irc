NAME = ircserv
CXX = c++
CXXFLAGS = -std=c++98
INCLUDES = -I$(HEADERS_DIR)

HEADERS_DIR = ./headers/
HEADERS_LS = Server.hpp\
			 Client.hpp\
			 Channel.hpp\
			 utils.hpp
HEADERS = $(addprefix $(HEADERS_DIR), $(HEADERS_LS))

SRCS_DIR = srcs/
OBJS_DIR = objs/

SRCS_LS =	main.cpp\
			Client.cpp\
			Server.cpp\
			Channel.cpp\
			parsing.cpp\
			login.cpp\
			utils.cpp\
			join.cpp\
			privMsg.cpp\
			topic.cpp\
			kick.cpp\
			mode.cpp\
			invite.cpp

SRCS = $(addprefix $(SRCS_DIR), $(SRCS_LS))
OBJS = $(addprefix $(OBJS_DIR),$(notdir $(SRCS_LS:.cpp=.o)))

GRAY = \033[2;29m
GREEN = \033[0;32m
YELLOW = \033[0;33m
NC = \033[0m


all: $(NAME)

#MINISHELL
$(NAME): $(OBJS_DIR) $(OBJS) $(HEADERS) Makefile	
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIB_FLAGS) $(INCLUDES) -o $@ 

#OBJS
$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp $(HEADERS) Makefile
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) $<  -o $@ 

$(OBJS_DIR):
	@mkdir $@

#CLEAN
clean:
	@rm -rf $(OBJS_DIR)
	@echo "$(GREEN)Objects removed$(NC)"
fclean: clean
	@rm -f $(NAME)
	@echo "$(GREEN)Objects and executables removed$(NC)"

re: fclean all

.PHONY: all clean fclean re