// (C) Thomas Baumeister, 2024
// For further information read the comment at the end of the file.



#include "JsonParser.h"

char
JSONParser::peek() const
{
  return input[index];
}

char
JSONParser::get()
{
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

void
JSONValue::print() const
{
  switch (type)
  {
  case JSONType::Object:
    objectValue->print();
    break;

  case JSONType::Array:
    arrayValue->print();
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
  get(); // Skip the starting "
  while (peek() != '"')
  {
    oss << get();
  }
  get(); // Skip the ending "
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
    array->values.push_back(parseValue());
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
