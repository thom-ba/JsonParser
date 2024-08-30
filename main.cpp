#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <cctype>
#include <stdexcept>

#define UNIMPLEMENTED() \
    do { \
        std::cerr << "UNIMPLEMENTED, in: " << __func__ << " line: " << __LINE__ << " / " << __FILE__ << std::endl; \
        std::exit(EXIT_FAILURE); \
    } while (0)

enum class JSONType {
    Object,
    Array,
    String, 
    Number,
    Boolean,
    Null,
};

class JSONObject; 

class JSONValue {
public:
    JSONType type;
    std::string stringValue;
    JSONObject* objectValue;

    explicit JSONValue(std::string value) : 
        type(JSONType::String), stringValue(std::move(value)), objectValue(nullptr) {}

    explicit JSONValue(JSONObject* object) :
        type(JSONType::Object), objectValue(object) {}
};

class JSONObject {
public:
    std::map<std::string, JSONValue*> values;

    ~JSONObject() {
        for (auto& pair : values) {
            delete pair.second;
        }
    }
};

class JSONParser {
public:
    const std::string input;
    size_t index;

    JSONParser(std::string input) : input(std::move(input)), index(0) {}

    char peek() const { return input[index]; }
    char get() { return input[index++]; }
    void skipWhitespace() {
        while (index < input.length() && std::isspace(peek())) {
            index++;
        }
    }

    JSONValue parseValue() {
        skipWhitespace();
        char ch = peek();

        if (ch == '"') {
            return JSONValue(parseString());
        }
        if (ch == '{') {
            return JSONValue(parseObject());
        }

        throw std::runtime_error("Error while parsing");
    }

    std::string parseString() {
        std::ostringstream oss;
        get();  // Skip the starting "
        while (peek() != '"') {
            oss << get();
        }
        get();  // Skip the ending "
        return oss.str();
    }

    JSONObject* parseObject() {
        auto* object = new JSONObject();
        get();  // Skip the starting '{'
        skipWhitespace();

        while (peek() != '}') {
            auto key = parseString();
            skipWhitespace();
            if (get() != ':') {
                throw std::runtime_error("Expected ':' after key in object");
            }
            skipWhitespace();
            JSONValue* jsonValue = new JSONValue(parseValue());
            object->values[key] = jsonValue;
            skipWhitespace();
            if (peek() == ',') get();  // Skip ','
        }
        get();  // Skip the ending '}'
        return object;
    }
};

void printJson() {
    UNIMPLEMENTED();
}

int main() {
    std::ifstream fin("input.txt");
    if (!fin) {
        std::cerr << "Failed to open file." << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << fin.rdbuf();
    fin.close();

    JSONParser jP(buffer.str());

    try {
        JSONValue value = jP.parseValue();
        std::cout << "JSON parsed successfully!" << std::endl;
        // Clean up
        if (value.type == JSONType::Object) {
            delete value.objectValue;
        }
    } catch (const std::exception& e) {
        std::cerr << "Parsing error: " << e.what() << std::endl;
    }

    return 0;
}

