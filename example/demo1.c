#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/json_transfer.h"

int main(int argc, char *argv[])
{
  //pack: {"method":"Set","params":{"mode":"single"},"id":1} 
  json_elem_t params[] = {
    {"mode",   JSON_TYPE_STRING, 0, 0, "single"},
    {NULL, 0, 0, 0, NULL}
  };

  int id = 1;
  json_elem_t root[] = {
    {"method", JSON_TYPE_STRING, 0, 0, "Set"},
    {"params", JSON_TYPE_OBJ, 0, 0, params},
    {"id",     JSON_TYPE_INT, 0, 0, &id},
    {NULL, 0, 0, 0, NULL}
  };

  char *json = transfer_data_to_json((json_elem_t *)root);
  printf("%s \n", json);

  //unpack
  char mode2[JSON_STRING_SIZE_MAX];
  json_elem_t params2[] = {
    {"mode",   JSON_TYPE_STRING, 0, 0, mode2},
    {NULL, 0, 0, 0, NULL}
  };

  char method2[JSON_STRING_SIZE_MAX];
  int id2 = 1;
  json_elem_t root2[] = {
    {"method", JSON_TYPE_STRING, 0, 0, method2},
    {"params", JSON_TYPE_OBJ, 0, 0, params2},
    {"id",     JSON_TYPE_INT, 0, 0, &id2},
    {NULL, 0, 0, 0, NULL}
  };
  transfer_json_to_data(json, (json_elem_t *)root2);
}

