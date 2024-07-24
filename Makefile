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
<TAB>$(CC) $(CFLAGS) -o $@ $(OBJS)

# Regla para generar archivos objeto
%.o: %.c
<TAB>$(CC) $(CFLAGS) -c $< -o $@

# Limpieza de archivos generados
clean:
<TAB>rm -f $(TARGET) $(OBJS)

.PHONY: clean
