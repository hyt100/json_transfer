/*
  Copyright (c) 2020 hyt100

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "json_transfer.h"

static int pack_obj(cJSON *node, json_elem_t *json_elem);
static int parse_obj(cJSON *node, json_elem_t *elem);

static int parse_array(cJSON *node, json_array_t *desp)
{
  int array_size = cJSON_GetArraySize(node);
  if (array_size < desp->arr_size) return -1;
  if (desp->arr_type >= JSON_TYPE_MAX) return -1;
  
  int i;
  for (i = 0; i < desp->arr_size; ++i) {
    cJSON *item = cJSON_GetArrayItem(node, i);
    if (!item) return -1;

    if (JSON_TYPE_INT == desp->arr_type) {
      if (cJSON_Number != item->type) return -1;
      ((int *)desp->val)[i] = item->valueint;
    }
    else if (JSON_TYPE_DOUBLE == desp->arr_type) {
      if (cJSON_Number != item->type) return -1;
      ((double *)desp->val)[i] = item->valuedouble;
    }
    else if (JSON_TYPE_STRING == desp->arr_type) {
      if (cJSON_String != item->type || strlen(item->valuestring) + 1 > JSON_STRING_SIZE_MAX) return -1;
      strcpy(((char **)desp->val)[i], item->valuestring);
    }
    else if (JSON_TYPE_OBJ == desp->arr_type) {
      if (cJSON_Object != item->type) return -1;
      if (parse_obj(item, ((json_elem_t **)desp->val)[i]) != 0) return -1;
    }
    else if (JSON_TYPE_ARRAY == desp->arr_type) {
      if (cJSON_Array != item->type) return -1;
      if (parse_array(item, ((json_array_t *)desp->val) + i ) != 0) return -1;
    }
    else {
      return -1;
    }
  }
  return 0;
}

static int parse_obj(cJSON *node, json_elem_t *elem)
{
  json_elem_t *it;
  for (it = elem; it->name != NULL; ++it) {
    if (it->type >= JSON_TYPE_MAX) return -1;
      
    cJSON *item = cJSON_GetObjectItem(node, it->name);
    if (NULL == item) return -1;
      
    if (JSON_TYPE_INT == it->type) {
      if (cJSON_Number != item->type) return -1;
      *((int *)it->val) = item->valueint;
    }
    else if (JSON_TYPE_DOUBLE == it->type) {
      if (cJSON_Number != item->type) return -1;
      *((double *)it->val) = item->valuedouble;
    }
    else if (JSON_TYPE_STRING == it->type) {
      if (cJSON_String != item->type || strlen(item->valuestring) + 1 > JSON_STRING_SIZE_MAX)  return -1;
      strcpy(it->val, item->valuestring);
    }
    else if (JSON_TYPE_OBJ == it->type) {
      if (cJSON_Object != item->type) return -1;
      if (parse_obj(item, (json_elem_t *)it->val) != 0) return -1;
    }
    else if (JSON_TYPE_ARRAY == it->type) {
      if (cJSON_Array != item->type) return -1;
      json_array_t desp = {it->arr_type, it->arr_size, it->val};
      if (parse_array(item, &desp) != 0) return -1;
    }
    else {
      return -1;
    }
  }

  return 0;
}

int transfer_json_to_data(const char *json_text, json_elem_t *elem)
{
  if (!json_text || !elem) return -1;
  
  cJSON *root = cJSON_Parse(json_text);
  if (NULL == root)
    return -1;

  int ret = parse_obj(root, elem);
  cJSON_Delete(root);
  return ret;
}

static int pack_array(cJSON *node, json_array_t *desp)
{
  int i;
  cJSON *item;
  for (i = 0; i < desp->arr_size; ++i) {
    if (JSON_TYPE_INT == desp->arr_type) {
      item = cJSON_CreateNumber( (double) (((int *)desp->val)[i]) );
    }
    else if (JSON_TYPE_DOUBLE == desp->arr_type) {
      item = cJSON_CreateNumber( (double) (((double *)desp->val)[i]) );
    }
    else if (JSON_TYPE_STRING == desp->arr_type) {
      item = cJSON_CreateString( ((char **)desp->val)[i] );
    }
    else if (JSON_TYPE_OBJ == desp->arr_type) {
      item = cJSON_CreateObject();
      if (!item) return -1;
      if (pack_obj(item, ((json_elem_t **)desp->val)[i] ) != 0) {cJSON_Delete(item); return -1;}
    }
    else if (JSON_TYPE_ARRAY == desp->arr_type) {
      item = cJSON_CreateArray();
      if (!item) return -1;
      if (pack_array(item, ((json_array_t *)desp->val) + i ) != 0) {cJSON_Delete(item); return -1;}
    }
    else {
      return -1;
    }

    if (!item) return -1;
    cJSON_AddItemToArray(node, item);
  }

  return 0;
}

static int pack_obj(cJSON *node, json_elem_t *json_elem)
{
  cJSON *item;
  json_elem_t *it;
  
  for (it = json_elem; it->name != NULL; ++it) {
    if (JSON_TYPE_INT == it->type) {
      item = cJSON_CreateNumber((double)(*(int *)it->val));
      if (!item) return -1;
    }
    else if (JSON_TYPE_DOUBLE == it->type) {
      item = cJSON_CreateNumber((double)(*(double *)it->val));
      if (!item) return -1;
    }
    else if (JSON_TYPE_STRING == it->type) {
      item = cJSON_CreateString((char *)it->val);
      if (!item) return -1;
    }
    else if (JSON_TYPE_OBJ == it->type) {
      item = cJSON_CreateObject();
      if (!item) return -1;
      if (pack_obj(item, it->val) != 0) {cJSON_Delete(item); return -1;}
    }
    else if (JSON_TYPE_ARRAY == it->type) {
      item = cJSON_CreateArray();
      if (!item) return -1;
      json_array_t desp = {it->arr_type, it->arr_size, it->val};
      if (pack_array(item, &desp) != 0) {cJSON_Delete(item); return -1;}
    }
    else {
      return -1;
    }

    cJSON_AddItemToObject(node, it->name, item);
  }
  
  return 0;
}

char *transfer_data_to_json(json_elem_t *json_elem)
{
  if (!json_elem) return NULL;
  
  cJSON *root = cJSON_CreateObject();
  if (!root)
    return NULL;

  int ret = pack_obj(root, json_elem);
  if (ret != 0)
    return NULL;

  char *text = cJSON_PrintUnformatted(root);
  //char *text = cJSON_Print(root); //for debug
  cJSON_Delete(root);
  return text;
}

