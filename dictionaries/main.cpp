#include <iostream>

struct key_value {
    char key;
    int value;
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

int main(int argc, char** argv) {
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

    return 0;
}
