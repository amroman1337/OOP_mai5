#include <memory_resource>
#include <list>
#include <cstddef>
#include <iterator>
#include <cstring>
#include <stdexcept>

class FixedBlockMemoryResource : public std::pmr::memory_resource {
public:
    FixedBlockMemoryResource(std::size_t total_size);
    ~FixedBlockMemoryResource();
    FixedBlockMemoryResource(const FixedBlockMemoryResource&) = delete;
    FixedBlockMemoryResource& operator=(const FixedBlockMemoryResource&) = delete;
private:
    void* memory_block_;
    std::size_t total_size_;
    std::size_t used_size_;
    std::list<std::pair<void*, std::size_t>> free_blocks_;
    void* do_allocate(std::size_t bytes, std::size_t alignment) override;
    void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) override;
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;
};

template<typename T>
class DynamicArray {
public:
    class Iterator {
    private:
        T* ptr_;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        Iterator(T* ptr = nullptr) : ptr_(ptr) {}
        reference operator*() const { return *ptr_; }
        pointer operator->() const { return ptr_; }
        
        Iterator& operator++() {
            ++ptr_;
            return *this;
        }
        Iterator operator++(int) {
            Iterator temp = *this;
            ++ptr_;
            return temp;
        }
        bool operator==(const Iterator& other) const { return ptr_ == other.ptr_; }
        bool operator!=(const Iterator& other) const { return ptr_ != other.ptr_; }
    };

    DynamicArray(std::pmr::memory_resource* mr = std::pmr::get_default_resource());
    DynamicArray(std::size_t initial_capacity, std::pmr::memory_resource* mr = std::pmr::get_default_resource());
    ~DynamicArray();

    DynamicArray(const DynamicArray&) = delete;
    DynamicArray& operator=(const DynamicArray&) = delete;
    void push_back(const T& value);
    void pop_back();
    T& operator[](std::size_t index);
    const T& operator[](std::size_t index) const;
    std::size_t size() const { return size_; }
    std::size_t capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }
    void clear();

    Iterator begin() { return Iterator(data_); }
    Iterator end() { return Iterator(data_ + size_); }
private:
    std::pmr::polymorphic_allocator<T> allocator_;
    T* data_;
    std::size_t size_;
    std::size_t capacity_;
    void resize_if_needed();
};

struct ComplexType {
    int id;
    double value;
    char name[32];
    
    ComplexType(int i = 0, double v = 0.0, const char* n = ""): id(i), value(v) {
        if (n) {
            size_t len = std::strlen(n);
            if (len > 31) len = 31;
            std::memcpy(name, n, len);
            name[len] = '\0';
        } else {
            name[0] = '\0';
        }
    }
    
    bool operator==(const ComplexType& other) const {
        return id == other.id && value == other.value && 
               std::strcmp(name, other.name) == 0;
    }
};