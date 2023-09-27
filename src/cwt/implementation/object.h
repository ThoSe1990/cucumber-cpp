#ifndef cwt_cucumber_object_h
#define cwt_cucumber_object_h

#include "common.h"
#include "value.h"
#include "chunk.h"


#define OBJ_TYPE(value)     (AS_OBJ(value)->type)

#define IS_STRING(value)    is_obj_type(value, OBJ_STRING)
#define IS_FUNCTION(value)  is_obj_type(value, OBJ_FUNCTION)
#define IS_NATIVE(value)    is_obj_type(value, OBJ_NATIVE)


#define AS_STRING(value)    ((obj_string*)AS_OBJ(value))
#define AS_CSTRING(value)   (((obj_string*)AS_OBJ(value))->chars)
#define AS_FUNCTION(value)  ((obj_function*)AS_OBJ(value))
#define AS_NATIVE(value)    (((obj_native*)AS_OBJ(value))->function)

typedef enum {
  OBJ_FUNCTION,
  OBJ_NATIVE,
  OBJ_STRING
} obj_type;

struct obj {
  obj_type type;
  struct obj* next;
};

typedef struct {
  obj object;
  int arity;
  chunk chunk;
  obj_string* name;
} obj_function;

typedef void (*cwtc_step_t)(int arg_count, cwtc_value* args);

typedef struct {
  obj object;
  cwtc_step_t function;
} obj_native;

struct obj_string {
  obj object;
  int length;
  char* chars;
  uint32_t hash;
};

obj_string* take_string(char* chars, int length);
obj_native* new_native(cwtc_step_t function);
obj_function* new_function();
obj_string* copy_string(const char* chars, int length);
void print_object(cwtc_value value);

static inline bool is_obj_type(cwtc_value value, obj_type type) 
{
  return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif 