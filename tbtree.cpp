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
    Node* minValueNode(Node* current) {
        while (current->left != NULL) {  // Traverse to the leftmost node
            current = current->left;
        }
        return current;
    }

    // Function to fix the tree after a deletion operation
    /*void fixDeletion(Node* helper) {
        Node* sibling;
        while (helper != root && helper->color == BLACK) {
            if (helper == helper->parent->left) {
                cout << "Helper is a left child..." << endl;
                sibling = helper->parent->right;
                if (sibling->color == RED) {
                    cout << "Sibling is RED..." << endl;
                    sibling->color = BLACK;
                    helper->parent->color = RED;
                    leftRotate(helper->parent);
                    sibling = helper->parent->right;
                    cout << "Performed left rotation at parent..." << endl;
                }
                if ((sibling->left == NULL || sibling->left->color == BLACK) &&
                    (sibling->right == NULL || sibling->right->color == BLACK)) {
                    cout << "Both children of sibling are BLACK..." << endl;
                    sibling->color = RED;
                    helper = helper->parent;
                } else {
                    if (sibling->right == NULL || sibling->right->color == BLACK) {
                        cout << "Right child of sibling is BLACK..." << endl;
                        if (sibling->left != NULL) {
                            sibling->left->color = BLACK;
                        }
                        sibling->color = RED;
                        rightRotate(sibling);
                        sibling = helper->parent->right;
                        cout << "Performed right rotation at sibling..." << endl;
                    }
                    sibling->color = helper->parent->color;
                    helper->parent->color = BLACK;
                    if (sibling->right != NULL) {
                        sibling->right->color = BLACK;
                    }
                    leftRotate(helper->parent);
                    helper = root;
                    cout << "Performed left rotation at parent..." << endl;
                }
            } else {
                cout << "Helper is a right child..." << endl;
                sibling = helper->parent->left;
                if (sibling->color == RED) {
                    cout << "Sibling is RED..." << endl;
                    sibling->color = BLACK;
                    helper->parent->color = RED;
                    rightRotate(helper->parent);
                    sibling = helper->parent->left;
                    cout << "Performed right rotation at parent..." << endl;
                }
                if ((sibling->right == NULL || sibling->right->color == BLACK) &&
                    (sibling->left == NULL || sibling->left->color == BLACK)) {
                    cout << "Both children of sibling are BLACK..." << endl;
                    sibling->color = RED;
                    helper = helper->parent;
                } else {
                    if (sibling->left == NULL || sibling->left->color == BLACK) {
                        cout << "Left child of sibling is BLACK..." << endl;
                        if (sibling->right != NULL) {
                            sibling->right->color = BLACK;
                        }
                        sibling->color = RED;
                        leftRotate(sibling);
                        sibling = helper->parent->left;
                        cout << "Performed left rotation at sibling..." << endl;
                    }
                    sibling->color = helper->parent->color;
                    helper->parent->color = BLACK;
                    if (sibling->left != NULL) {
                        sibling->left->color = BLACK;
                    }
                    rightRotate(helper->parent);
                    helper = root;
                    cout << "Performed right rotation at parent..." << endl;
                }
            }
        }
        if (helper != NULL) {
            helper->color = BLACK;
            cout << "Helper is colored BLACK..." << endl;
        }
    }*/

    void fixDeletion(Node* x) {
		Node* s;
		while (x != root && x->color == BLACK) {
			if (x == x->parent->left) {
				s = x->parent->right;
				if (s->color == RED) {
					// case 3.1
					s->color = BLACK;
					x->parent->color = RED;
					leftRotate(x->parent);
					s = x->parent->right;
				}

				if (s->left->color == BLACK && s->right->color == BLACK) {
					// case 3.2
					s->color = RED;
					x = x->parent;
				} else {
					if (s->right->color == BLACK) {
						// case 3.3
						s->left->color = BLACK;
						s->color = RED;
						rightRotate(s);
						s = x->parent->right;
					} 

					// case 3.4
					s->color = x->parent->color;
					x->parent->color = BLACK;
					s->right->color = BLACK;
					leftRotate(x->parent);
					x = root;
				}
			} else {
				s = x->parent->left;
				if (s->color == RED) {
					// case 3.1
					s->color = BLACK;
					x->parent->color = RED;
					rightRotate(x->parent);
					s = x->parent->left;
				}

				if (s->right->color == BLACK && s->right->color == BLACK) {
					// case 3.2
					s->color = RED;
					x = x->parent;
				} else {
					if (s->left->color == BLACK) {
						// case 3.3
						s->right->color = BLACK;
						s->color = RED;
						leftRotate(s);
						s = x->parent->left;
					} 

					// case 3.4
					s->color = x->parent->color;
					x->parent->color = BLACK;
					s->left->color = BLACK;
					rightRotate(x->parent);
					x = root;
				}
			} 
		}
		x->color = BLACK;
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
/*void remove(int data) {
    Node* nodert = root;        // Temporary pointer for traversing the tree
    Node* nodeToDelete = NULL;  // Pointer to the node to be deleted
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
    
    if (nodeToDelete == NULL) { // Check if the node to delete is found
        cout << "No such node exists." << endl;
        return;
    }
    
    // Replace the node to delete with its successor if necessary
    helper = nodeToDelete;      // Initialize helper node with the node to delete
    Color originalColor = helper->color; // Store the original color of the helper node
    cout << "Original color of helper: " << (originalColor == RED ? "RED" : "BLACK") << endl;
    if (nodeToDelete->left == NULL) {
        cout << "Checking if left child is NULL..." << endl;
        helperChild = nodeToDelete->right; // If the left child is NULL, assign the right child to helperChild
        transplant(nodeToDelete, nodeToDelete->right); // Replace nodeToDelete with its right child
    } else if (nodeToDelete->right == NULL) {
        cout << "Checking if right child is NULL..." << endl;
        helperChild = nodeToDelete->left;  // If the right child is NULL, assign the left child to helperChild
        transplant(nodeToDelete, nodeToDelete->left); // Replace nodeToDelete with its left child
    } else {
        cout << "Neither left nor right child is NULL..." << endl;
        helper = minValueNode(nodeToDelete->right); // Find the minimum value node in the right subtree
        cout << "Helper data: " << helper->data << endl;
        originalColor = helper->color; // Update the original color with the color of the successor node
        cout << "Original color of helper: " << (originalColor == RED ? "RED" : "BLACK") << endl;
        helperChild = helper->right;   // Assign the right child of the helper node to helperChild
        if (helper->parent == nodeToDelete) {
            cout << "Helper's parent is the node to delete..." << endl;
            helperChild->parent = helper; // Update the parent of the helperChild
        } else {
            cout << "Helper's parent is not the node to delete..." << endl;
            transplant(helper, helper->right); // Replace helper with its right child
            helper->right = nodeToDelete->right; // Update the right child of the helper node
            helper->right->parent = helper; // Update the parent of the right child
        }
        cout << "Bfr Transplant" << endl;
        transplant(nodeToDelete, helper); // Replace nodeToDelete with the helper node
        helper->left = nodeToDelete->left; // Update the left child of the helper node
        helper->left->parent = helper; // Update the parent of the left child
        helper->color = nodeToDelete->color; // Update the color of the helper node
    }
    delete nodeToDelete; // Delete the nodeToDelete
    cout << "After node is deleted" << endl;
    if (originalColor == BLACK) {
        cout << "Fixing delete" << endl;
        fixDeletion(helperChild); // Fix any violations of Red-Black Tree properties
    }
}
*/

void remove(Node* noderm, int key) {
		// find the node containing key
		Node*  z = NULL;
		Node*  x;
        Node* y;
		while (noderm != NULL){
			if (noderm->data == key) {
				z = noderm;
			}

			if (noderm->data <= key) {
				noderm = noderm->right;
			} else {
				noderm = noderm->left;
			}
		}

		if (z == NULL) {
			cout<<"Couldn't find key in the tree"<<endl;
			return;
		} 

		y = z;
		int y_original_color = y->color;
		if (z->left == NULL) {
			x = z->right;
			transplant(z, z->right);
		} else if (z->right == NULL) {
			x = z->left;
			transplant(z, z->left);
		} else {
			y = minValueNode(z->right);
			y_original_color = y->color;
			x = y->right;
			if (y->parent == z) {
				x->parent = y;
			} else {
				transplant(y, y->right);
				y->right = z->right;
				y->right->parent = y;
			}

			transplant(z, y);
			y->left = z->left;
			y->left->parent = y;
			y->color = z->color;
		}
		delete z;
		if (y_original_color == 0){
			fixDeletion(x);
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

    Node* getroot() {
        return root;
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
            rbt.remove(rbt.getroot(), num);
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