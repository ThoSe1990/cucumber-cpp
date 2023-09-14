#ifndef cwt_cucumber_value_h
#define cwt_cucumber_value_h



#include "common.h"

typedef struct obj obj;
typedef struct obj_string obj_string;


typedef enum {
  VAL_BOOL,
  VAL_NIL,
  VAL_NUMBER,
  VAL_OBJ 
} value_type;

typedef struct {
  value_type type;
  union {
    bool boolean;
    double number;
    obj* object; 
  } as;
} value;

#define IS_BOOL(v)    ((v).type == VAL_BOOL)
#define IS_NIL(v)     ((v).type == VAL_NIL)
#define IS_NUMBER(v)  ((v).type == VAL_NUMBER)
#define IS_OBJ(v)     ((v).type == VAL_OBJ)

#define AS_BOOL(v)    ((v).as.boolean)
#define AS_NUMBER(v)  ((v).as.number)
#define AS_OBJ(v)     ((v).as.object)

#define BOOL_VAL(v)   ((value){VAL_BOOL, {.boolean = v}})
#define NIL_VAL       ((value){VAL_NIL, {.number = 0}})
#define NUMBER_VAL(v) ((value){VAL_NUMBER, {.number = v}})
#define OBJ_VAL(v)    ((value){VAL_OBJ, {.object = (obj*)v}})



typedef struct {
  int capacity;
  int count;
  value* values;
} value_array;

bool values_equal(value a, value b);
void init_value_array(value_array* arr);
void write_value_array(value_array* arr, value v);
void free_value_array(value_array* arr);
void print_value(value v);
#endif 