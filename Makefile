NAME = codexion

CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread

INCLUDES = -I includes

SRCS = src/main.c \
src/parse.c \
src/init.c \
src/init_coders.c \
src/clean.c \
src/simulate.c \
src/coder_routine.c \
src/monitor.c \
src/actions.c \
src/dongle.c \
src/scheduler.c \
src/heap.c \
src/heap_utils.c \
src/stop.c \
src/print.c \
src/time.c


OBJS = $(SRCS:.c=.o)
HEADERS = includes/codexion.h

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)
fclean: clean
	rm -f $(NAME)

re: fclean all

debug: CFLAGS += -g -O0 -fsanitize=address
debug: all

.PHONY: all clean fclean re debug
