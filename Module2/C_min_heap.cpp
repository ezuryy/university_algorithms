#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>

using std::cin;
using std::cout;
using std::istream;
using std::ostream;
using std::pair;
using std::string;
using std::unordered_map;
using std::vector;

template <typename K, typename V>
class min_heap {
private:
    size_t get_parent(const size_t& index) const;
    
    size_t get_left_child(const size_t& index) const;
    
    size_t get_right_child(const size_t& index) const;
    
    void down_heapify(const size_t& index);
    
    void up_heapify(const size_t& index);
    
public:
    min_heap() = default;
    
    ~min_heap() = default;
    
    void add(const K& key, const V& value);
    
    void set(const K& key, const V& value);
    
    void remove(const K& key);
    
    bool search(const K& key, size_t& index, V& value);
    
    void min(K& key, size_t& index, V& value);
    
    void max(K& key, size_t& index, V& value);
    
    std::pair<K, V> extract();
    
    friend void print(const min_heap<int64_t, string>& tree, ostream& out);
    
protected:
    vector<pair<K, V>> heap;
    unordered_map<K, size_t> hash_table;
};

template <typename K, typename V>
size_t min_heap<K, V>::get_parent(const size_t& index) const {
    if (index == 0) {
        return 0;
    }
    return std::floor((index - 1) / 2);
}

template <typename K, typename V>
size_t min_heap<K, V>::get_left_child(const size_t& index) const {
    return 2 * index + 1;
}

template <typename K, typename V>
size_t min_heap<K, V>::get_right_child(const size_t& index) const {
    return 2 * index + 2;
}

template <typename K, typename V>
void min_heap<K, V>::down_heapify(const size_t& index) {
    size_t current_ind = index;
    size_t left_ind = get_left_child(current_ind);
    size_t right_ind = get_right_child(current_ind);
    
    while (current_ind < heap.size() / 2 &&
           (heap[current_ind] > heap[left_ind] ||
            (right_ind < heap.size() && heap[current_ind] > heap[right_ind]))) {
        if (right_ind >= heap.size() || heap[left_ind] <= heap[right_ind]) {
            hash_table[heap[left_ind].first] = current_ind;
            hash_table[heap[current_ind].first] = left_ind;
            std::swap(heap[left_ind], heap[current_ind]);
            current_ind = left_ind;
        } else {
            hash_table[heap[right_ind].first] = current_ind;
            hash_table[heap[current_ind].first] = right_ind;
            std::swap(heap[right_ind], heap[current_ind]);
            current_ind = right_ind;
        }
        
        left_ind = get_left_child(current_ind);
        right_ind = get_right_child(current_ind);
    }
}

template <typename K, typename V>
void min_heap<K, V>::up_heapify(const size_t& index) {
    size_t current_ind = index;
    size_t parent_ind = get_parent(current_ind);
    
    while (current_ind != 0 && heap[parent_ind] > heap[current_ind]) {
        hash_table[heap[parent_ind].first] = current_ind;
        hash_table[heap[current_ind].first] = parent_ind;
        std::swap(heap[parent_ind], heap[current_ind]);
        current_ind = parent_ind;
        parent_ind = get_parent(current_ind);
    }
}

template <typename K, typename V>
void min_heap<K, V>::add(const K& key, const V& value) {
    if (hash_table.find(key) != hash_table.end()) {
        throw std::invalid_argument("such element is already in min heap");
    }
    heap.push_back(pair<K, V>(key, value));
    hash_table.insert(std::make_pair(key, heap.size() - 1));
    up_heapify(heap.size() - 1);
}

template <typename K, typename V>
void min_heap<K, V>::set(const K& key, const V& value) {
    auto iter = hash_table.find(key);
    if (iter == hash_table.end()) {
        throw std::invalid_argument("no such element");
    }
    heap[iter->second].second = value;
}

template <typename K, typename V>
void min_heap<K, V>::remove(const K& key) {
    auto iter = hash_table.find(key);
    if (iter == hash_table.end()) {
        throw std::invalid_argument("no such element");
    }
    size_t index = iter->second;
    hash_table[heap[heap.size() - 1].first] = index;
    std::swap(heap[index], heap[heap.size() - 1]);
    heap.pop_back();
    hash_table.erase(iter);
    if (key < heap[index].first) {
        down_heapify(index);
    } else {
        up_heapify(index);
    }
}

template <typename K, typename V>
bool min_heap<K, V>::search(const K& key, size_t& index, V& value) {
    auto iter = hash_table.find(key);
    if (iter == hash_table.end()) {
        return false;
    }
    index = iter->second;
    value = heap[index].second;
    return true;
}

template <typename K, typename V>
void min_heap<K, V>::min(K& key, size_t& index, V& value) {
    if (heap.empty()) {
        throw std::out_of_range("min heap is empty");
    }
    index = 0;
    key = heap[0].first;
    value = heap[0].second;
}

template <typename K, typename V>
void min_heap<K, V>::max(K& key, size_t& index, V& value) {
    if (heap.empty()) {
        throw std::out_of_range("min heap is empty");
    }
    key = heap[0].first;
    for (size_t i = heap.size() / 2; i < heap.size(); ++i) {
        if (key < heap[i].first) {
            key = heap[i].first;
        }
    }
    index = hash_table.find(key)->second;
    value = heap[index].second;
}

template <typename K, typename V>
std::pair<K, V> min_heap<K, V>::extract() {
    if (heap.empty()) {
        throw std::invalid_argument("min heap is empty");
    }
    pair<K, V> result;
    hash_table[heap[heap.size() - 1].first] = 0;
    std::swap(heap[0], heap[heap.size() - 1]);
    result = heap.back();
    heap.pop_back();
    hash_table.erase(hash_table.find(result.first));
    down_heapify(0);
    return result;
}

void print(const min_heap<int64_t, string>& tree, ostream& out) {
    if (tree.heap.empty()) {
        out << "_\n";
        return;
    }
    out << "[" << tree.heap[0].first << " " << tree.heap[0].second << "]\n";
    if (tree.heap.size() == 1) {
        return;
    }
    size_t i = 1;
    size_t current_line = 1;
    size_t max_index_in_line = 2;
    while (i <= max_index_in_line) {
        if (i >= tree.heap.size()) {
            out << "_";
        } else {
            out << "[" << tree.heap[i].first << " ";
            out << tree.heap[i].second << " ";
            out << tree.heap[tree.get_parent(i)].first << "]";
        }
        if (i == max_index_in_line) {
            out << "\n";
            if (i + 1 >= tree.heap.size()) {
                break;
            }
            ++current_line;
            max_index_in_line += static_cast<size_t>(std::pow(2, current_line));
        } else {
            out << " ";
        }
        ++i;
    }
}

template <typename K, typename V>
void handler(min_heap<K, V>& tree, istream& in, ostream& out) {
    string command;
    K key;
    V value;
    size_t index = 0;
    while (!in.eof()) {
        in >> command;
        if (command.empty()) {
            continue;
        } else if (command == "add") {
            in >> key >> value;
            try {
                tree.add(key, value);
            } catch (...) {
                out << "error\n";
            }
        } else if (command == "set") {
            in >> key >> value;
            try {
                tree.set(key, value);
            } catch (...) {
                out << "error\n";
            }
        } else if (command == "delete") {
            in >> key;
            try {
                tree.remove(key);
            } catch (...) {
                out << "error\n";
            }
        } else if (command == "search") {
            in >> key;
            if (tree.search(key, index, value)) {
                out << "1 " << index << " " << value << "\n";
            } else {
                out << "0\n";
            }
        } else if (command == "min") {
            try {
                tree.min(key, index, value);
                out << key << " " << index << " " << value << "\n";
            } catch (...) {
                out << "error\n";
            }
        } else if (command == "max") {
            try {
                tree.max(key, index, value);
                out << key << " " << index << " " << value << "\n";
            } catch (...) {
                out << "error\n";
            }
        } else if (command == "extract") {
            try {
                pair<K, V> result = tree.extract();
                out << result.first << " " << result.second << "\n";
            } catch (...) {
                out << "error\n";
            }
            
        } else if (command == "print") {
            print(tree, out);
        } else {
            out << "error\n";
        }
        command.clear();
    }
}

int main(int argc, const char* argv[]) {
    min_heap<int64_t, string> tree;
    handler(tree, cin, cout);
    
    return 0;
}
