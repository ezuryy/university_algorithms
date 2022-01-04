#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <locale>
#include <string>
#include <set>
#include <unordered_map>

using std::string;
using std::set;
using std::unordered_map;
using std::wstring;
using std::wistream;
using std::wostream;

struct Node {
    wchar_t key{};
    std::unordered_map<wchar_t, Node*> children;
    bool is_last = false;
    
    explicit Node(bool last) : key('\0'), is_last(last) {}
    Node(const wchar_t symbol, bool last) : key(symbol), is_last(last) {}
    ~Node() = default;
};

class Trie {
public:
    
    ~Trie() { deleter(root); }
    
    void add_word(const wstring& s, std::locale& loc);
    
    bool is_correct(const wstring& word, std::locale& loc) const;
    
    set<wstring> search_typo(const wstring& word, std::locale& loc) const;
    
    bool empty() {
        return root == nullptr;
    }
    
private:
    
    void deleter(Node* curr);
    
    void recursive_extra_or_missing(set<wstring>& answer, size_t extra_counter, size_t missing_counter, Node* curr, const wstring& dict_str, const wstring& my_str, size_t ind) const;
    
    void recursive_one_typo(set<wstring>& answer, size_t typo_counter, Node* curr, const wstring& dict_str, const wstring& my_str, size_t ind) const;
    
    void recursive_transposition(set<wstring>& answer, size_t typo_counter, wchar_t prev_symbol,
                                 Node* curr, const wstring& dict_str, const wstring& my_str, size_t ind) const;
    
    Node* root = nullptr;
};

// Добавление слова в префиксное дерево
//
// Сложность: O(n), где n - длина добавляемого слова
// Проходимся по слову длины n в цикле for, проверяя, что каждый символ добавляемого слова
// есть в дереве (либо в случае отсутствия добавляя его). Обращение к хэш-таблице с детьми выполняется в
// среднем за константу, таким образом, операции внутри цикла выполняются в среднем за константу

void Trie::add_word(const wstring& s, std::locale& loc) {
    if (!root) {
        root = new Node(false);
    }
    wstring lower_word = s;
    std::transform(lower_word.begin(), lower_word.end(), lower_word.begin(),
                   std::bind(std::tolower<wchar_t>,
                             std::placeholders::_1,
                             std::cref(loc)));
    Node* curr = root;
    for (wchar_t symbol : lower_word) {
        if (curr->children.find(symbol) == curr->children.end()) {
            curr->children[symbol] = new Node(symbol, false);
        }
        curr = curr->children[symbol];
    }
    curr->is_last = true;
}

void Trie::deleter(Node* curr) {
    if (curr != nullptr) {
        for (auto elem : curr->children) {
            deleter(elem.second);
        }
        delete curr;
    }
}

// Функция поиска похожих слов для случаев пропуска символа либо лишнего символа
//
// Сложность: O(n * m), где n - длина проверяемого слова, m - колическтво узлов в дереве
// Рекурсивно идем от корня к листьям, сравнивая значение текущего ключа и символа в проверяемом слове,
// для каждого узла если число ошибок корректно, то проверяем детей узла, иначе выходим

void Trie::recursive_extra_or_missing(set<wstring>& answer, size_t extra_counter, size_t missing_counter, Node* curr, const wstring& dict_str, const wstring& my_str, size_t ind) const {
    if (!curr) {
        return;
    }
    if (curr->is_last &&
        ((dict_str.size() == my_str.size() && my_str.size() == ind && extra_counter == 0 && missing_counter == 0) ||
         (dict_str.size() + 1 == my_str.size() && ((my_str.size() == ind + 1 && extra_counter == 0) || my_str.size() == ind) && missing_counter == 0) ||
         (dict_str.size() == my_str.size() + 1 && my_str.size() == ind && extra_counter == 0))) {
        answer.insert(dict_str);
    } else if (curr->is_last && (extra_counter > 1 || missing_counter > 1 || extra_counter * missing_counter != 0)) {
        return;
    }
    
    
    for (auto& element : curr->children) {
        if (ind < my_str.size() && element.first == my_str[ind]) {
            recursive_extra_or_missing(answer, extra_counter, missing_counter, element.second,
                                       dict_str + element.first, my_str, ind + 1);
        } else if (extra_counter == 0 && missing_counter == 0) {
            recursive_extra_or_missing(answer, extra_counter, missing_counter + 1, element.second, dict_str + element.first, my_str, ind);
            
            recursive_extra_or_missing(answer, extra_counter + 1, missing_counter, curr, dict_str, my_str, ind + 1);
        }
    }
}

// Функция поиска похожих слов для случая, когда в проверяемом слове один символ неправильный
//
// Сложность: O(n * m), где n - длина проверяемого слова, m - колическтво узлов в дереве
// Рекурсивно идем от корня к листьям, сравнивая значение текущего ключа и символа в проверяемом слове,
// для каждого узла если число ошибок корректно, то проверяем детей узла, иначе выходим

void Trie::recursive_one_typo(set<wstring>& answer, size_t typo_counter,
                              Node* curr, const wstring& dict_str, const wstring& my_str, size_t ind) const {
    if (!curr) {
        return;
    }
    if (curr->is_last && ind == my_str.size()) {
        answer.insert(dict_str);
        return;
    }
    for (auto& element : curr->children) {
        if (ind < my_str.size() && element.first == my_str[ind]) {
            recursive_one_typo(answer, typo_counter, element.second, dict_str + element.first, my_str, ind + 1);
        } else if (element.first && typo_counter == 0) {
            recursive_one_typo(answer, typo_counter + 1, element.second, dict_str + element.first, my_str, ind + 1);
        }
    }
}

// Функция поиска если похожих слов для случая транспозиции символов
//
// Сложность по времени: O(n * m), где n - длина проверяемого слова, m - колическтво узлов в дереве
// Рекурсивно идем от корня к листьям, сравнивая значение текущего ключа и символа в проверяемом слове,
// для каждого узла если число ошибок корректно, то проверяем детей узла, иначе выходим

void Trie::recursive_transposition(set<wstring>& answer, size_t typo_counter, wchar_t prev_symbol, Node* curr, const wstring& dict_str, const wstring& my_str, size_t ind) const {
    if (!curr) {
        return;
    }
    if (curr->is_last && ind == my_str.size()) {
        answer.insert(dict_str);
        return;
    }
    
    for (auto& element : curr->children) {
        if (ind < my_str.size() && element.first == my_str[ind] && typo_counter <= 2) {
            // символы одинаковые и не больше двух транспозиции
            recursive_transposition(answer, typo_counter, my_str[ind], element.second, dict_str + element.first, my_str,ind + 1);
        } else if (ind < my_str.size() && ((element.first == prev_symbol && curr->key == my_str[ind] &&
                                            curr->key != prev_symbol && typo_counter == 1) ||
                                           (element.first  != my_str[ind] && typo_counter == 0))) {
            // если транспозиция или если первая ошибка
            recursive_transposition(answer, typo_counter + 1, my_str[ind], element.second, dict_str + element.first, my_str, ind + 1);
        }
    }
}

// Функция проверки наличия слова в дереве
//
// Сложность: O(n), где n - длина проверяемого слова
// Проходимся в цикле for по проверяемому слову длины n, внутри цикла операции
// выполняются в среднем за константу, т.к. обращаемся к хэш-таблице

bool Trie::is_correct(const wstring& word, std::locale& loc) const {
    Node* curr = root;
    wstring lower_word = word;
    std::transform(lower_word.begin(), lower_word.end(), lower_word.begin(),
                   std::bind(std::tolower<wchar_t>,
                             std::placeholders::_1,
                             std::cref(loc)));
    for (wchar_t symbol : lower_word) {
        if (curr->children.find(symbol) == curr->children.end()) {
            return false;
        }
        curr = curr->children.at(symbol);
    }
    return curr->is_last;
}

// Функция поиска опечаток
//
// Сложность: O(n * m), где n - длина проверяемого слова, m - колическтво узлов в дереве
// Сначала проверяем, если ли в дереве проверяемое слово за О(n), если оно не нашлось, ищем опечатки,
// вызывая последоватеольно 3 функции, каждая из которых работает за O(n * m)

set<wstring> Trie::search_typo(const wstring& word, std::locale& loc) const {
    set<wstring> answer;
    wstring lower_word = word;
    std::transform(lower_word.begin(), lower_word.end(), lower_word.begin(),
                   std::bind(std::tolower<wchar_t>,
                             std::placeholders::_1,
                             std::cref(loc)));
    if (is_correct(lower_word, loc)) {
        answer.insert(lower_word);
        return answer;
    } else {
        recursive_transposition(answer, 0, '\0', root, L"", lower_word, 0);
        recursive_one_typo(answer, 0, root, L"", lower_word, 0);
        recursive_extra_or_missing(answer, 0, 0, root, L"", lower_word, 0);
    }
    return answer;
}

void handler(Trie& tree, std::wistream& in, std::wostream& out) {
    std::ios_base::sync_with_stdio(false);
    std::locale loc{""};
    in.imbue(loc);
    out.imbue(loc);
    wstring word;
    
    size_t dictionary_size = 0;
    in >> dictionary_size;
    
    for (size_t i = 0; i < dictionary_size;) {
        std::getline(in, word);
        if (!word.empty()) {
            tree.add_word(word, loc);
            ++i;
        }
    }
    
    in.ignore();
    in.clear();
    
    while (std::getline(in, word)) {
        if (!word.empty()) {
            if (tree.empty()) {
                out << word << " -?\n";
            } else if (tree.is_correct(word, loc)) {
                out << word << " - ok\n";
            } else {
                set<wstring> result = tree.search_typo(word, loc);
                if (result.empty() || tree.empty()) {
                    out << word << " -?\n";
                } else {
                    out << word << " -> ";
                    for (auto it = result.begin(); next(it) != result.end(); ++it) {
                        out << *it << ", ";
                    }
                    out << *prev(result.end()) << "\n";
                }
            }
            word.clear();
        }
    }
}

int main() {
    Trie tree;
    handler(tree, std::wcin, std::wcout);
    return 0;
}
