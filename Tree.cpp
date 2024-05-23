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

// Function to fix the tree after a deletion operation
    void fixDeletion(Node* node) {
        while (node != root && (node == NULL || node->color == BLACK)) {
            if (node == node->parent->left) {
                Node* sibling = node->parent->right;
                if (sibling->color == RED) {
                    sibling->color = BLACK;  // Case 1: Sibling is RED
                    node->parent->color = RED;  // Recolor sibling to BLACK and parent to RED
                    leftRotate(node->parent);  // Left rotate at parent
                    sibling = node->parent->right;  // Update sibling
                }
                if ((sibling->left == NULL || sibling->left->color == BLACK) &&
                    (sibling->right == NULL || sibling->right->color == BLACK)) {
                    sibling->color = RED;  // Case 2: Both children of sibling are BLACK
                    node = node->parent;  // Move node up to parent
                } else {
                    if (sibling->right == NULL || sibling->right->color == BLACK) {
                        if (sibling->left != NULL) sibling->left->color = BLACK;  // Case 3: Sibling's left child is RED
                        sibling->color = RED;  // Recolor sibling to RED
                        rightRotate(sibling);  // Right rotate at sibling
                        sibling = node->parent->right;  // Update sibling
                    }
                    sibling->color = node->parent->color;  // Case 4: Sibling's right child is RED
                    node->parent->color = BLACK;  // Recolor parent to BLACK
                    if (sibling->right != NULL) sibling->right->color = BLACK;  // Recolor sibling's right child to BLACK
                    leftRotate(node->parent);  // Left rotate at parent
                    node = root;  // Set node to root to terminate loop
                }
            } else {
                Node* sibling = node->parent->left;
                if (sibling->color == RED) {
                    sibling->color = BLACK;  // Case 1: Sibling is RED
                    node->parent->color = RED;  // Recolor sibling to BLACK and parent to RED
                    rightRotate(node->parent);  // Right rotate at parent
                    sibling = node->parent->left;  // Update sibling
                }
                if ((sibling->left == NULL || sibling->left->color == BLACK) &&
                    (sibling->right == NULL || sibling->right->color == BLACK)) {
                    sibling->color = RED;  // Case 2: Both children of sibling are BLACK
                    node = node->parent;  // Move node up to parent
                } else {
                    if (sibling->left == NULL || sibling->left->color == BLACK) {
                        if (sibling->right != NULL) sibling->right->color = BLACK;  // Case 3: Sibling's right child is RED
                        sibling->color = RED;  // Recolor sibling to RED
                        leftRotate(sibling);  // Left rotate at sibling
                        sibling = node->parent->left;  // Update sibling
                    }
                    sibling->color = node->parent->color;  // Case 4: Sibling's left child is RED
                    node->parent->color = BLACK;  // Recolor parent to BLACK
                    if (sibling->left != NULL) sibling->left->color = BLACK;  // Recolor sibling's left child to BLACK
                    rightRotate(node->parent);  // Right rotate at parent
                    node = root;  // Set node to root to terminate loop
                }
            }
        }
        if (node != NULL) node->color = BLACK;  // Ensure node is BLACK
    }

    // Helper function to perform inorder traversal of the tree
    void inorderHelper(Node* root, int level) {
        if (root != NULL) {
            inorderHelper(root->right, level + 1); // Traverse right subtree
            for (int i = 0; i < level; i++)        // Indentation based on tree level
                cout << "   ";
            cout << root->data;
            if (root->color == RED)
                cout << " (Red)" << endl;          // Indicate red nodes
            else
                cout << " (Black)" << endl;        // Indicate black nodes
            inorderHelper(root->left, level + 1);  // Traverse left subtree
        }
    }

public:
    // Constructor to initialize an empty Red-Black Tree
    RedBlackTree() : root(NULL) {}

    // Function to insert a new node into the tree
    void insert(int data) {
        Node* newNode = new Node(data);           // Create a new node
        Node* parent = NULL;
        Node* current = root;

        while (current != NULL) {              // Find the correct position to insert
            parent = current;
            if (newNode->data < current->data)
                current = current->left;
            else
                current = current->right;
        }
        newNode->parent = parent;                 // Set the parent of the new node
        if (parent == NULL)
            root = newNode;                       // New node is the root if tree was empty
        else if (newNode->data < parent->data)
            parent->left = newNode;               // Insert as left child
        else
            parent->right = newNode;              // Insert as right child
        newNode->color = RED;                     // New node must be RED
        fixInsertion(newNode);                    // Fix any violations caused by insertion
    }

    // Function to remove a node with the given data from the tree
    void remove(int data) {
        Node* targetNode = root;  // Initialize the node to be removed
        Node* replacementNode;  // Node that will replace the removed node
        Node* nodeToFix;  // Node that may need fixing to maintain tree properties
        Color originalColor;  // Store the original color of the replacement node

        // Traverse the tree to find the node with the given data
        while (targetNode != NULL && targetNode->data != data) {
            if (data < targetNode->data)
                targetNode = targetNode->left;  // Traverse left subtree
            else
                targetNode = targetNode->right;  // Traverse right subtree
        }

        // If the node to be deleted is not found, print an error message and return
        if (targetNode == NULL) {
            cout << "Couldn't find key in the tree\n";
            return;
        }

        replacementNode = targetNode;  // Start with the target node as the replacement
        originalColor = replacementNode->color;  // Save the original color of the replacement node

        // If the left child is null, replace the target node with its right child
        if (targetNode->left == NULL) {
            nodeToFix = targetNode->right;
            transplant(targetNode, targetNode->right);
        }
        // If the right child is null, replace the target node with its left child
        else if (targetNode->right == NULL) {
            nodeToFix = targetNode->left;
            transplant(targetNode, targetNode->left);
        }
        // If the target node has two children
        else {
            // Find the minimum value node in the right subtree
            replacementNode = minValueNode(targetNode->right);
            originalColor = replacementNode->color;  // Save the color of the replacement node
            nodeToFix = replacementNode->right;  // Right child of the replacement node

            // If the replacement node's parent is the target node
            if (replacementNode->parent == targetNode) {
                if (nodeToFix != NULL)
                    nodeToFix->parent = replacementNode;
            } else {
                // Replace the replacement node with its right child
                transplant(replacementNode, replacementNode->right);
                replacementNode->right = targetNode->right;  // Set the target's right child to replacement
                replacementNode->right->parent = replacementNode;
            }
            // Replace the target node with the replacement node
            transplant(targetNode, replacementNode);
            replacementNode->left = targetNode->left;  // Set the target's left child to replacement
            replacementNode->left->parent = replacementNode;
            replacementNode->color = targetNode->color;  // Set the replacement node's color to the target's color
        }

        // Fix the tree if the original color of the replacement node was BLACK
        if (originalColor == BLACK)
            fixDeletion(nodeToFix);

        delete targetNode;  // Delete the target node
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