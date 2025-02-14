CC = gcc
CFLAGS = -g -Wall -Wextra -Werror 
LDFLAGS = 
# - предупреждения - ошибки

SRC_DIR = source

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

TARGET = a.exe

all: a.exe

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

$(OBJS): $(SRCS)
	$(CC) -c $(CFLAGS) $(SRCS)

clean:
	rm -rf $(TARGET)