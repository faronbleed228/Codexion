
NAME		= codexion

SRCS		=	main.c \
				parsing.c \
				free_sim.c \
				helper.c \
				lst_helper.c \
				sim_helper.c \
				sim_logic.c \
				sim_start.c \
				simulation_init.c \
				queue_logic.c \
				dongle_logic.c

HEADERS_DIR	= .
HEADERS		=	$(HEADERS_DIR)/codexion.h

CC			= cc
CFLAGS		=  -Wall -Wextra -Werror -pthread
RM			= rm -f

OBJS_DIR	= objs
OBJS		= $(SRCS:%.c=$(OBJS_DIR)/%.o)

INCLUDES	= -I$(HEADERS_DIR)


all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "[$(NAME)] compiled successfully."

$(OBJS_DIR)/%.o: %.c $(HEADERS)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@$(RM) -r $(OBJS_DIR)
	@echo "objects removed."

fclean: clean
	@$(RM) $(NAME)
	@echo "$(NAME) removed."

re: fclean all

.PHONY: all clean fclean re

test: all
	@mkdir -p .tests
	$(CC) $(CFLAGS) $(INCLUDES) tests/queue.c $(filter-out main.c,$(SRCS)) -o .tests/queue
	./.tests/queue
	python3 tests/check.py

.PHONY: test
