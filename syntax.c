#include "editor.h"

int is_separator(int c) {
  return isspace(c) || c == '\0' || strchr(",.()+-/*=~%<>[];", c) != NULL;
}

void editorUpdateSyntax(erow *row) {
  row->highlight = realloc(row->highlight, row->rendered_size);
  memset(row->highlight, HL_NORMAL, row->rendered_size);
  if (E.syntax == NULL) return;
  char **keywords = E.syntax->keywords;
  char *singleline_comment_start = E.syntax->singleline_comment_start;
  char *multiline_comment_start = E.syntax->multiline_comment_start;
  char *multiline_comment_end = E.syntax->multiline_comment_end;
  int singleline_comment_start_length = singleline_comment_start ? strlen(singleline_comment_start) : 0;
  int multiline_comment_start_length = multiline_comment_start ? strlen(multiline_comment_start) : 0;
  int multiline_comment_end_length = multiline_comment_end ? strlen(multiline_comment_end) : 0;
  int previous_separator = 1;
  int in_string = 0;
  int in_comment = (row->index > 0 && E.row[row->index - 1].highlight_open_comment);
  int i = 0;
  while (i < row->rendered_size) {
    char c = row->rendered_characters[i];
    unsigned char prev_highlight = (i > 0) ? row->highlight[i - 1] : HL_NORMAL;
    if (singleline_comment_start_length && !in_string && !in_comment) {
      if (!strncmp(&row->rendered_characters[i], singleline_comment_start, singleline_comment_start_length)) {
        memset(&row->highlight[i], HL_COMMENT, row->rendered_size - i);
        break;
      }
    }
    if (multiline_comment_start_length && multiline_comment_end_length && !in_string) {
      if (in_comment) {
        row->highlight[i] = HL_MLCOMMENT;
        if (!strncmp(&row->rendered_characters[i], multiline_comment_end, multiline_comment_end_length)) {
          memset(&row->highlight[i], HL_MLCOMMENT, multiline_comment_end_length);
          i += multiline_comment_end_length;
          in_comment = 0;
          previous_separator = 1;
          continue;
        } else {
          i++;
          continue;
        }
      } else if (!strncmp(&row->rendered_characters[i], multiline_comment_start, multiline_comment_start_length)) {
        memset(&row->highlight[i], HL_MLCOMMENT, multiline_comment_start_length);
        i += multiline_comment_start_length;
        in_comment = 1;
        continue;
      }
    }
    if (E.syntax->flags & HL_HIGHLIGHT_STRINGS) {
      if (in_string) {
        row->highlight[i] = HL_STRING;
        if (c == '\\' && i + 1 < row->rendered_size) {
          row->highlight[i + 1] = HL_STRING;
          i += 2;
          continue;
        }
        if (c == in_string) in_string = 0;
        i++;
        previous_separator = 1;
        continue;
      } else {
        if (c == '"' || c == '\'') {
          in_string = c;
          row->highlight[i] = HL_STRING;
          i++;
          continue;
        }
      }
    }
    if (E.syntax->flags & HL_HIGHLIGHT_NUMBERS) {
      if (isdigit(c) && (previous_separator || prev_highlight == HL_NUMBER)) {
        row->highlight[i] = HL_NUMBER;
        i++;
        previous_separator = 0;
        continue;
      }
    }
    if (E.syntax->flags & HL_HIGHLIGHT_KEYWORDS) {
      if (is_separator(c)) {
        previous_separator = 1;
      } else {
        previous_separator = 0;
      }
      if (is_separator(row->rendered_characters[i - 1])) {
        char *p = row->rendered_characters;
        while (!is_separator(*p)) p++;
        int keyword_length = p - &row->rendered_characters[i];
        if (keyword_length) {
          for (int j = 0; keywords[j]; j++) {
            if (keyword_length == strlen(keywords[j]) &&
                !strncmp(p - keyword_length, keywords[j], keyword_length)) {
              memset(&row->highlight[i - keyword_length], HL_KEYWORD1, keyword_length);
              break;
            }
          }
        }
      }
    }
    i++;
  }
  if (in_comment) {
    row->highlight[row->rendered_size - 1] = HL_MLCOMMENT;
  }
  row->highlight_open_comment = in_comment;
}
