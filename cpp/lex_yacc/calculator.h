#pragma once

#define MAX_SYMBOL_NUM 20

struct Symbol {
  char *name;
  double value;
};
struct Symbol *symbol_look(const char*);
