#include "laba5.h"
#include <algorithm>

FixedBlockMemoryResource::FixedBlockMemoryResource(std::size_t total_size): 
            total_size_(total_size), used_size_(0), memory_block_(nullptr) {
    memory_block_ = ::operator new(total_size_);
    free_blocks_.emplace_back(memory_block_, total_size_);
}

FixedBlockMemoryResource::~FixedBlockMemoryResource() {
    if (memory_block_) {
        ::operator delete(memory_block_);
    }
}

void* FixedBlockMemoryResource::do_allocate(std::size_t bytes, std::size_t) {
    for (auto it = free_blocks_.begin(); it != free_blocks_.end(); ++it) {
        void* block_ptr = it->first;
        std::size_t block_size = it->second;
        
        if (block_size >= bytes) {
            free_blocks_.erase(it);
            if (block_size > bytes) {
                void* remaining_ptr = static_cast<char*>(block_ptr) + bytes;
                std::size_t remaining_size = block_size - bytes;
                free_blocks_.emplace_back(remaining_ptr, remaining_size);
            }
            used_size_ += bytes;
            return block_ptr;
        }
    }
    throw std::bad_alloc();
}

void FixedBlockMemoryResource::do_deallocate(void* p, std::size_t bytes, std::size_t) {
    free_blocks_.emplace_back(p, bytes);
    used_size_ -= bytes;
    free_blocks_.sort([](const auto& a, const auto& b) {
        return a.first < b.first;
    });

    for (auto it = free_blocks_.begin(); it != free_blocks_.end(); ) {
        auto next = std::next(it);
        if (next != free_blocks_.end()) {
            void* current_end = static_cast<char*>(it->first) + it->second;
            if (current_end == next->first) {
                it->second += next->second;
                free_blocks_.erase(next);
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }
}

bool FixedBlockMemoryResource::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
    return this == &other;
}

template<typename T>
DynamicArray<T>::DynamicArray(std::pmr::memory_resource* mr): allocator_(mr), data_(nullptr), size_(0), capacity_(0) {}

template<typename T>
DynamicArray<T>::DynamicArray(std::size_t initial_capacity, std::pmr::memory_resource* mr): 
            allocator_(mr), data_(nullptr), size_(0), capacity_(0) {
    if (initial_capacity > 0) {
        data_ = allocator_.allocate(initial_capacity);
        capacity_ = initial_capacity;
    }
}

template<typename T>
DynamicArray<T>::~DynamicArray() {
    if (data_) {
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        allocator_.deallocate(data_, capacity_);
    }
}

template<typename T>
void DynamicArray<T>::resize_if_needed() {
    if (size_ >= capacity_) {
        std::size_t new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
        T* new_data = allocator_.allocate(new_capacity);
        
        for (std::size_t i = 0; i < size_; ++i) {
            new(new_data + i) T(std::move(data_[i]));
            data_[i].~T();
        }
        
        if (data_) {
            allocator_.deallocate(data_, capacity_);
        }
        data_ = new_data;
        capacity_ = new_capacity;
    }
}

template<typename T>
void DynamicArray<T>::push_back(const T& value) {
    resize_if_needed();
    new(data_ + size_) T(value);
    ++size_;
}

template<typename T>
void DynamicArray<T>::pop_back() {
    if (size_ > 0) {
        data_[size_ - 1].~T();
        --size_;
    }
}

template<typename T>
T& DynamicArray<T>::operator[](std::size_t index) {
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }
    return data_[index];
}

template<typename T>
const T& DynamicArray<T>::operator[](std::size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }
    return data_[index];
}

template<typename T>
void DynamicArray<T>::clear() {
    for (std::size_t i = 0; i < size_; ++i) {
        data_[i].~T();
    }
    size_ = 0;
}

template class DynamicArray<int>;
template class DynamicArray<ComplexType>;