#ifndef EDITOR_H
#define EDITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#define HL_NORMAL 0
#define HL_COMMENT 1
#define HL_MLCOMMENT 2
#define HL_STRING 3
#define HL_NUMBER 4
#define HL_KEYWORD1 5

typedef struct erow {
  int index;
  int size;
  int rendered_size;
  int highlight_open_comment;
  char *chars;
  char *rendered_characters;
  unsigned char *highlight;
} erow;

typedef struct editorSyntax {
  char *filetype;
  char **keywords;
  char *singleline_comment_start;
  char *multiline_comment_start;
  char *multiline_comment_end;
  int flags;
} editorSyntax;

typedef struct editorConfig {
  int cx, cy;
  int rowoff, coloff;
  int screenrows;
  int screencols;
  int numrows;
  erow *row;
  int dirty;
  char *filename;
  editorSyntax *syntax;
  struct termios orig_termios;
} editorConfig;

extern editorConfig E;

void die(const char *s);
void enableRawMode();
void disableRawMode();
int editorReadKey();
int getCursorPosition(int *rows, int *cols);
int getWindowSize(int *rows, int *cols);
void editorUpdateSyntax(erow *row);
void editorUpdateRow(erow *row);
void editorAppendRow(char *s, size_t len);
void editorFreeRow(erow *row);
void editorRefreshScreen();
void editorOpen(char *filename);
void editorSave();
void editorFind();

#endif /* EDITOR_H */
