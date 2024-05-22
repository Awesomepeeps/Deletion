// Used Algorithm Tutor (deletion) and Parents (insertion) 
// Nikaansh S.

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// way to compare to Red and Black instead of 0 ans 1
enum Color { RED, BLACK };

// Struct for node with values initioalized at the beigninge
struct node {
    int data;
    node* left;
    node* right;
    node* parent;
    Color color;
    node(int data) : data(data), left(nullptr), right(nullptr), parent(nullptr), color(BLACK) {}
};

class RedBlackTree {
private:
    node* root;

    void leftRotate(node* current) {
        node* y = current->right;
        current->right = y->left;
        if (y->left != nullptr)
            y->left->parent = current;
        y->parent = current->parent;
        if (current->parent == nullptr)
            root = y;
        else if (current == current->parent->left)
            current->parent->left = y;
        else
            current->parent->right = y;
        y->left = current;
        current->parent = y;
    }

    void rightRotate(node* y) {
        node* current = y->left;
        y->left = current->right;
        if (current->right != nullptr)
            current->right->parent = y;
        current->parent = y->parent;
        if (y->parent == nullptr)
            root = current;
        else if (y == y->parent->left)
            y->parent->left = current;
        else
            y->parent->right = current;
        current->right = y;
        y->parent = current;
    }

    void ficurrentInsertion(node* z) {
        while (z != root && z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                node* y = z->parent->parent->right;
                if (y != nullptr && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            } else {
                node* y = z->parent->parent->left;
                if (y != nullptr && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    void transplant(node* current, node* comp) {
        if (current->parent == nullptr) {
            root = comp;
        }
        else if (current == current->parent->left) {
            current->parent->left = comp;
        }
        else {
            current->parent->right = comp;
        }
        comp->parent = current->parent;
    }

    node* minValueNode(node* minnode) {
        node* current = minnode;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }

    void ficurrentDeletion(node* current) {
        while (current != root && current->color == BLACK) {
            if (current == current->parent->left) {
                node* pRight = current->parent->right;
                if (pRight->color == RED) {
                    pRight->color = BLACK;
                    current->parent->color = RED;
                    leftRotate(current->parent);
                    pRight = current->parent->right;
                }
                if (pRight->left->color == BLACK && pRight->right->color == BLACK) {
                    pRight->color = RED;
                    current = current->parent;
                } else {
                    if (pRight->right->color == BLACK) {
                        pRight->left->color = BLACK;
                        pRight->color = RED;
                        rightRotate(pRight);
                        pRight = current->parent->right;
                    }
                    pRight->color = current->parent->color;
                    current->parent->color = BLACK;
                    pRight->right->color = BLACK;
                    leftRotate(current->parent);
                    current = root;
                }
            } else {
                node* pRight = current->parent->left;
                if (pRight->color == RED) {
                    pRight->color = BLACK;
                    current->parent->color = RED;
                    rightRotate(current->parent);
                    pRight = current->parent->left;
                }
                if (pRight->right->color == BLACK && pRight->left->color == BLACK) {
                    pRight->color = RED;
                    current = current->parent;
                } else {
                    if (pRight->left->color == BLACK) {
                        pRight->right->color = BLACK;
                        pRight->color = RED;
                        leftRotate(pRight);
                        pRight = current->parent->left;
                    }
                    pRight->color = current->parent->color;
                    current->parent->color = BLACK;
                    pRight->left->color = BLACK;
                    rightRotate(current->parent);
                    current = root;
                }
            }
        }
        current->color = BLACK;
    }

    void inorderHelper(node* root, int level) {
        if (root != nullptr) {
            inorderHelper(root->right, level + 1);
            for (int i = 0; i < level; i++)
                cout << "   ";
            cout << root->data;
            if (root->color == RED)
                cout << " (Red)" << endl;
            else
                cout << " (Black)" << endl;
            inorderHelper(root->left, level + 1);
        }
    }

public:
    RedBlackTree() : root(nullptr) {}

    void insert(int data) {
        node* z = nepRight node(data);
        node* y = nullptr;
        node* current = root;

        while (current != nullptr) {
            y = current;
            if (z->data < current->data)
                current = current->left;
            else
                current = current->right;
        }
        z->parent = y;
        if (y == nullptr)
            root = z;
        else if (z->data < y->data)
            y->left = z;
        else
            y->right = z;
        z->left = nullptr;
        z->right = nullptr;
        z->color = RED;
        ficurrentInsertion(z);
    }

    void remove(int data) {
        node* z = root;
        node* current, *y;
        while (z != nullptr) {
            if (z->data == data)
                break;
            if (z->data < data)
                z = z->right;
            else
                z = z->left;
        }
        if (z == nullptr) {
            cout << "Couldn't find key in the tree\n";
            return;
        }
        y = z;
        Color y_original_color = y->color;
        if (z->left == nullptr) {
            current = z->right;
            transplant(z, z->right);
        } else if (z->right == nullptr) {
            current = z->left;
            transplant(z, z->left);
        } else {
            y = minValueNode(z->right);
            y_original_color = y->color;
            current = y->right;
            if (y->parent == z)
                current->parent = y;
            else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        if (y_original_color == BLACK)
            ficurrentDeletion(current);
        delete z;
    }

    void search(int data) {
        node* node = root;
        while (node != nullptr) {
            if (data == node->data) {
                cout << "Found: " << data << endl;
                return;
            } else if (data < node->data)
                node = node->left;
            else
                node = node->right;
        }
        cout << "Not Found: " << data << endl;
    }

    void inorder() {
        inorderHelper(root, 0);
    }

    void readFromFile(const string& filename) {
        ifstream inputFile(filename);
        if (inputFile.is_open()) {
            int num;
            while (inputFile >> num) {
                insert(num);
            }
            inputFile.close();
        } else {
            cout << "Unable to open file: " << filename << endl;
        }
    }

};

int main() {
    RedBlackTree rbt;
    string input;

    while (true) { 
        cout << "Do you want to add, remove, print, search, read, or quit?" << endl;
        cin >> input;

        if (input == "add") {
            int num;
            cout << "Enter number to add: ";
            cin >> num;
            rbt.insert(num);
        } else if (input == "remove") {
            int num;
            cout << "Enter number to remove: ";
            cin >> num;
            rbt.remove(num);
        } else if (input == "print") {
            cout << "Inorder Traversal:" << endl;
            rbt.inorder();
            cout << endl;
        } else if (input == "search") {
            int num;
            cout << "Enter number to search: ";
            cin >> num;
            rbt.search(num);
        } else if (input == "read") {
            string filename;
            cout << "Enter filename: ";
            cin >> filename;
            rbt.readFromFile(filename);
        } else if (input == "quit") {
            break;
        }
    }

    return 0;
}