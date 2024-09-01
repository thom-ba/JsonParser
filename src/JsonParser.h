#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
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

  void
  print() const;
  // JSONType type;
  // JSONObject *objectValue;
  // JSONArray *arrayValue;
  // std::string stringValue;
  // std::nullptr_t nullValue;
  // int numberValue;
  // bool boolValue;
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
