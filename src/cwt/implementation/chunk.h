#ifndef chunk_common_h
#define chunk_common_h

#include "common.h"
#include "value.h"

typedef enum {
  OP_CONSTANT,
  OP_TAG,
  OP_PRINT_LINE,
  OP_NIL,
  OP_POP,
  OP_GET_VARIABLE,
  OP_DEFINE_VARIABLE,
  OP_SET_VARIABLE,
  OP_PRINT_LINEBREAK,
  OP_STEP_RESULT,
  OP_SCENARIO_RESULT,
  OP_OVERALL_RESULTS,
  OP_JUMP_IF_FAILED,
  OP_LOOP,
  OP_CALL,
  OP_CALL_STEP,
  OP_RETURN
} op_code;

typedef struct
{
  int count;
  int capacity;
  uint8_t* code;
  int* lines;
  value_array constants;
} chunk;

void init_chunk(chunk* c);
void free_chunk(chunk* c);
void write_chunk(chunk* c, uint8_t byte, int line);
int add_constant(chunk* c, cuke_value value);

#endif