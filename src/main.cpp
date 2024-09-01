// (C) Thomas Baumeister, 2024
// For further information read the comment at the end of the file.

#include "JsonParser.h"

void
printPaths(const JSONValue &root)
{
  if (root.type != JSONType::Object)
  {
    std::cerr << "Root is not an object" << std::endl;
    return;
  }

  const JSONObject *rootObject =
      static_cast<const JSONObject *>(root.getActVal());
  try
  {
    // Obtain the value associated with the "routes" key
    JSONValue *routesValue = rootObject->getValue("routes");

    if (routesValue->type != JSONType::Array)
    {
      std::cerr << "\"routes\" is not an array" << std::endl;
      return;
    }

    const JSONArray *routesArray =
        static_cast<const JSONArray *>(routesValue->getActVal());
    for (const auto &item : routesArray->values)
    {
      if (item.type != JSONType::Object)
      {
        std::cerr << "Array item is not an object" << std::endl;
        continue;
      }

      const JSONObject *routeObject =
          static_cast<const JSONObject *>(item.getActVal());
      try
      {
        // Obtain the value associated with the "path" key
        JSONValue *pathValue = routeObject->getValue("path");

        if (pathValue->type != JSONType::String)
        {
          std::cerr << "\"path\" is not a string" << std::endl;
          continue;
        }

        std::string *pathString =
            static_cast<std::string *>(pathValue->getActVal());
        printf("path: %s\n", pathString->c_str());
      }
      catch (const std::runtime_error &e)
      {
        std::cerr << "Error: " << e.what() << std::endl;
      }
    }
  }
  catch (const std::runtime_error &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

int
main()
{
  std::ifstream fin("input.txt");
  if (!fin)
  {
    std::cerr << "Failed to open file." << std::endl;
    return 1;
  }

  std::stringstream jsonInput;
  jsonInput << fin.rdbuf();
  fin.close();

  JSONParser parser(jsonInput.str());
  try
  {
    JSONValue root = parser.parseValue();
    printPaths(root); // Print only the paths
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }
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
