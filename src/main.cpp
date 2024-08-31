#include "JsonParser.h"

int
main()
{
  std::ifstream fin("input.txt");
  if (!fin)
  {
    std::cerr << "Failed to open file." << std::endl;
    return 1;
  }

  std::stringstream buffer;
  buffer << fin.rdbuf();
  fin.close();

  JSONParser jP(buffer.str());

  try
  {
    JSONValue value = jP.parseValue();
    std::cout << "JSON parsed successfully!" << std::endl;
    // Clean up
    if (value.type == JSONType::Object)
    {
      delete value.objectValue;
    }
  }
  catch (const std::exception &e)
  {
    std::cerr << "Parsing error: " << e.what() << std::endl;
  }

  return 0;
}
