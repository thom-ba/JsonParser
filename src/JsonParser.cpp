// (C) Thomas Baumeister, 2024
// For further information read the comment at the end of the file.

#include "JsonParser.h"

char
JSONParser::peek() const
{
  if (index >= input.length())
    throw std::out_of_range("Index out of bounds");
  return input[index];
}

char
JSONParser::get()
{
  if (index >= input.length())
    throw std::out_of_range("Index out of bounds");
  return input[index++];
}

void
JSONParser::skip_white_spaces()
{
  while (index < input.length() && std::isspace(peek()))
  {
    index++;
  }
}

JSONValue *
JSONObject::getValue(std::string key) const
{
  auto it = values.find(key);
  if (it != values.end())
  {
    return it->second;
  }

  throw std::runtime_error("Key not found or value is null: " + key);
}

void
JSONObject::print()
{
  for (const auto &val : values)
  {
    if (val.first.empty())
    {
      printf("NaN: ");
    }
    else
    {
      printf("%s: ", val.first.c_str());
    }
    val.second->print();

    printf("\n");
  }
}

void
JSONArray::print()
{
  printf("\n");
  for (const auto &value : values)
  {
    value.print();
  }
}

JSONValue
JSONParser::parseValue()
{
  skip_white_spaces();
  char ch = peek();

  if (ch == '"')
  {
    return JSONValue(parseString());
  }
  if (ch == '{')
  {
    return JSONValue(parseObject());
  }
  if (ch == '[')
  {
    return JSONValue(parseArray());
  }
  if (std::isdigit(ch) || ch == '-')
  {
    return JSONValue(parseNumber());
  }
  if (input.substr(index, 4) == "true")
    return JSONValue(parseTrue());
  if (input.substr(index, 5) == "false")
    return JSONValue(parseFalse());
  if (input.substr(index, 4) == "null")
    return JSONValue(parseNull());

  printf("Error while parsing at char: %c", peek());
  exit(-1);
}

void *
JSONValue::getActVal() const
{
  switch (type)
  {
  case JSONType::Object:
    return objectValue;

  case JSONType::Array:
    return arrayValue;

  case JSONType::String:
    return static_cast<void *>(new std::string(stringValue));

  case JSONType::Number:
    return static_cast<void *>(new int(numberValue));

  case JSONType::Boolean:
    return static_cast<void *>(new bool(boolValue));

  case JSONType::Null:
    return nullValue;

  default:
    throw std::runtime_error("Unknown JSON type");
  }
}

void
JSONValue::print() const
{
  switch (type)
  {
  case JSONType::Object:
    objectValue->print();
    break;

  case JSONType::Array:
    printf("[\n");
    for (const auto &item : arrayValue->values)
    {
      item.print();
      printf(",\n");
    }
    printf("]\n");
    break;

  case JSONType::String:
    printf("\"%s\"", stringValue.c_str());
    break;

  case JSONType::Number:
    printf("%d", numberValue);
    break;

  case JSONType::Boolean:
    printf("%s", (boolValue ? "true" : "false"));
    break;

  case JSONType::Null:
    printf("null");
    break;
  }
}

bool
JSONParser::parseTrue()
{
  index += 4;
  return true;
}

bool
JSONParser::parseFalse()
{
  index += 5;
  return false;
}

std::nullptr_t
JSONParser::parseNull()
{
  index += 4;
  return NULL;
}
std::string
JSONParser::parseString()
{
  std::ostringstream oss;
  get();
  while (peek() != '"')
  {
    char ch = get();
    if (ch == '\\')
    {
      char next = get();
      switch (next)
      {
      case '\"':
        oss << '\"';
        break;
      case '\\':
        oss << '\\';
        break;
      case 'n':
        oss << '\n';
        break;
      case 'r':
        oss << '\r';
        break;
      case 't':
        oss << '\t';
        break;
      default:
        oss << next;
        break;
      }
    }
    else
    {
      oss << ch;
    }
  }
  get();
  return oss.str();
}

int
JSONParser::parseNumber()
{
  std::string result;
  skip_white_spaces();
  while (isdigit(peek()) || peek() == '.' || peek() == '-')
  {
    result += get();
  }

  return std::stoi(result);
}

JSONObject *
JSONParser::parseObject()
{
  auto *object = new JSONObject();
  get(); // Skip the starting '{'

  while (peek() != '}')
  {
    skip_white_spaces();
    auto key = parseString();
    skip_white_spaces();
    if (get() != ':')
    {
      throw std::runtime_error("Expected ':' after key in object");
    }
    skip_white_spaces();
    JSONValue *jsonValue = new JSONValue(parseValue());
    object->values[key] = jsonValue;
    skip_white_spaces();
    if (peek() == ',')
      get(); // Skip ','
  }
  get(); // Skip the ending '}'
  return object;
}

JSONArray *
JSONParser::parseArray()
{
  auto *array = new JSONArray();
  get(); // Skip first '['

  while (peek() != ']')
  {
    skip_white_spaces();
    JSONValue value = parseValue();
    array->values.push_back(value);
    skip_white_spaces();
    if (peek() == ',')
      get();
  }

  get();
  return array;
}

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
