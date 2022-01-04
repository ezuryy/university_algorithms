#include <iostream>
#include <queue>

using std::cin;
using std::cout;
using std::string;
using std::pair;

template <typename K, typename V>
struct Node {
    Node *parent;
    Node *left;
    Node *right;
    K key;
    V value;
    
    Node() : parent(nullptr), left(nullptr), right(nullptr) {}
    
    Node(Node<K, V> *p, Node<K, V> *l, Node<K, V> *r, const K &k, const V &v)
    : parent(p), left(l), right(r), key(k), value(v) {
        if (parent != nullptr) {
            if (key < parent->key)
                parent->left = this;
            else
                parent->right = this;
        }
    }
};

template <typename K, typename V>
class SplayTree {
private:
    void deleter(Node<K, V> *node);
    
    void rotate_to_right(Node<K, V> *child, Node<K, V> *par, Node<K, V> *subroot);
    
    void rotate_to_left(Node<K, V> *child, Node<K, V> *par, Node<K, V> *subroot);
    
    void update_subroot(Node<K, V> *child, Node<K, V> *subroot);
    
    void zig(Node<K, V> *node, Node<K, V> *p);
    
    void zig_zig(Node<K, V> *node, Node<K, V> *p, Node<K, V> *gp);
    
    void zig_zag(Node<K, V> *node, Node<K, V> *p, Node<K, V> *gp);
    
    void splay(Node<K, V> *node);
    
    Node<K, V> *find(const K &key) const;
    
    Node<K, V> *previous(const K &key);
    
    Node<K, V> *splay_max(Node<K, V> *subroot);
    
public:
    SplayTree() : root(nullptr) {}
    
    SplayTree(const SplayTree &) = delete;
    
    SplayTree &operator=(const SplayTree &) = delete;
    
    ~SplayTree() { deleter(root); }
    
    void add(const K &key, const V &value);
    
    void set(const K &key, const V &value);
    
    void remove(const K &key);
    
    bool search(const K &key, V &value);
    
    pair<K, V> min();
    
    pair<K, V> max();
    
    friend void print(const SplayTree<int64_t, string>& tree);
    
protected:
    Node<K, V> *root;
};

template <typename K, typename V>
void SplayTree<K, V>::deleter(Node<K, V>* node) {
    if (node != nullptr) {
        deleter(node->left);
        deleter(node->right);
        delete node;
    }
}

template <typename K, typename V>
void SplayTree<K, V>::rotate_to_right(Node<K, V>* child, Node<K, V>* par,
                                      Node<K, V>* subroot) {
    par->left = child->right;
    if (par->left != nullptr) par->left->parent = par;
    child->right = par;
    par->parent = child;
    update_subroot(child, subroot);
}

template <typename K, typename V>
void SplayTree<K, V>::rotate_to_left(Node<K, V>* child, Node<K, V>* par,
                                     Node<K, V>* subroot) {
    par->right = child->left;
    if (par->right != nullptr) par->right->parent = par;
    child->left = par;
    par->parent = child;
    update_subroot(child, subroot);
}

template <typename K, typename V>
void SplayTree<K, V>::update_subroot(Node<K, V>* child, Node<K, V>* subroot) {
    child->parent = subroot;
    if (subroot != nullptr) {
        if (child->key < subroot->key)
            subroot->left = child;
        else
            subroot->right = child;
    }
}

template <typename K, typename V>
void SplayTree<K, V>::zig(Node<K, V>* node, Node<K, V>* p) {
    Node<K, V>* subroot = p->parent;
    if (p->left == node) {
        rotate_to_right(node, p, subroot);
    } else {
        rotate_to_left(node, p, subroot);
    }
}

template <typename K, typename V>
void SplayTree<K, V>::zig_zig(Node<K, V>* node, Node<K, V>* p, Node<K, V>* gp) {
    Node<K, V>* subroot = gp->parent;
    if (p->left == node) {
        rotate_to_right(p, gp, subroot);
        rotate_to_right(node, p, subroot);
    } else {
        rotate_to_left(p, gp, subroot);
        rotate_to_left(node, p, subroot);
    }
}

template <typename K, typename V>
void SplayTree<K, V>::zig_zag(Node<K, V>* node, Node<K, V>* p, Node<K, V>* gp) {
    Node<K, V>* subroot = gp->parent;
    if (p->right == node) {
        rotate_to_left(node, p, subroot);
        rotate_to_right(node, gp, subroot);
    } else {
        rotate_to_right(node, p, subroot);
        rotate_to_left(node, gp, subroot);
    }
}

template <typename K, typename V>
void SplayTree<K, V>::splay(Node<K, V>* node) {
    if (node == nullptr) return;
    if (node->parent != nullptr) {
        Node<K, V>* p = node->parent;
        if (p->parent == nullptr) {
            zig(node, p);
            root = node;
            return;
        }
        Node<K, V>* gp = p->parent;
        if (gp != nullptr) {
            if ((gp->left == p && p->left == node) ||
                (gp->right == p && p->right == node)) {
                zig_zig(node, p, gp);
            } else {
                zig_zag(node, p, gp);
            }
        }
        if (node->parent != nullptr) {
            splay(node);
        }
    }
    root = node;
}

template <typename K, typename V>
Node<K, V>* SplayTree<K, V>::find(const K& key) const {
    Node<K, V>* current = root;
    while (current != nullptr) {
        if (key == current->key) {
            return current;
        } else if (key < current->key) {
            current = current->left;
        } else if (key > current->key) {
            current = current->right;
        }
    }
    return nullptr;
}

template <typename K, typename V>
Node<K, V>* SplayTree<K, V>::splay_max(Node<K, V>* subroot) {
    Node<K, V>* current = subroot;
    while (current != nullptr && current->right != nullptr)
        current = current->right;
    splay(current);
    return current;
}

template <typename K, typename V>
Node<K, V>* SplayTree<K, V>::previous(const K& key) {
    Node<K, V>* current = root;
    Node<K, V>* previous = root;
    while (current != nullptr) {
        if (key < current->key) {
            previous = current;
            current = current->left;
        } else if (key > current->key) {
            previous = current;
            current = current->right;
        }
    }
    return previous;
}

template <typename K, typename V>
void SplayTree<K, V>::add(const K& key, const V& value) {
    Node<K, V>* found = find(key);
    if (found != nullptr) {
        splay(found);
        throw std::invalid_argument("this element is already in splay tree");
    }
    Node<K, V>* prev = previous(key);
    auto* current = new Node<K, V>(prev, nullptr, nullptr, key, value);
    splay(current);
}

template <typename K, typename V>
void SplayTree<K, V>::set(const K& key, const V& value) {
    Node<K, V>* node = find(key);
    if (node == nullptr) {
        node = previous(key);
        splay(node);
        root = node;
        throw std::invalid_argument("no such element");
    }
    node->value = value;
    splay(node);
}

template <typename K, typename V>
void SplayTree<K, V>::remove(const K& key) {
    Node<K, V>* node = find(key);
    if (node == nullptr) {
        node = previous(key);
        splay(node);
        root = node;
        throw std::invalid_argument("no such element");
    }
    splay(node);
    if (node->left != nullptr && node->right != nullptr) {
        splay_max(node->left);
        splay(node);
        root = node->left;
        root->right = node->right;
        node->right->parent = root;
        root->parent = nullptr;
    } else if (node->left != nullptr && node->right == nullptr) {
        root = node->left;
        root->parent = nullptr;
    } else if (node->left == nullptr && node->right != nullptr) {
        root = node->right;
        root->parent = nullptr;
    } else {
        root = nullptr;
    }
    delete node;
}

template <typename K, typename V>
bool SplayTree<K, V>::search(const K& key, V& value) {
    Node<K, V>* node = find(key);
    if (node == nullptr) {
        splay(previous(key));
        return false;
    }
    splay(node);
    value = node->value;
    return true;
}

template <typename K, typename V>
pair<K, V> SplayTree<K, V>::min() {
    if (root == nullptr) {
        throw std::out_of_range("splay tree is empty");
    }
    Node<K, V>* current = root;
    while (current != nullptr && current->left != nullptr)
        current = current->left;
    splay(current);
    return pair<K, V>(current->key, current->value);
}

template <typename K, typename V>
pair<K, V> SplayTree<K, V>::max() {
    if (root == nullptr) {
        throw std::out_of_range("splay tree is empty");
    }
    Node<K, V>* maximum = splay_max(root);
    return pair<K, V>(maximum->key, maximum->value);
}

void print(const SplayTree<int64_t, string>& tree) {
    if (tree.root == nullptr) {
        cout <<  "_\n";
        return;
    }
    cout <<
    "[" << std::to_string(tree.root->key) << " " << tree.root->value << "]\n";
    string line;
    std::queue<Node<int64_t, string>*> q;
    q.push(tree.root->left);
    q.push(tree.root->right);
    Node<int64_t, string>* current;
    
    size_t element_counter = 0;
    size_t elements_in_line = 2;
    while (!q.empty()) {
        current = q.front();
        q.pop();
        if (current != nullptr) {
            line += "[" + std::to_string(current->key) + " " + current->value + " " +
            std::to_string(current->parent->key) + "]";
            q.push(current->left);
            q.push(current->right);
        } else {
            line += "_";
            q.push(nullptr);
            q.push(nullptr);
        }
        ++element_counter;
        if (element_counter == elements_in_line) {
            string null_line = "_";
            for (size_t j = 1; j < elements_in_line; ++j) null_line += " _";
            
            if (line != null_line) {
                cout << line << "\n";
                line.clear();
            } else {
                break;
            }
            elements_in_line *= 2;
            element_counter = 0;
        } else {
            line += " ";
        }
    }
}

void handler(SplayTree<int64_t, string>& tree) {
    string command;
    int64_t key;
    string value;
    
    while (!cin.eof()) {
        cin >> command;
        if (command.empty()) {
            continue;
        } else if (command == "add") {
            cin >> key >> value;
            try {
                tree.add(key, value);
            } catch (...) {
                cout << "error\n";
            }
        } else if (command == "set") {
            cin >> key >> value;
            try {
                tree.set(key, value);
            } catch (...) {
                cout << "error\n";
            }
        } else if (command == "delete") {
            cin >> key;
            try {
                tree.remove(key);
            } catch (...) {
                cout << "error\n";
            }
        } else if (command == "search") {
            cin >> key;
            if (!tree.search(key, value)) {
                cout << "0\n";
            } else {
                cout << "1 " << value << "\n";
            }
        } else if (command == "min") {
            try {
                pair<int64_t, string> found = tree.min();
                cout << found.first << " " << found.second << "\n";
            } catch (...) {
                cout << "error\n";
            }
        } else if (command == "max") {
            try {
                pair<int64_t, string> found = tree.max();
                cout << found.first << " " << found.second << "\n";
            } catch (...) {
                cout << "error\n";
            }
        } else if (command == "print") {
            print(tree);
        } else {
            cout << "error\n";
        }
        command.clear();
    }
}

int main() {
    SplayTree<int64_t, string> tree;
    handler(tree);
    return 0;
}
