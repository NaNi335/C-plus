#include <iostream>
#include "list.h"

int main() {
    int_list_t list1;
    for (int i = 0; i < 10; ++i) {
        list1.push_front(i);
    }
    int_list_t list2;
    for (int i = 0; i < 10; ++i) {
        list2.push_front(i + 10);
    }
    std::cout << "List 1: " << list1 << std::endl;
    std::cout << "List 2: " << list2 << std::endl;

    int_list_t list_tmp = list1;

    list_tmp.reverse();

    std::cout << "Reverse: " << list_tmp << std::endl;

    std::cout << "Second element: " << list_tmp[2] << std::endl;

    list_tmp[2] = 40;

    std::cout << "Changed second element: "  << list_tmp[2] << std::endl;

    list_tmp.insert(4, 666);

    list_tmp.merge(list2);
    std::cout << "Merge: " << list_tmp << std::endl;

    list_tmp.splice(5, 4);
    std::cout << "Splice: " << list_tmp << std::endl;

    return 0;
}
