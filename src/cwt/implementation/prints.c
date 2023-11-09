#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "common.h"
#include "prints.h"

void start_black()
{
  printf("\x1b[30m");
}
void end_black()
{
  printf("\x1b[0m");
}
void start_red_on_stderr()
{
  fprintf(stderr, "\x1b[31m");
}
void end_red_on_stderr()
{
  fprintf(stderr, "\x1b[0m");
}
void print_red(const char* format, ...)
{
  va_list args;
  va_start(args, format);
  printf("\x1b[31m");
  vprintf(format, args);
  printf("\x1b[0m");
  va_end(args);
}
void print_yellow(const char* format, ...)
{
  va_list args;
  va_start(args, format);
  printf("\x1b[33m");
  vprintf(format, args);
  printf("\x1b[0m");
  va_end(args);
}
void print_blue(const char* format, ...)
{
  va_list args;
  va_start(args, format);
  printf("\x1b[34m");
  vprintf(format, args);
  printf("\x1b[0m");
  va_end(args);
}
void print_green(const char* format, ...)
{
  va_list args;
  va_start(args, format);
  printf("\x1b[32m");
  vprintf(format, args);
  printf("\x1b[0m");
  va_end(args);
}
void print_black(const char* format, ...)
{
  va_list args;
  va_start(args, format);
  printf("\x1b[30m");
  vprintf(format, args);
  printf("\x1b[0m");
  va_end(args);
}

void print_white(const char* format, ...)
{
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
}
