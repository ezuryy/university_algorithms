#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::istream;

class Stack {
private:
    size_t capacity;
    size_t size;
    string* array;
    
public:
    Stack() : capacity(0), size(0), array(nullptr) {}
    
    Stack(const size_t cap) : capacity(cap), size(0) {
        array = new string[capacity];
    }
    
    ~Stack() {
        delete [] array;
    }
    
    bool empty() {
        return size == 0;
    }
    
    bool is_array_set() {
        return capacity != 0;
    }
    
    void set_capacity(const size_t cap) {
        if (!is_array_set()) {
            capacity = cap;
            array = new string[capacity];
        } else {
            std::cout << "error\n";
        }
    }
    
    void push(const string& x) {
        if (!is_array_set()) {
            std::cout << "error\n";
            return;
        }
        if (size == capacity) {
            std::cout << "overflow\n";
            return;
        }
        array[size] = x;
        size++;
    }
    
    string pop() {
        if (!is_array_set()) return "error";
        if (empty()) return "underflow";
        size--;
        return array[size];
    }
    
    string print() {
        if (!is_array_set()) return "error";
        if (empty()) return "empty";
        string result;
        for (size_t i = 0; i < size; ++i) {
            result += array[i];
            if (i + 1 < size) {
                result += " ";
            }
        }
        return result;
    }
};

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<string> elems;
    std::stringstream ss;
    ss.str(s);
    string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

void command_handler(const string& command, Stack& stack) {
    if (command == "") {
        return;
    } else if (command == "print") {
        std::cout << stack.print() << "\n";
        return;
    } else if (command == "pop") {
        std::cout << stack.pop() << "\n";
        return;
    }
    
    std::vector<std::string> commands = split(command, ' ');
    if (commands.size() == 0) return;
    if (commands.size() == 1) {
        std::cout << "error\n";
    } else if (commands.size() == 2) {
        if (commands[0] == "set_size") {
            try {
                int capacity = std::stoi(commands[1]);
                if (capacity <= 0) {
                    std::cout << "error\n";
                } else {
                    stack.set_capacity(static_cast<size_t>(capacity));
                }
            } catch (...) {
                std::cout << "error\n";
            }
        } else if (commands[0] == "push") {
            stack.push(commands[1]);
        } else {
            std::cout << "error\n";
        }
    } else {
        std::cout << "error\n";
    }
}

int main() {
    string command;
    Stack stack;
    while(!std::cin.eof()) {
        std::getline(std::cin, command);
        command_handler(command, stack);
    }
}
