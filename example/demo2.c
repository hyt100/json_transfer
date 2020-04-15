#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/json_transfer.h"

int unpack(char *text)
{
  char s1[JSON_STRING_SIZE_MAX];
  char s2[JSON_STRING_SIZE_MAX];
  char s3[JSON_STRING_SIZE_MAX];
  char s4[JSON_STRING_SIZE_MAX];
  char s5[JSON_STRING_SIZE_MAX];
  char s6[JSON_STRING_SIZE_MAX];
  
  json_elem_t u1[] = {
    {"name", JSON_TYPE_STRING, 0, 0, s1},
    {"addr", JSON_TYPE_STRING, 0, 0, s2},
    {NULL, 0, 0, 0, NULL}
  };
  json_elem_t u2[] = {
    {"name", JSON_TYPE_STRING, 0, 0, s3},
    {"addr", JSON_TYPE_STRING, 0, 0, s4},
    {NULL, 0, 0, 0, NULL}
  };

  int value[] = {0, 0};
  json_elem_t* arr_obj[] = {u1, u2};
  json_array_t arr_arr2[] = {
    {JSON_TYPE_INT, ARRAY_SIZE(value), value},
    {JSON_TYPE_INT, ARRAY_SIZE(value), value},
  };
  json_array_t arr_arr1[] = {
    {JSON_TYPE_ARRAY, ARRAY_SIZE(arr_arr2), arr_arr2},
    {JSON_TYPE_ARRAY, ARRAY_SIZE(arr_arr2), arr_arr2},
  };
  json_elem_t params[] = {
    {"mode",   JSON_TYPE_STRING, 0, 0, s5},
    {"value",  JSON_TYPE_ARRAY, JSON_TYPE_INT, ARRAY_SIZE(value), value},
    {"other",  JSON_TYPE_ARRAY, JSON_TYPE_OBJ, ARRAY_SIZE(arr_obj), arr_obj},
    {"other2", JSON_TYPE_ARRAY, JSON_TYPE_ARRAY, ARRAY_SIZE(arr_arr1), arr_arr1},
    {NULL, 0, 0, 0, NULL}
  };

  int id = 0;
  json_elem_t root[] = {
    {"method", JSON_TYPE_STRING, 0, 0, s6},
    {"params", JSON_TYPE_OBJ, 0, 0, params},
    {"id",     JSON_TYPE_INT, 0, 0, &id},
    {NULL, 0, 0, 0, NULL}
  };

  int ret = transfer_json_to_data(text, (json_elem_t *)root);
  if (ret != 0)
    return ret;

  //pack again for debug
  #if 1
  char *json = transfer_data_to_json((json_elem_t *)root);
  if (json) {
    printf("%s \n", json);
    free(json);
  }
  #endif

  return ret;
}


//  {"method":"Set","params":{"mode":"single","value":[18,19],"other":[{"name":"eric","addr":"xxx"},{"name":"jack","addr":"yyy"}],
//  "other2":[[[18,19],[18,19]],[[18,19],[18,19]]]},"id":1} 
char* pack(void)
{
  json_elem_t u1[] = {
    {"name", JSON_TYPE_STRING, 0, 0, "eric"},
    {"addr", JSON_TYPE_STRING, 0, 0, "xxx"},
    {NULL, 0, 0, 0, NULL}
  };
  json_elem_t u2[] = {
    {"name", JSON_TYPE_STRING, 0, 0, "jack"},
    {"addr", JSON_TYPE_STRING, 0, 0, "yyy"},
    {NULL, 0, 0, 0, NULL}
  };

  int value[] = {18, 19};
  json_elem_t* arr_obj[] = {u1, u2};
  json_array_t arr_arr2[] = {
    {JSON_TYPE_INT, ARRAY_SIZE(value), value},
    {JSON_TYPE_INT, ARRAY_SIZE(value), value},
  };
  json_array_t arr_arr1[] = {
    {JSON_TYPE_ARRAY, ARRAY_SIZE(arr_arr2), arr_arr2},
    {JSON_TYPE_ARRAY, ARRAY_SIZE(arr_arr2), arr_arr2},
  };
  json_elem_t params[] = {
    {"mode",   JSON_TYPE_STRING, 0, 0, "single"},
    {"value",  JSON_TYPE_ARRAY, JSON_TYPE_INT, ARRAY_SIZE(value), value},
    {"other",  JSON_TYPE_ARRAY, JSON_TYPE_OBJ, ARRAY_SIZE(arr_obj), arr_obj},
    {"other2", JSON_TYPE_ARRAY, JSON_TYPE_ARRAY, ARRAY_SIZE(arr_arr1), arr_arr1},
    {NULL, 0, 0, 0, NULL}
  };

  int id = 1;
  json_elem_t root[] = {
    {"method", JSON_TYPE_STRING, 0, 0, "Set"},
    {"params", JSON_TYPE_OBJ, 0, 0, params},
    {"id",     JSON_TYPE_INT, 0, 0, &id},
    {NULL, 0, 0, 0, NULL}
  };

  return transfer_data_to_json((json_elem_t *)root);
}

int main(int argc, char *argv[])
{
  char *json = pack();
  if (!json) {
    printf("pack failed. \n");
    return -1;
  }
  printf("%s \n", json);
  //return 0;

  int ret = unpack(json);
  if (ret != 0) {
    printf("unpack failed. \n");
    return -1;
  }
  
  free(json);
  return 0;
}
