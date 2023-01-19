#include <iostream>
#include <algorithm>

enum class Color { RED, BLACK };

template <typename T>
struct Node {
    T key;
    Node *left;
    Node *right;
    Node *parent;
    Color color;

    Node() {
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }

    Node(T key) {
        this->key = key;
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }
};

template <typename T>
class RBTree {
 public:
    RBTree() {
        root = nullptr;
        sizee = 0;
    }
    RBTree(std::initializer_list<T> list) {
        std::for_each(list.begin(), list.end(), [this](T el) -> void { insert(el); });
    }

    void deleteTree(Node<T> *node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

    ~RBTree() {
        deleteTree(root);
    }

    void rotateRight(Node<T> *node) {
        Node<T> *parent = node->parent;
        Node<T> *left_child = node->left;
        node->left = left_child->right;
        if (left_child->right) {
            left_child->right->parent = node;
        }
        left_child->right = node;
        node->parent = left_child;
        rebuildTree(parent, node, left_child);
    }

    void rotateLeft(Node<T> *node) {
        Node<T> *parent = node->parent;
        Node<T> *right_child = node->right;
        node->right = right_child->left;
        if (right_child->left) {
            right_child->left->parent = node;
        }
        right_child->left = node;
        node->parent = right_child;
        rebuildTree(parent, node, right_child);
    }

    void rebuildTree(Node<T> *parent, Node<T> *child, Node<T> *new_child) {
        if (!parent) {
            root = new_child;
        } else if (parent->right == child) {
            parent->right = new_child;
        } else if (parent->left == child) {
            parent->left = new_child;
        } else {
            return;
        }
        if (new_child) {
            new_child->parent = parent;
        }
    }

    void insert(T key) {
        Node<T> *node = root;
        Node<T> *parent = nullptr;
        while (node) {
            parent = node;
            if (key > node->key) {
                node = node->right;
            } else if (key < node->key) {
                node = node->left;
            } else {
                return;
            }
        }
        Node<T> *new_node = new Node<T>(key);
        new_node->color = Color::RED;
        if (!parent) {
            root = new_node;
        } else if (key < parent->key) {
            parent->left = new_node;
        } else {
            parent->right = new_node;
        }
        new_node->parent = parent;
        sizee++;
        changeColors(new_node);
    }

    void changeColors(Node<T> *node) {
        Node<T> *parent = node->parent;
        if (!parent) {
            return;
        }
        // best case
        if (parent->color == Color::BLACK) {
            return;
        }
        Node<T> *grand_papa = parent->parent;
        if (!grand_papa) {
            parent->color = Color::BLACK;
            return;
        }
        Node<T> *uncle = findNodeUncle(parent);
        if (uncle && uncle->color == Color::RED) {
            parent->color = Color::BLACK;
            grand_papa->color = Color::RED;
            uncle->color = Color::BLACK;
            changeColors(grand_papa);
        } else if (parent == grand_papa->left) {
            if (node == parent->right) {
                rotateLeft(parent);
                parent = node;
            }
            rotateRight(grand_papa);
            parent->color = Color::BLACK;
            grand_papa->color = Color::RED;
        } else {
            if (node == parent->left) {
                rotateRight(parent);
                parent = node;
            }
            rotateLeft(grand_papa);
            parent->color = Color::BLACK;
            grand_papa->color = Color::RED;
        }
    }

    Node<T> *findNodeUncle(Node<T> *parent) {
        Node<T> *grand_papa = parent->parent;
        if (grand_papa->left == parent) {
            return grand_papa->right;
        } else if (grand_papa->right == parent) {
            return grand_papa->left;
        }
    }

    int size() const {
        return sizee;
    }

    bool empty() const {
        return sizee == 0;
    }

    T *lowerBound(T key) const {
        Node<T> *cur = root;
        Node<T> *res = nullptr;
        while (cur) {
            if (cur->key >= key) {
                res = cur;
                cur = cur->left;
            } else {
                cur = cur->right;
            }
        }
        return (res ? &res->key : nullptr);
    }

    T *find(T key) const {
        Node<T> *node = root;
        while (node) {
            if (key == node->key) {
                return &node->key;
            } else if (key >= node->key) {
                node = node->right;
            } else {
                node = node->left;
            }
        }
        return nullptr;
    }

    Node<T> *root{};
    int sizee;
};