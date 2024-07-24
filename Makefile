# Nombre del ejecutable
TARGET = texor

# Compilador y flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic  # Eliminado -Werror

# Archivos fuente
SRCS = editor.c terminal.c syntax.c row.c fileio.c find.c main.c

# Archivos objeto
OBJS = $(SRCS:.c=.o)

# Regla principal
$(TARGET): $(OBJS)
  $(CC) $(CFLAGS) -o $@ $(OBJS)

# Regla para generar archivos objeto
%.o: %.c
  $(CC) $(CFLAGS) -c $< -o $@

# Limpieza de archivos generados
clean:
  rm -f $(TARGET) $(OBJS)

.PHONY: clean
