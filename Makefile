# ============================================================================
# CODEXION — Makefile
#
# ЗАПОЛНИ ЭТИ ДВА СПИСКА СВОИМИ ФАЙЛАМИ. Больше ничего трогать не нужно.
# ============================================================================

NAME		= codexion

# --- Твои .c файлы (без расширения не надо, полный путь от корня) ----------
SRCS		=	main.c \
				parsing.c \
				free_sim.c \
				helper.c \
				lst_helper.c \
				sim_helper.c \
				sim_logic.c \
				sim_start.c \
				simulation_init.c \
				queue_logic.c

# --- Твои .h файлы (нужны как зависимость, чтобы re-link был не лишним) ----
HEADERS_DIR	= .
HEADERS		=	$(HEADERS_DIR)/codexion.h

# ============================================================================
# Ниже — механика, обычно не требует правок
# ============================================================================

CC			= cc
CFLAGS		=  -pthread
RM			= rm -f

OBJS_DIR	= objs
OBJS		= $(SRCS:%.c=$(OBJS_DIR)/%.o)

INCLUDES	= -I$(HEADERS_DIR)


all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)[$(NAME)] compiled successfully.$(RESET)"

# Правило сборки объектников: каждый .o зависит от своего .c И от заголовков.
# Это гарантирует, что при изменении .h всё, что его инклюдит, пересоберётся,
# но НЕ будет пересобираться то, что не менялось -> нет лишнего релинка.
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
