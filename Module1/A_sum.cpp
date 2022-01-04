#include <iostream>
#include <string>
#include <sstream>
#include <regex>

using std::string;

void parse(const char& a, const char& b, string& str) {
    if (a >= '0' && a <= '9')
        str += a;
    else if (a == '-' && b >= '0' && b <= '9')
        str += a;
    else
        str += ' ';
}

long long get_sum(string& input) {
    string full_input = regex_replace(input, std::regex(" +"), " ");
    std::stringstream inout;
    inout << full_input << ' ';
    int current = 0;
    long long summary = 0;
    while (!inout.eof()) {
        summary += current;
        inout >> current;
    }
    return summary;
}

int main() {
    string str;
    char a = getchar();
    char b = getchar();
    while(b != EOF) {
        parse(a, b, str);
        a = b;
        b = getchar();
    }
    
    std::cout << get_sum(str) << '\n';
}
