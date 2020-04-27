# json_transfer
`json_transfer`是一个JSON序列化/反序列化器，适合已知json格式时快速转换，采用数据结构映射json格式，简洁不易出错：
- C语言实现，共4个文件，方便移植
- 集成了[cJSON](https://github.com/DaveGamble/cJSON)库，使用时需要链接math库(-lm)

使用示例：`{"method":"Set","params":{"mode":"single"},"id":1}`


1.数据结构反序列化为json文本
``` c
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
  free(json); //It's the user's responsibility to free this memory
```

2.json文本序列化为数据结构
``` c
  char mode2[JSON_STRING_SIZE_MAX];
  json_elem_t params2[] = {
    {"mode",   JSON_TYPE_STRING, 0, 0, mode2},
    {NULL, 0, 0, 0, NULL}
  };

  char method2[JSON_STRING_SIZE_MAX];
  int id2 = 0;
  json_elem_t root2[] = {
    {"method", JSON_TYPE_STRING, 0, 0, method2},
    {"params", JSON_TYPE_OBJ, 0, 0, params2},
    {"id",     JSON_TYPE_INT, 0, 0, &id2},
    {NULL, 0, 0, 0, NULL}
  };
  transfer_json_to_data(json, (json_elem_t *)root2);
```
