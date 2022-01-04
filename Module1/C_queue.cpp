#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

using std::string;
using std::ofstream;
using std::ifstream;

class Queue {
private:
    size_t capacity;
    size_t size;
    size_t head;
    size_t tail;
    string* array;
    
public:
    Queue() : capacity(0), size(0), head(0), tail(0), array(nullptr) {}
    
    ~Queue() {
        delete[] array;
    }
    
    bool array_is_set() {
        return capacity != 0;
    }
    
    bool empty() {
        return size == 0;
    }
    
    void set_capacity(const size_t cap, ofstream& out) {
        if (array_is_set()) {
            out << "error\n";
            return;
        }
        capacity = cap;
        tail = capacity - 1;
        array = new string[capacity];
    }
    
    void push(const string& x, ofstream& out) {
        if (!array_is_set()) {
            out << "error\n";
            return;
        }
        if (size == capacity) {
            out << "overflow\n";
            return;
        }
        if (tail + 1 == capacity)
            tail = 0;
        else
            tail++;
        array[tail] = x;
        size++;
    }
    
    string pop() {
        if (!array_is_set()) return "error";
        if (empty()) return "underflow";
        size--;
        if (head + 1 == capacity) {
            head = 0;
            return array[capacity - 1];
        }
        return array[head++];
    }
    
    string print() {
        if (!array_is_set()) return "error";
        if (empty()) return "empty";
        string result;
        for (size_t i = head; i != tail; ++i) {
            if (i == capacity) {
                i = 0;
                if (i == tail) break;
            }
            result += array[i];
            result += " ";
        }
        result += array[tail];
        return result;
    }
};

std::vector<string> split(const string &s, char delim) {
    std::vector<string> elems;
    std::stringstream ss;
    ss.str(s);
    string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

void command_handler(const string& command, Queue& queue, ofstream& out) {
    if (command == "") {
        return;
    } else if (command == "print") {
        out << queue.print() << "\n";
        return;
    } else if (command == "pop") {
        out << queue.pop() << "\n";
        return;
    }
    
    std::vector<string> commands = split(command, ' ');
    if (commands.size() == 0) return;
    if (commands.size() == 1) {
        out << "error\n";
    } else if (commands.size() == 2) {
        if (commands[0] == "set_size") {
            try {
                int capacity = std::stoi(commands[1]);
                if (capacity <= 0) {
                    out << "error\n";
                } else {
                    queue.set_capacity(static_cast<size_t>(capacity), out);
                }
            } catch (...) {
                out << "error\n";
            }
        } else if (commands[0] == "push") {
            queue.push(commands[1], out);
        } else {
            out << "error\n";
        }
    } else {
        out << "error\n";
    }
}

int main(int argc, const char * argv[]) {
    if (argc < 3) {
        std::cerr << "arguments not found" << std::endl;
    }
    string input_filename = argv[1];
    string output_filename = argv[2];
    
    ifstream in(input_filename);
    ofstream out(output_filename);
    
    if (!in) {
        std::cerr << "can't open file " << input_filename << std::endl;
        exit(1);
    }
    if (!out) {
        std::cerr << "can't open file " << output_filename << std::endl;
        exit(1);
    }
    
    string command;
    Queue queue;
    while(!in.eof()) {
        std::getline(in, command);
        command_handler(command, queue, out);
    }
    in.close();
    out.close();
}
