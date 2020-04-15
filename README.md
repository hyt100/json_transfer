# json_transfer
json_transfer是一个JSON解析器，适合已知json格式时快速解析，采用数据结构的形式定义json格式，简洁不易出错：
- C语言实现，共4个文件，方便移植
- 依赖于[cJSON](https://github.com/DaveGamble/cJSON)库，需要链接math库(-lm)

使用示例：


1.数据转换为json
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
```

2.json转换为数据
``` c
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
```
