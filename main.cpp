#include "laba5.h"
#include <iostream>

void demonstrate_int_array() {
    std::cout << "=== int array demo ===" << std::endl;
    
    FixedBlockMemoryResource mr(1024 * 1024);
    DynamicArray<int> array(&mr);
    
    for (int i = 0; i < 10; ++i) {
        array.push_back(i * 10);
        std::cout << "Added: " << i * 10 << std::endl;
    }
    
    std::cout << "Index access:" << std::endl;
    for (std::size_t i = 0; i < array.size(); ++i) {
        std::cout << "array[" << i << "] = " << array[i] << std::endl;
    }
    
    std::cout << "Iterators:" << std::endl;
    for (auto it = array.begin(); it != array.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Range-based for:" << std::endl;
    for (int value : array) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Size: " << array.size() << ", capacity: " << array.capacity() << std::endl;
}

void demonstrate_complex_type() {
    std::cout << "\n === ComplexType demo === " << std::endl;
    
    FixedBlockMemoryResource mr(1024 * 1024);
    DynamicArray<ComplexType> array(&mr);
    
    array.push_back(ComplexType(1, 3.14, "First"));
    array.push_back(ComplexType(2, 2.71, "Second"));
    array.push_back(ComplexType(3, 1.41, "Third"));
    
    std::cout << "ComplexType array:" << std::endl;
    for (const auto& item : array) {
        std::cout << "ID: " << item.id << ", Value: " << item.value 
                  << ", Name: " << item.name << std::endl;
    }
    
    auto it = array.begin();
    ++it;
    it->value = 9.99;
    
    char modified_name[] = "Modified";
    std::memcpy(const_cast<char*>(it->name), modified_name, sizeof(modified_name));
    
    std::cout << "After modification:" << std::endl;
    for (const auto& item : array) {
        std::cout << "ID: " << item.id << ", Value: " << item.value 
                  << ", Name: " << item.name << std::endl;
    }
    
    std::cout << "Before pop_back: size = " << array.size() << std::endl;
    array.pop_back();
    std::cout << "After pop_back: size = " << array.size() << std::endl;
    
    array.clear();
    std::cout << "After clear: size = " << array.size() << std::endl;
}

void demonstrate_memory_reuse() {
    std::cout << "\n=== Memory reuse demo ===" << std::endl;
    
    FixedBlockMemoryResource mr(256);
    DynamicArray<int> array(&mr);
    
    std::cout << "Adding elements..." << std::endl;
    for (int i = 0; i < 5; ++i) {
        array.push_back(i);
        std::cout << "Added " << i << ", size: " << array.size() 
                  << ", capacity: " << array.capacity() << std::endl;
    }
    
    std::cout << "Removing elements..." << std::endl;
    while (!array.empty()) {
        std::cout << "Removed " << array[array.size() - 1] << std::endl;
        array.pop_back();
    }
    
    std::cout << "Adding again..." << std::endl;
    for (int i = 10; i < 15; ++i) {
        array.push_back(i);
        std::cout << "Added " << i << ", size: " << array.size() 
                  << ", capacity: " << array.capacity() << std::endl;
    }
}

int main() {
    try {
        demonstrate_int_array();
        demonstrate_complex_type();
        demonstrate_memory_reuse();
        
        std::cout << "\n=== All demos completed successfully ===" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}