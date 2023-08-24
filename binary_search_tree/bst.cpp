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

bst* search(bst* tree, unsigned int key) {
    if(!tree)
        return nullptr;

    if(tree->data.key == key)
        return tree;

    if(tree->data.key > key)
        return search(tree->left, key);

    return search(tree->right, key);
}

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

// @NEEDS WORK: i gotta rethink this whole pointer to a pointer business. gonna review the text book.
void del(bst** tree, unsigned int key, bst* parent = nullptr) {
    bst* node = search(*tree, key);

    if(!node) {
        std::cout << "Target node is not in tree." << std::endl;
    }
    else if (!node->left && !node->right) {
        delete node;
    }
}

// can pass it a void function that takes a bst* to call for each node
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

bst* min(bst* tree);
bst* max(bst* tree);
bst* pred(bst* tree);
bst* succ(bst* tree);

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

int main(int argc, char** argv) {
/*
    bst* tree = nullptr;

    unsigned int keys[10] = {4,8,1,43,12,7,0,16,22,9};

    for(unsigned int i = 0; i < 5; i++) {
        ins(&tree, {keys[i], 'X'});
    }

    std::cout << "In-order traversal" << std::endl;
    traverse(tree, &print_node, IN_ORDER);

    ins(&tree, {8, 'D'});

    print_node(search(tree, 8));
    print_node(search(tree, 7));
*/ 
    bst* leaf = nullptr;
    ins(&leaf, {10, 'H'});
    print_node(leaf);

    return 0;
}
