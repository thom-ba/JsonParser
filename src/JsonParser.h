// (C) Thomas Baumeister, 2024
// For further information read the comment at the end of the file.

#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <cctype>
#include <climits>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

enum class JSONType
{
  Object,
  Array,
  String,
  Number,
  Boolean,
  Null,
};

#define UNIMPLEMENTED()                                                        \
  do                                                                           \
  {                                                                            \
    std::cerr << "UNIMPLEMENTED, in: " << __func__ << " line: " << __LINE__    \
              << " / " << __FILE__ << std::endl;                               \
    std::exit(EXIT_FAILURE);                                                   \
  } while (0)

class JSONObject;
class JSONArray;

class JSONValue
{
public:
  JSONType type;
  JSONObject *objectValue;
  JSONArray *arrayValue;
  std::string stringValue;
  std::nullptr_t nullValue;
  int numberValue;
  bool boolValue;

  JSONValue(std::string value)
      : type(JSONType::String), stringValue(std::move(value))
  {
  }

  explicit JSONValue(JSONObject *object)
      : type(JSONType::Object), objectValue(object)
  {
  }

  explicit JSONValue(JSONArray *arr) : type(JSONType::Array), arrayValue(arr) {}

  explicit JSONValue(int number) : type(JSONType::Number), numberValue(number)
  {
  }

  explicit JSONValue(bool boolValue)
      : type(JSONType::Boolean), numberValue(boolValue)
  {
  }

  explicit JSONValue(std::nullptr_t null)
      : type(JSONType::Null), nullValue(null)
  {
  }

  using ValueVariant = std::variant<std::string, JSONObject *, JSONArray *, int,
                                    bool, std::nullptr_t>;

  void*
  getActVal() const;

  void
  print() const;
};

class JSONObject
{
public:
  std::map<std::string, JSONValue *> values;

  ~JSONObject()
  {
    for (auto &pair : values)
    {
      delete pair.second;
    }
  }
  
  JSONValue*  
  getValue(std::string val) const;

  void
  print();
};

class JSONArray
{
public:
  std::vector<JSONValue> values;

  ~JSONArray()
  {
    for (auto &value : values)
    {
      delete &value;
    }
  }

  void
  print();
};

class JSONParser
{
public:
  const std::string input;
  size_t index;

  JSONParser(std::string input) : input(std::move(input)), index(0) {}

  char
  peek() const;
  char
  get();
  void
  skip_white_spaces();

  JSONValue
  parseValue();
  JSONObject *
  parseObject();
  JSONArray *
  parseArray();

  bool
  parseTrue();
  bool
  parseFalse();
  std::nullptr_t
  parseNull();
  std::string
  parseString();
  int
  parseNumber();
};
#endif // JSONPARSER_H

// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
//
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// For more information, please refer to <https://unlicense.org/>
