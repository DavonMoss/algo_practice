#include <iostream>

struct key_value {
    char key;
    int value;
};

struct sll_node {
    key_value data;
    sll_node* next;
    sll_node() : data({'\0', 0}), next(nullptr) {}
    sll_node(key_value d) : data(d), next(nullptr) {}
    sll_node(key_value d, sll_node* n) : data(d), next(n) {}
};

class arr_dict {
private:
    static const int STATIC_ARRAY_SIZE = 12;
    key_value data[STATIC_ARRAY_SIZE] = {'\0', 0};
    int next_free_space = 0;    
public:
    /*
     * HELPERS FOR TESTING STUFF.
     * */
    void print_index_address(int i) {
        std::cout << "Address of arr_dict.data at " << i << " is " << &data[i] << "." << std::endl;
    }
    void print_item(key_value* item) {
	std::cout << "[Index: " << (int)(item - &data[0]);
	std::cout << ", Key: " << item->key;
	std::cout << ", Value: " << item->value;
	std::cout << ", Address: " << item << "]" << std::endl;
    }

    /*
     * SEARCH [unsorted: O(n)]
     *
     * Params:
     * char key - the key to return the value for
     *
     * Returns:
     * pointer to key-value struct in array
     * */
    key_value* search(char key) {
        for(int i = 0; i < STATIC_ARRAY_SIZE; i++) {
            if(data[i].key == key) {
	        std::cout << "Found entry: " << std::endl;
		print_item(&data[i]);
		return &data[i];
	    }	
	}
      
	std::cout << "Element not found :(" << std::endl;
	return nullptr;
    }

    /*
     * INSERT [unsorted: O(1)]
     *
     * Params:
     * key_value item - the item (key-value pair) to insert into the dictionary
     *
     * Returns:
     * Nothing.
     * */
    void ins(key_value item) {
        if(next_free_space >= STATIC_ARRAY_SIZE) {
	    std::cout << "Can't add that, array full" << std::endl;
	} else {
            data[next_free_space] = item;
            next_free_space++;	    
	} 
    }

    /*
     * DELETE [unsorted: O(1)]
     *
     * Decrements next_free_space pointer, overwrites item with the last element in the array, and then zeroes out the last element.
     *
     * Params:
     * key_value* item - pointer to the item you want to delete. you can get this pointer using search().
     *
     * Returns:
     * Nothing.
     * */
    void del(key_value* item) {
	if(next_free_space > 0) {
	    next_free_space--;
            *item = data[next_free_space];
	    data[next_free_space] = {'\0', 0};
	} else {
	    std::cout << "We already empty bruh" << std::endl;
	}
    }

    /*
     * MAX [unsorted: O(n)]
     *
     * Returns a pointer to the item with the maximum key in the dictionary. NOT value, KEY!
     *
     * Returns:
     * Returns pointer to item with maximum key in dictionary.
     * */
    key_value* max() {
	key_value* max_item = &data[0];

        for(int i = 0; i < STATIC_ARRAY_SIZE; i++) {
	    if(data[i].key > max_item->key) {
	        max_item = &data[i];
	    } 
	}

	return max_item;
    }

    /*
     * MIN [unsorted: O(n)]
     *
     * Returns a pointer to the item with the minimum key in the dictionary. NOT value, KEY!
     *
     * Returns:
     * Returns pointer to item with minimum key in dictionary.
     * */
    key_value* min() {
	key_value* min_item = &data[0];

        for(int i = 0; i < STATIC_ARRAY_SIZE; i++) {
	    if(data[i].key <  min_item->key) {
	        min_item = &data[i];
	    } 
	}

	return min_item;
    }
    
    /*
     * PREDECESSOR [unsorted: O(n)]
     *
     * Returns a pointer to the item with the key that is immediately less than the key of a given item.
     *
     * Params:
     * key_value* item - the item to find the key predecessor of
     *
     * Returns:
     * Pointer to item with predecessor by key.
     * */
    key_value* pred(key_value* item) {
        key_value* bottom = min();

	for(int i = 0; i < STATIC_ARRAY_SIZE; i++) {
	    if(data[i].key > bottom->key && data[i].key < item->key) {
	        bottom = &data[i];
	    }
	}

	return bottom;
    }

    /*
     * SUCCESSOR [unsorted: O(n)]
     *
     * Returns a pointer to the item with the key that is immediately greater than the key of a given item.
     *
     * Params:
     * key_value* item - the item to find the key successor of
     *
     * Returns:
     * Pointer to item with successor by key.
     * */
    key_value* succ(key_value* item) {
        key_value* top = max();

	for(int i = 0; i < STATIC_ARRAY_SIZE; i++) {
	    if(data[i].key < top->key && data[i].key > item->key) {
	        top = &data[i];
	    }
	}

	return top;
    }

    /*
     * PRINTS CONTENTS OF EACH KEY-VALUE ELEMENT IN DICTIONARY.
     * */
    void print() {
        for(int i = 0; i < STATIC_ARRAY_SIZE; i++) {
	    print_item(&data[i]);
	}
    }
};

class sll_dict {
private:
    sll_node* head = nullptr;
    sll_node* max_node = nullptr;
    sll_node* min_node = nullptr;
public:

    sll_node* search(char key) {
        sll_node* curr = head;
	while(curr) {
	    if(curr->data.key == key) {
	        return curr;
	    }
	    curr = curr->next;
	}

	return nullptr;
    }

    void ins(key_value item) {
        sll_node* node = new sll_node(item, head);
	head = node;

	//for min and max tracking
	if(!max_node && !min_node) {
	    max_node = node;
	    min_node = node;
	} else {
	    if(node->data.key > max_node->data.key)
	        max_node = node;
	    if(node->data.key < min_node->data.key)
	        min_node = node;
	}
    }

    void del(sll_node* node) {
	if(!head) {
	    std::cout << "List empty" << std::endl;
	}
        else if (head == node) {
            if(!head->next) {
		delete head;
		head = nullptr;
	    }
	        
	    sll_node* removed_node = head;
	    head = head->next;
            delete removed_node;
	} 
	else {
	    bool found = false;
            sll_node* curr = head;

            //removing the node
	    while(curr && curr->next) {
	        if(curr->next == node) {
		    //unlink node and save heap pointer to the data
		    found = true;
		    sll_node* removed_node = curr->next;
	            curr->next = curr->next->next;

                    //free the heap data
		    delete removed_node;
		    break;
	        } else {
	            curr = curr->next;
		}
	    }

	    if (!found)
                std::cout << "Node not in list, can't delete" << std::endl;
	}

	//additional linear pass to update min/max. allows us to keep min()/max() constant
	sll_node* curr = head;
	max_node = curr;
	min_node = curr;
	while (curr) {
	    if(curr->data.key > max_node->data.key)
	        max_node = curr;
	    if(curr->data.key < min_node->data.key)
		min_node = curr;
	    curr = curr->next;	
	}
    }

    sll_node* max() {
        return max_node;
    }

    sll_node* min() {
        return min_node;
    }

    sll_node* pred(sll_node* node) {
	sll_node* curr = head;
	sll_node* bottom = min();

        while(curr) {
	    if(curr->data.key < node->data.key && curr->data.key > bottom->data.key)
                bottom = curr;
	    curr = curr->next;
	}

	return bottom;
    }

    sll_node* succ(sll_node* node) {
	sll_node* curr = head;
	sll_node* top = max();

        while(curr) {
	    if(curr->data.key > node->data.key && curr->data.key < top->data.key)
                top = curr;
	    curr = curr->next;
	}

	return top;
    }

    static void print_item(sll_node* node) {
	if(!node) {
	    std::cout << "Node doesn't exist." << std::endl;
	} else {
	    std::cout << "[Node address: " << node;
    	    std::cout << ", Next address: " << node->next;
            std::cout << ", Node key: " << node->data.key;
	    std::cout << ", Node value: " << node->data.value << "]" << std::endl;
	}
    }

    void print() {
        sll_node* curr = head;
	std::cout << "HEAD ==> ";
	while(curr) {
	    print_item(curr);
	    curr = curr->next;
	}
    }
};

void arr_dict_testing() {
    arr_dict* ad = new arr_dict();

    char key = 'a';
    for(int i = 0; i < 15; i++) {
        ad->ins({(char)(key+i), i*5});
    }

    std::cout << "initial" << std::endl;
    ad->print();

    ad->del(ad->search('i'));
    ad->del(ad->search('d'));
    ad->del(ad->search('a'));

    std::cout << "3 deletions" << std::endl;
    ad->print();

    std::cout << "max" << std::endl;
    ad->print_item(ad->max());

    std::cout << "min" << std::endl;
    ad->print_item(ad->min());

    std::cout << "pred" << std::endl;
    ad->print_item(ad->pred(ad->search('j')));

    std::cout << "succ" << std::endl;
    ad->print_item(ad->succ(ad->search('j')));
}

void sll_dict_testing() {
    sll_dict* sd = new sll_dict();

    char key = 'a';
    for(int i = 0; i < 5; i++) {
        sd->ins({(char)(key+i), i*5});
    }

    std::cout << "deleting b" << std::endl;
    sd->del(sd->search('b'));
    std::cout << "deleting x" << std::endl;
    sd->del(sd->search('x'));

    std::cout << "print it all" << std::endl;
    sd->print();

    std::cout << "finding c" << std::endl;
    sd->print_item(sd->search('c'));

    std::cout << "finding k" << std::endl;
    sd->print_item(sd->search('k'));

    std::cout << "1st min" << std::endl;
    sd->print_item(sd->min());
    std::cout << "1st max" << std::endl;
    sd->print_item(sd->max());

    std::cout << "deleting a" << std::endl;
    sd->del(sd->search('a'));
    std::cout << "deleting e" << std::endl;
    sd->del(sd->search('e'));

    std::cout << "print it all again" << std::endl;
    sd->print();

    std::cout << "2nd min" << std::endl;
    sd->print_item(sd->min());
    std::cout << "2nd max" << std::endl;
    sd->print_item(sd->max());

    char keys[5] = {'r', 'x', 'S', '>', 'f'};
    for(int i = 0; i < 5; i++) {
        sd->ins({keys[i], i*7});
    }

    std::cout << "print it all again, and again" << std::endl;
    sd->print();

    std::cout << "predecessor of \'c\'" << std::endl;
    sd->print_item(sd->pred(sd->search('c')));
    std::cout << "predecessor of \'r\'" << std::endl;
    sd->print_item(sd->pred(sd->search('r')));
    std::cout << "successor of \'c\'" << std::endl;
    sd->print_item(sd->succ(sd->search('c')));
    std::cout << "successor of \'r\'" << std::endl;
    sd->print_item(sd->succ(sd->search('r')));
}

int main(int argc, char** argv) {

    //arr_dict_testing();
    sll_dict_testing();

    return 0;
}
