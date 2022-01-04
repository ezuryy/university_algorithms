#include <iostream>
#include <vector>

enum Move { increment, decrement, doubling };

struct ZeroCounter {
    size_t zero_count = 0;
    size_t number_after_divs = 0;

    ZeroCounter() = default;
    ZeroCounter(const size_t& count, const size_t& number)
        : zero_count(count), number_after_divs(number) {}
    ~ZeroCounter() = default;
};

ZeroCounter count_zeros(size_t number) {
    size_t counter = 0;
    while (number > 0 && number % 2 == 0) {
        number >>= 1;
        ++counter;
    }
    return {counter, number};
}

std::vector<Move> find_shortest_sequence(size_t number) {
    std::vector<Move> moves;
    while (number > 0) {
        if (number % 2 == 0) {
            number >>= 1;
            moves.push_back(Move::doubling);
            continue;
        }
        ZeroCounter previous = count_zeros(number - 1);
        ZeroCounter next = count_zeros(number + 1);
        ZeroCounter next_position;
        if (number <= 3 || previous.zero_count >= next.zero_count) {
            moves.push_back(Move::increment);
            next_position = previous;
        } else {
            moves.push_back(Move::decrement);
            next_position = next;
        }
        for (size_t i = 0; i < next_position.zero_count; ++i) {
            moves.push_back(Move::doubling);
        }
        number = next_position.number_after_divs;
    }
    return moves;
}

int main() {
    size_t number = 0;
    std::cin >> number;

    std::vector<Move> moves = find_shortest_sequence(number);

    for (auto iterator = moves.rbegin(); iterator != moves.rend(); ++iterator) {
        switch (*iterator) {
            case increment:
                std::cout << "inc\n";
                break;
            case decrement:
                std::cout << "dec\n";
                break;
            case doubling:
                std::cout << "dbl\n";
                break;
        }
    }
    return 0;
}
