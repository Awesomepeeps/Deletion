// Used Algorithm Tutor (deletion) and Parents (insertion) 
// Nikaansh S.

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// Enum to define color for Red-Black Tree nodes
enum Color { RED, BLACK };

// Struct for node with values initialized at the beginning
struct node {
    int data;           // Data stored in the node
    node* left;         // Pointer to the left child
    node* right;        // Pointer to the right child
    node* parent;       // Pointer to the parent node
    Color color;        // Color of the node (RED or BLACK)
    node(int data) : data(data), left(nullptr), right(nullptr), parent(nullptr), color(BLACK) {} // Constructor to initialize node
};

// Class representing the Red-Black Tree
class RedBlackTree {
private:
    node* root;         // Pointer to the root of the tree

    // Function to perform a left rotation around the given node
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

    // Function to perform a right rotation around the given node
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

    // Function to fix the tree after an insertion operation
    void fixInsertion(node* z) {
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

    // Function to replace one subtree as a child of its parent with another subtree
    void transplant(node* current, node* comp) {
        if (current->parent == nullptr) {
            root = comp;
        } else if (current == current->parent->left) {
            current->parent->left = comp;
        } else {
            current->parent->right = comp;
        }
        comp->parent = current->parent;
    }

    // Function to find the node with the minimum value in a subtree
    node* minValueNode(node* minnode) {
        node* current = minnode;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }

    // Function to fix the tree after a deletion operation
    void fixDeletion(node* current) {
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

    // Helper function for inorder traversal
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
    // Constructor to initialize the Red-Black Tree
    RedBlackTree() : root(nullptr) {}

    // Function to insert a new node into the tree
    void insert(int data) {
        node* z = new node(data);
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
        fixInsertion(z); // Fix any violations caused by the insertion
    }

    // Function to remove a node with the given data from the tree
    void remove(int data) {
        node* z = root;
        node* current, * y;
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
            fixDeletion(current); // Fix any violations caused by the deletion
        delete z;
    }

    // Function to search for a node with the given data
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

    // Function to perform inorder traversal of the tree
    void inorder() {
        inorderHelper(root, 0);
    }

    // Function to read data from a file and insert into the tree
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

// Main function to interact with the Red-Black Tree
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
