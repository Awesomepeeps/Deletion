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
            cout << root->data;
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
        Node* node = root;
        Node* nodeToDelete = NULL;
        Node* helper;
        Node* helperChild;
        
        // Find the node to delete
        while (node != NULL) {
            if (node->data == data) {
                nodeToDelete = node;
            }
            if (node->data <= data) {
                node = node->right;
            } else {
                node = node->left;
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
                if (helperChild != NULL) {
                    helperChild->parent = helper;
                }
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
        
        if (originalColor == BLACK) {
            fixDeletion(helperChild);
        }
    }

    // Function to perform an inorder traversal of the tree
    void inorderTraversal() {
        inorderHelper(root, 0);
    }

    // Function to print the tree structure for debugging
    void debug() {
        debugHelper(root);
    }
};

// Main function to test the Red-Black Tree implementation
int main() {
    RedBlackTree tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(15);
    tree.insert(25);

    cout << "Tree after insertion:" << endl;
    tree.inorderTraversal();
    
    tree.remove(20);
    tree.remove(10);
    //tree.remove(15);
    cout << "Tree after deletion:" << endl;
    tree.inorderTraversal();
    
    return 0;
}