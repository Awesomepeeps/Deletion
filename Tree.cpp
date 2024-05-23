// Used Algorithm Tutor (deletion) and Parents (insertion) 
// Nikaansh S.

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// Enum to define color for Red-Black Tree nodes
enum Color { RED, BLACK };

// Struct for node with values initialized at the beginning
struct Node {
    int data;           // Data stored in the node
    Node* left;         // Pointer to the left child
    Node* right;        // Pointer to the right child
    Node* parent;       // Pointer to the parent node
    Color color;        // Color of the node (RED or BLACK)
    Node(int data) : data(data), left(NULL), right(NULL), parent(NULL), color(BLACK) {} // Constructor to initialize node
};

// Class representing the Red-Black Tree
class RedBlackTree {
private:
    Node* root;         // Pointer to the root of the tree

    // Function to perform a left rotation around the given node
    void leftRotate(Node* current) {
        Node* rightChild = current->right;        // Store the right child
        current->right = rightChild->left;        // Move left subtree of right child to the right subtree of current node
        if (rightChild->left != NULL)
            rightChild->left->parent = current;   // Update parent pointer of moved subtree
        rightChild->parent = current->parent;     // Update parent pointer of right child
        if (current->parent == NULL)
            root = rightChild;                    // Update root if necessary
        else if (current == current->parent->left)
            current->parent->left = rightChild;   // Update left child of parent
        else
            current->parent->right = rightChild;  // Update right child of parent
        rightChild->left = current;               // Move current node to left of right child
        current->parent = rightChild;             // Update parent pointer of current node
    }

    // Function to perform a right rotation around the given node
    void rightRotate(Node* current) {
        Node* leftChild = current->left;          // Store the left child
        current->left = leftChild->right;         // Move right subtree of left child to the left subtree of current node
        if (leftChild->right != NULL)
            leftChild->right->parent = current;   // Update parent pointer of moved subtree
        leftChild->parent = current->parent;      // Update parent pointer of left child
        if (current->parent == NULL)
            root = leftChild;                     // Update root if necessary
        else if (current == current->parent->right)
            current->parent->right = leftChild;   // Update right child of parent
        else
            current->parent->left = leftChild;    // Update left child of parent
        leftChild->right = current;               // Move current node to right of left child
        current->parent = leftChild;              // Update parent pointer of current node
    }

    // Function to fix the tree after an insertion operation
    void fixInsertion(Node* node) {
        while (node != root && node->parent->color == RED) {
            if (node->parent == node->parent->parent->left) {
                Node* uncle = node->parent->parent->right;
                if (uncle != NULL && uncle->color == RED) {
                    node->parent->color = BLACK;     // Case 1: Uncle is RED
                    uncle->color = BLACK;            // Recolor parent and uncle to BLACK
                    node->parent->parent->color = RED; // Recolor grandparent to RED
                    node = node->parent->parent;     // Move node up to grandparent
                } else {
                    if (node == node->parent->right) {
                        node = node->parent;         // Case 2: Node is right child
                        leftRotate(node);            // Left rotate at parent
                    }
                    node->parent->color = BLACK;     // Case 3: Node is left child
                    node->parent->parent->color = RED; // Recolor parent to BLACK and grandparent to RED
                    rightRotate(node->parent->parent); // Right rotate at grandparent
                }
            } else {
                Node* uncle = node->parent->parent->left;
                if (uncle != NULL && uncle->color == RED) {
                    node->parent->color = BLACK;     // Case 1: Uncle is RED
                    uncle->color = BLACK;            // Recolor parent and uncle to BLACK
                    node->parent->parent->color = RED; // Recolor grandparent to RED
                    node = node->parent->parent;     // Move node up to grandparent
                } else {
                    if (node == node->parent->left) {
                        node = node->parent;         // Case 2: Node is left child
                        rightRotate(node);           // Right rotate at parent
                    }
                    node->parent->color = BLACK;     // Case 3: Node is right child
                    node->parent->parent->color = RED; // Recolor parent to BLACK and grandparent to RED
                    leftRotate(node->parent->parent); // Left rotate at grandparent
                }
            }
        }
        root->color = BLACK;                         // Ensure root is always BLACK
    }

    // Function to replace one subtree as a child of its parent with another subtree
    void transplant(Node* target, Node* with) {
        if (target->parent == NULL) {
            root = with;  // Update root if necessary
        } else if (target == target->parent->left) {
            target->parent->left = with;  // Update left child of parent
        } else {
            target->parent->right = with;  // Update right child of parent
        }
        if (with != NULL) {
            with->parent = target->parent;  // Update parent pointer of new subtree
        }
    }

    // Function to find the node with the minimum value in a subtree
    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current->left != NULL) {  // Traverse to the leftmost node
            current = current->left;
        }
        return current;
    }

    void fixDeletion(Node* helper) {
        Node* sibling;
        while (helper != root && (helper == NULL || helper->color == BLACK)) {
            if (helper == helper->parent->left) {
                sibling = helper->parent->right;
                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    helper->parent->color = RED;
                    leftRotate(helper->parent);
                    sibling = helper->parent->right;
                }
                if ((sibling->left == NULL || sibling->left->color == BLACK) &&
                    (sibling->right == NULL || sibling->right->color == BLACK)) {
                    sibling->color = RED;
                    helper = helper->parent;
                } else {
                    if (sibling->right == NULL || sibling->right->color == BLACK) {
                        if (sibling->left != NULL) {
                            sibling->left->color = BLACK;
                        }
                        sibling->color = RED;
                        rightRotate(sibling);
                        sibling = helper->parent->right;
                    }
                    sibling->color = helper->parent->color;
                    helper->parent->color = BLACK;
                    if (sibling->right != NULL) {
                        sibling->right->color = BLACK;
                    }
                    leftRotate(helper->parent);
                    helper = root;
                }
            } else {
                sibling = helper->parent->left;
                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    helper->parent->color = RED;
                    rightRotate(helper->parent);
                    sibling = helper->parent->left;
                }
                if ((sibling->right == NULL || sibling->right->color == BLACK) &&
                    (sibling->left == NULL || sibling->left->color == BLACK)) {
                    sibling->color = RED;
                    helper = helper->parent;
                } else {
                    if (sibling->left == NULL || sibling->left->color == BLACK) {
                        if (sibling->right != NULL) {
                            sibling->right->color = BLACK;
                        }
                        sibling->color = RED;
                        leftRotate(sibling);
                        sibling = helper->parent->left;
                    }
                    sibling->color = helper->parent->color;
                    helper->parent->color = BLACK;
                    if (sibling->left != NULL) {
                        sibling->left->color = BLACK;
                    }
                    rightRotate(helper->parent);
                    helper = root;
                }
            }
        }
        if (helper != NULL) {
            helper->color = BLACK;
        }
    }

    // Helper function to perform inorder traversal of the tree
    void inorderHelper(Node* root, int level) {
        if (root != NULL) {
            inorderHelper(root->right, level + 1); // Traverse right subtree
            for (int i = 0; i < level; i++)        // Indentation based on tree level
                cout << "   ";
            cout << "Node: " << root->data;
            if (root->parent != NULL)
                cout << " (Parent: " << root->parent->data << ")";
            else
                cout << " (Parent: NULL)";
            if (root->left != NULL)
                cout << " (Left: " << root->left->data << ")";
            else
                cout << " (Left: NULL)";
            if (root->right != NULL)
                cout << " (Right: " << root->right->data << ")";
            else
                cout << " (Right: NULL)";
            if (root->color == RED)
                cout << " (Red)" << endl;          // Indicate red nodes
            else
                cout << " (Black)" << endl;        // Indicate black nodes
            inorderHelper(root->left, level + 1);  // Traverse left subtree
        }
    }


    // Helper function to print the tree structure for debugging
    void debugHelper(Node* root) {
        if (root != NULL) {
            cout << "Node: " << root->data << " Color: " << (root->color == RED ? "Red" : "Black");
            if (root->parent != NULL)
                cout << " Parent: " << root->parent->data;
            else
                cout << " Parent: NULL";
            if (root->left != NULL)
                cout << " Left: " << root->left->data;
            else
                cout << " Left: NULL";
            if (root->right != NULL)
                cout << " Right: " << root->right->data;
            else
                cout << " Right: NULL";
            cout << endl;
            debugHelper(root->left);
            debugHelper(root->right);
        }
    }

public:
    // Constructor to initialize the tree with a NULL root
    RedBlackTree() : root(NULL) {}

    // Function to insert a new value into the tree
    void insert(int data) {
        Node* newNode = new Node(data);  // Create a new node with the given data
        newNode->color = RED;            // New nodes are always red
        if (root == NULL) {
            root = newNode;              // Insert as root if tree is empty
            root->color = BLACK;         // Root is always black
            return;
        }

        Node* current = root;            // Start at the root
        Node* parent = NULL;
        while (current != NULL) {
            parent = current;
            if (data < current->data)
                current = current->left; // Move to the left child
            else
                current = current->right; // Move to the right child
        }

        newNode->parent = parent;        // Set parent of the new node
        if (data < parent->data)
            parent->left = newNode;      // Set as left child
        else
            parent->right = newNode;     // Set as right child

        fixInsertion(newNode);           // Fix any violations of the Red-Black Tree properties
    }

    // Function to remove a node with the given data from the tree
    void remove(int data) {
        Node* nodert = root;
        Node* nodeToDelete = NULL;
        Node* helper;
        Node* helperChild;
        
        // Find the node to delete
        while (nodert != NULL) {
            if (nodert->data == data) {
                nodeToDelete = nodert;
            }
            if (nodert->data <= data) {
                nodert = nodert->right;
            } else {
                nodert = nodert->left;
            }
        }
        
        if (nodeToDelete == NULL) {
            cout << "No such node exists." << endl;
            return;
        }
        
        // Replace the node to delete with its successor if necessary
        helper = nodeToDelete;
        Color originalColor = helper->color;
        if (nodeToDelete->left == NULL) {
            helperChild = nodeToDelete->right;
            transplant(nodeToDelete, nodeToDelete->right);
        } else if (nodeToDelete->right == NULL) {
            helperChild = nodeToDelete->left;
            transplant(nodeToDelete, nodeToDelete->left);
        } else {
            helper = minValueNode(nodeToDelete->right);
            originalColor = helper->color;
            helperChild = helper->right;
            if (helper->parent == nodeToDelete) {
                    helperChild->parent = helper;
            } else {
                transplant(helper, helper->right);
                helper->right = nodeToDelete->right;
                helper->right->parent = helper;
            }
            transplant(nodeToDelete, helper);
            helper->left = nodeToDelete->left;
            helper->left->parent = helper;
            helper->color = nodeToDelete->color;
        }
        delete nodeToDelete;
        if (originalColor == BLACK) {
            fixDeletion(helperChild);
        }
    }

    // Function to search for a node with the given data in the tree
    void search(int data) {
        Node* current = root;
        while (current != NULL) {
            if (data == current->data) {
                cout << "Found: " << data << endl;
                return;
            } else if (data < current->data)
                current = current->left;
            else
                current = current->right;
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