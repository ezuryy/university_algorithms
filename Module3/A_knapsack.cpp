#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

struct Stuff {
    size_t weight = 0;
    size_t cost = 0;

    Stuff() = default;
    Stuff(const size_t& item_weight, const size_t& item_cost)
        : weight(item_weight), cost(item_cost) {}
    ~Stuff() = default;
};

vector<size_t> fill_knapsack(const size_t& volume, const vector<Stuff>& stuff) {
    size_t total_cost = 0;
    std::for_each(stuff.begin(), stuff.end(),
                  [&](Stuff stuff) { total_cost += stuff.cost; });
    vector<size_t> min_weights(total_cost + 1, volume + 1);
    min_weights[0] = 0;
    vector<vector<size_t>> content(total_cost + 1);
    vector<size_t> final_content;
    size_t max_cost = 0;

    for (size_t i = 1; i <= stuff.size(); ++i) {
        for (size_t cost = total_cost; cost > 0; --cost) {
            if (cost >= stuff[i - 1].cost &&
                min_weights[cost] > min_weights[cost - stuff[i - 1].cost] + stuff[i - 1].weight) {

                min_weights[cost] = min_weights[cost - stuff[i - 1].cost] + stuff[i - 1].weight;
                content[cost] = content[cost - stuff[i - 1].cost];
                content[cost].push_back(i);
                if (cost > max_cost) {
                    final_content = vector<size_t>(content[cost].begin(), content[cost].end());
                    max_cost = cost;
                }
            }
        }
    }
    return final_content;
}

std::pair<size_t, size_t> get_weight_and_cost(const vector<Stuff>& stuff,
                                              const vector<size_t>& knapsack) {
    size_t weight = 0;
    size_t cost = 0;
    for (const auto& item : knapsack) {
        weight += stuff[item - 1].weight;
        cost += stuff[item - 1].cost;
    }
    return {weight, cost};
}

bool is_number(const string& input) {
    return std::all_of(input.begin(), input.end(),
                       [](char c) { return std::isdigit(c); });
}

std::pair<size_t, size_t> parse_input(const string& input) {
    auto pos = std::find(input.begin(), input.end(), ' ');
    if (pos != input.end()) {
        string first = string(input.begin(), pos);
        string second = string(pos + 1, input.end());
        if (is_number(first) && is_number(second)) {
            return {std::stoull(first), std::stoull(second)};
        }
    }
    throw std::invalid_argument("error");
}

int main() {
    string input;
    size_t volume = 0;
    while (std::getline(std::cin, input)) {
        if (input.empty()) {
            continue;
        } else if (is_number(input)) {
            volume = std::stoull(input);
            break;
        } else {
            std::cout << "error\n";
        }
        input.clear();
    }

    vector<Stuff> stuff;

    while (std::getline(std::cin, input)) {
        try {
            auto [weight, cost] = parse_input(input);
            if (weight == 0 && cost == 0) {
                std::cout << "error\n";
            } else {
                stuff.emplace_back(weight, cost);
            }
            input.clear();
        } catch (...) {
            std::cout << "error\n";
            continue;
        }
    }

    vector<size_t> content = fill_knapsack(volume, stuff);

    auto [final_weight, final_cost] = get_weight_and_cost(stuff, content);

    std::cout << final_weight << " " << final_cost << "\n";
    for (const auto& index : content) {
        std::cout << index << "\n";
    }
    return 0;
}
