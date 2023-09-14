
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"

#define TABLE_MAX_LOAD 0.75

void init_table(table* t)
{
  t->count = 0;
  t->capacity = 0;
  t->entries = NULL;
}

void free_table(table* t)
{
  FREE_ARRAY(entry, t->entries, t->capacity);
  init_table(t);
}

static entry* find_entry(entry* entries, int capacity, obj_string* key)
{
  uint32_t index = key->hash % capacity;
  entry* tombstone = NULL;
  for(;;)
  {
    entry* e = &entries[index];

    if (e->key == NULL) 
    {
      if (IS_NIL(e->value))
      {
        return tombstone != NULL ? tombstone : e;
      } 
      else 
      {
        if (tombstone == NULL) {tombstone = e; }
      }
    }
    else if (e->key == key)
    {
      return e;
    }

    index = (index+1) % capacity;
  }
}

static void adjust_capacity(table* t, int capacity)
{
  entry* entries = ALLOCATE(entry, capacity);
  for (int i = 0 ; i < capacity ; i++)
  {
    entries[i].key = NULL;
    entries[i].value = NIL_VAL;
  }
  t->count = 0;
  for (int i = 0; i < t->capacity; i++)
  {
    entry* e = &t->entries[i];
    if (e->key == NULL) { continue; }
    entry* dst = find_entry(entries, capacity, e->key);
    dst->key = e->key;
    dst->value = e->value;
    t->count++;
  }
  FREE_ARRAY(entry, t->entries, t->capacity);
  t->entries = entries;
  t->capacity = capacity;
}

void table_add_all(table* from, table* to)
{
  for (int i = 0; i < from->capacity ; i++)
  {
    entry* e = &from->entries[i];
    if (e->key != NULL)
    {
      table_set(to, e->key, e->value);
    }
  }
}

obj_string* table_find_string(table* t, const char* chars, int length, uint32_t hash)
{
  if (t->count == 0) { return NULL; }

  uint32_t index = hash % t->capacity;
  for(;;)
  {
    entry* e = &t->entries[index];
    if (e->key == NULL) 
    {
      if (IS_NIL(e->value)) { return NULL; }
    }
    else if (e->key->length == length 
      && e->key->hash == hash 
      && memcmp(e->key->chars, chars, length) == 0)
    {
      return e->key;
    }

    index = (index+1) % t->capacity;
  }
}

bool table_get(table* t, obj_string* key, value* v)
{
  if (t->count == 0) { return false; }
  
  entry* e = find_entry(t->entries, t->capacity, key);
  if (e->key == NULL) { return false; }
  
  *v = e->value; 
  return v;
}

bool table_delete(table* t, obj_string* key)
{
  if (t->count == 0) { return false; }

  entry* e = find_entry(t->entries, t->capacity, key);
  if (e->key == NULL) { return false; }

  e->key = NULL;
  e->value = BOOL_VAL(true);
  return true; 
}

bool table_set(table* t, obj_string* key, value v)
{
  if (t->count+1 > t->capacity * TABLE_MAX_LOAD)
  {
    int capacity = GROW_CAPACITY(t->capacity);
    adjust_capacity(t, capacity);
  }
  entry* e = find_entry(t->entries, t->capacity, key);
  bool is_new = e->key == NULL;
  if (is_new && IS_NIL(e->value)) 
  {
    t->count++;
  }
  e->key = key;
  e->value = v;
  return is_new;
}