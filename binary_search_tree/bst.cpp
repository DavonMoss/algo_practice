#include <iostream>

/*
 * KEY VALUE PAIR: Represent the data for each node.
 * */
struct key_value {
    unsigned int key;
    char value;
};

/*
 * BINARY SEARCH TREE: Represents a root node for a BST. Trying the struct + methods approach as opposed to classes this time.
 *
 * Making the assumption that we are not working with duplicate keys.
 *
 * */
struct bst {
    key_value data = {0, '\0'};
    bst* parent = nullptr;
    bst* left = nullptr;
    bst* right = nullptr;
    bst(key_value d) : data(d), left(nullptr), right(nullptr) {}
};

/*
 * ORDER SPECIFICATION: Must pass one of these enum values to traversal() to specify traversal order.
 * */
enum order {
    PRE_ORDER,
    IN_ORDER,
    POST_ORDER
};

/*
 * MINIMUM [O(h), log n <= h <= n | h is the height of the tree]
 *
 * Finds the node with the smallest key in the tree. Due to the constraints of
 * a BST, this is just a left only traversal to left-most leaf node.
 *
 * PARAMS:
 * - bst* tree: the head node of the tree to find the minimum key in.
 *
 * RETURNS:
 * A pointer to the node with the minimum key.
 * */
bst* min(bst* tree) {
    bst* curr = tree;

    while(curr->left) {
        curr = curr->left;
    }

    return curr;
}

/*
 * MAXIMUM [O(h), log n <= h <= n | h is the height of the tree]
 *
 * Finds the node with the biggest key in the tree. Due to the constraints of
 * a BST, this is just a right only traversal to right-most leaf node.
 *
 * PARAMS:
 * - bst* tree: the head node of the tree to find the maximum key in.
 *
 * RETURNS:
 * A pointer to the node with the maximum key.
 * */
bst* max(bst* tree) {
    bst* curr = tree;

    while(curr->right) {
        curr = curr->right;
    }

    return curr;
}

/*
 * PREDECESSOR [O(h), log n <= h <= n | h is the height of the tree]
 *
 * Gets the immediate predecessor in terms of key to the given node. Only searches the subtree
 * starting at the node given.
 *
 * PARAMS:
 * - bst* node: the node to find the predecessor of.
 *
 * RETURNS:
 * A pointer to the predecessor.
 * */
bst* pred(bst* node) {
    return max(node->left);
}

/*
 * SUCCESSOR [O(h), log n <= h <= n | h is the height of the tree]
 *
 * Gets the immediate successor in terms of key to the given node. Only searches the subtree
 * starting at the node given.
 *
 * PARAMS:
 * - bst* node: the node to find the successor of.
 *
 * RETURNS:
 * A pointer to the successor.
 * */
bst* succ(bst* tree) {
    return min(tree->right);
}

/*
 * SEARCH [O(log n)]
 *
 * Finds the node in the tree with the given key, if it exists.
 *
 * PARAMS:
 * - bst* tree: the tree to search for the key in
 * - unsigned int key: the key to search for
 *
 * RETURNS:
 * A pointer to the node found with matching key, nullptr if it doesn't find anything.
 * */
bst* search(bst* tree, unsigned int key) {
    if(!tree)
        return nullptr;

    if(tree->data.key == key)
        return tree;

    if(tree->data.key > key)
        return search(tree->left, key);

    return search(tree->right, key);
}

/*
 * INSERT [O(log n)]
 *
 * Inserts a piece of data, constructing a node for it and fitting it into the tree.
 *
 * PARAMS:
 * - bst** tree: the address of the variable which is holding the address of the tree (pointer to the pointer to the tree lol)
 *               this is needed so that we can update the actual pointers of the tree structure, basically when you call this
 *               just slap a & in front of the node i.e. ins(&node, ...) instead of ins(node, ...)
 * - key_value data: the data to insert
 * - bst* prev: a pointer to track the previous node we were at while traversing. needed to update parent of newly inserted node. 
 *              shouldn't need to manually put anything here.
 * */
void ins(bst** tree, key_value data, bst* prev = nullptr) {
    if(!*tree) {
        *tree = new bst(data);
	(*tree)->parent = prev;
    }
    else if(search(*tree, data.key)) {
        std::cout << "Key already exists, must be unique" << std::endl; // dont like this that much, just like undefined behavior less
    }
    else if((*tree)->data.key > data.key) {
        ins(&(*tree)->left, data, *tree);
    }
    else if((*tree)->data.key < data.key) {
        ins(&(*tree)->right, data, *tree);
    }
}

/*
 * DELETE [O(log n)]
 *
 * Removes the node with matched key from the tree.
 *
 * PARAMS:
 * - bst** tree: the address of the variable which is holding the address of the tree. tl;dr use del(&tree, ...) instead of del(tree, ...)
 * - unsigned int key: the key of the node to be removed
 * */
void del(bst** tree, unsigned int key) {
    bst* node = search(*tree, key);

    if(!node) {
        std::cout << "Target node is not in tree." << std::endl;
    }
    else if(!node->left && !node->right) {
    // CASE: NODE TO DELETE HAS NO KIDS, CLEAR THE POINTER AND FREE THE MEMORY
	if(node->parent) {
	    if(node->data.key > node->parent->data.key) {
	        node->parent->right = nullptr;
	    } else {
	        node->parent->left = nullptr;
	    }
	} else {
	    *tree = nullptr;
	}

	delete node;
    }
    else if(!node->left || !node->right) {
    // CASE: NODE TO DELETE HAS ONE CHILD, MAKE THE PARENT POINT TO SINGLE CHILD, THE CHILD POINT TO THE GRANDPARENT, AND FREE THE MEMORY
        bst* child = nullptr;
        if(node->left) {
	    child = node->left;
	} else {
	    child = node->right;
	}

	if(node->parent) {
	    if(node->data.key > node->parent->data.key) {
	        node->parent->right = child;
	    } else {
	        node->parent->left = child;
	    }
            child->parent = node->parent;
	} else {
	    *tree = child;
	    (*tree)->parent = nullptr;
	}

        delete node;
    }
    else if(node->left && node->right) {
    // CASE: NODE TO DELETE HAS TWO CHILDREN, OVERWRITE NODE DATA WITH SUCCESSOR DATA, RECURSE DELETE ON SUCCESSOR (will always be one of the other cases)
        bst* successor = succ(node);
	key_value temp = successor->data;
        del(tree, successor->data.key);
        node->data = temp;
    }
}

/*
 * TRAVERSE [O(n)]
 *
 * Traverses the tree in the order given (PRE_ORDER, IN_ORDER, or POST_ORDER), and applies a function at each node.
 *
 * PARAMS:
 * - bst* tree: the head node of the tree to traverse
 * - void (*func)(bst*): a function parameter/signature, for a function that takes a pointer to a bst as its own parameter and returns nothing (void).  
 * - order type: one of the orders specified in the order enum (PRE_ORDER, IN_ORDER, POST_ORDER).
 * */
void traverse(bst* tree, void (*func)(bst*), order type) {
    if(tree && type == PRE_ORDER) {
        func(tree);
        traverse(tree->left, func, type);
        traverse(tree->right, func, type);
    }
    else if(tree && type == IN_ORDER) {
        traverse(tree->left, func, type);
        func(tree);
        traverse(tree->right, func, type);
    }
    else if(tree && type == POST_ORDER) {
        traverse(tree->left, func, type);
        traverse(tree->right, func, type);
        func(tree);
    }
}

/*
 * PRINT HELPERS FOR TESTING
 * */
void print_node(bst* node){
    if(!node){
        std::cout << "no such thing" << std::endl;
    } else {
        std::cout << "[Key: " << node->data.key;
        std::cout << ", Value: " << node->data.value;
        std::cout << ", Addr: " << node;
        std::cout << ", Parent Addr: " << node->parent;
        std::cout << ", Left Addr: " << node->left;
        std::cout << ", Right Addr: " << node->right << "]" << std::endl;
    }
}

/*
 * MAIN
 * */
int main(int argc, char** argv) {
    bst* tree = nullptr;

    unsigned int keys[10] = {4,8,1,43,12,7,0,16,22,9};

    for(unsigned int i = 0; i < 6; i++) {
        ins(&tree, {keys[i], 'X'});
    }

    bst* small_tree = nullptr;

    if(argc < 2) {
        std::cout << "not enough args, pick a case [1-6]" << std::endl;
        return -1;
    }

    switch(*argv[1]) {
        case '1':
	    // LEAF CASE
            std::cout << "BEFORE:" << '\n';
            traverse(tree, &print_node, IN_ORDER);
	    del(&tree, 1);
            std::cout << "AFTER:" << '\n';
            traverse(tree, &print_node, IN_ORDER);
	    break;
	case '2':
	    // LEAF ROOT CASE
	    ins(&small_tree, {12, 'd'});

            std::cout << "BEFORE:" << '\n';
            traverse(small_tree, &print_node, IN_ORDER);
	    del(&small_tree, 12);
            std::cout << "AFTER:" << '\n';
            traverse(small_tree, &print_node, IN_ORDER);
	    break;
	case '3':
	    // ONE CHILD CASE
            std::cout << "BEFORE:" << '\n';
            traverse(tree, &print_node, IN_ORDER);
	    del(&tree, 43);
            std::cout << "AFTER:" << '\n';
            traverse(tree, &print_node, IN_ORDER);
	    break;
	case '4':
	    // ONE CHILD ROOT CASE
	    ins(&small_tree, {12, 'd'});
	    ins(&small_tree, {13, 'f'});

            std::cout << "BEFORE:" << '\n';
            traverse(small_tree, &print_node, IN_ORDER);
	    del(&small_tree, 12);
            std::cout << "AFTER:" << '\n';
            traverse(small_tree, &print_node, IN_ORDER);
	    break;
	case '5':
	    // TWO CHILD CASE
            std::cout << "BEFORE:" << '\n';
            traverse(tree, &print_node, IN_ORDER);
	    del(&tree, 8);
            std::cout << "AFTER:" << '\n';
            traverse(tree, &print_node, IN_ORDER);
	    break;
	case '6':
	    // TWO CHILD ROOT CASE
	    ins(&small_tree, {12, 'd'});
	    ins(&small_tree, {13, 'f'});
	    ins(&small_tree, {11, 'a'});

            std::cout << "BEFORE:" << '\n';
            traverse(small_tree, &print_node, IN_ORDER);
	    del(&small_tree, 12);
            std::cout << "AFTER:" << '\n';
            traverse(small_tree, &print_node, IN_ORDER);
	    break;
	default:
	    std::cout << "No argument passed" << std::endl;
	    break;
    }

    return 0;
}
