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
#ifndef __JSON_TRANSFER_H__
#define __JSON_TRANSFER_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define JSON_STRING_SIZE_MAX   256

typedef enum {
  JSON_TYPE_STRING = 0,
  JSON_TYPE_INT,
  JSON_TYPE_DOUBLE,
  JSON_TYPE_ARRAY,
  JSON_TYPE_OBJ,
  JSON_TYPE_MAX
} json_elem_type_e;

typedef struct {
  int arr_type;
  int arr_size;
  void *val;
} json_array_t;

typedef struct {
  const char *name;
  int type;
  int arr_type;  //only for array
  int arr_size;  //only for array
  void *val;
} json_elem_t;

#define ARRAY_SIZE(arr)   ( sizeof(arr) / sizeof((arr)[0]) )


int transfer_json_to_data(const char *json_text, json_elem_t *elem);
char *transfer_data_to_json(json_elem_t *elem);


#ifdef __cplusplus
}
#endif

#endif

