#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <cctype>

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
    int numberValue;
    std::nullptr_t nullValue;
    bool boolValue;

    explicit JSONValue(std::string value) : 
        type(JSONType::String), stringValue(std::move(value)) {}

    explicit JSONValue(JSONObject* object) :
        type(JSONType::Object), objectValue(object) {}

    explicit JSONValue(int number) :
        type(JSONType::Number), numberValue(number) {}

    explicit JSONValue(bool boolValue) :
        type(JSONType::Boolean), numberValue(boolValue) {}

    explicit JSONValue(std::nullptr_t null) :
        type(JSONType::Null), nullValue(null) {}
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
        if (std::isdigit(ch) || ch == '-')  {
            return JSONValue(parseNumber());
        }
        if (input.substr(index,  4) == "true") return JSONValue(parseTrue());
        if (input.substr(index, 5) == "false") return JSONValue(parseFalse()); 
        if (input.substr(index, 4) == "null") return JSONValue(parseNull());

        printf("Error while parsing at char: %c", peek());
        exit(-1);
    }

    bool parseTrue() {
        // printf("Debug: Parsing true\n");
        index += 4;
        return true;
    }

    bool parseFalse() {
        // printf("Debug: Parsing false\n");
        index += 5;
        return false;
    }

    std::nullptr_t parseNull() {
        // printf("Debug: Parsing null\n");
        index += 4;
        return NULL;
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

    int parseNumber() {
        std::string result;
        skipWhitespace();
        while(isdigit(peek()) || peek() == '.' || peek() == '-') {
            result += get();
        }

        return std::stoi(result);
    } 
    int count =  0;
    JSONObject* parseObject() {
        auto* object = new JSONObject();
        get();  // Skip the starting '{'
        
        while (peek() != '}') {
            skipWhitespace();
            auto key = parseString();
            skipWhitespace();
            if (get() != ':') {
                printf("%d\n", count); 
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

void printJson(JSONObject object) {
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

