NAME = codexion

CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread

INCLUDES = -I includes

SRCS = src/coder_routine.c \
src/monitor.c \
src/main.c \
src/coders.c \
src/dongle.c \
src/heap.c \
src/parse.c \
src/simulate.c \
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
