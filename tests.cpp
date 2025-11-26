#include "laba5.h"
#include <gtest/gtest.h>
#include <vector>

TEST(FixedBlockMemoryResourceTest, BasicAllocation) {
    FixedBlockMemoryResource mr(1024);
    
    void* ptr1 = mr.allocate(100, 1);
    ASSERT_NE(ptr1, nullptr);
    void* ptr2 = mr.allocate(200, 1);
    ASSERT_NE(ptr2, nullptr);
    mr.deallocate(ptr1, 100, 1);
    mr.deallocate(ptr2, 200, 1);
}

TEST(FixedBlockMemoryResourceTest, OutOfMemory) {
    FixedBlockMemoryResource mr(100);
    
    void* ptr1 = mr.allocate(50, 1);
    ASSERT_NE(ptr1, nullptr);
    EXPECT_THROW(mr.allocate(60, 1), std::bad_alloc);
    mr.deallocate(ptr1, 50, 1);
}

TEST(FixedBlockMemoryResourceTest, MemoryReuse) {
    FixedBlockMemoryResource mr(200);
    
    void* ptr1 = mr.allocate(100, 1);
    mr.deallocate(ptr1, 100, 1);
    void* ptr2 = mr.allocate(100, 1);
    EXPECT_EQ(ptr1, ptr2);
    mr.deallocate(ptr2, 100, 1);
}

TEST(DynamicArrayTest, IntArrayBasicOperations) {
    FixedBlockMemoryResource mr(1024);
    DynamicArray<int> array(&mr);
    
    EXPECT_TRUE(array.empty());
    EXPECT_EQ(array.size(), 0);
    array.push_back(1);
    array.push_back(2);
    array.push_back(3);
    EXPECT_FALSE(array.empty());
    EXPECT_EQ(array.size(), 3);
    EXPECT_EQ(array[0], 1);
    EXPECT_EQ(array[1], 2);
    EXPECT_EQ(array[2], 3);
    array.pop_back();
    EXPECT_EQ(array.size(), 2);
    EXPECT_EQ(array[1], 2);
}

TEST(DynamicArrayTest, ComplexTypeArray) {
    FixedBlockMemoryResource mr(1024);
    DynamicArray<ComplexType> array(&mr);
    
    ComplexType ct1(1, 1.1, "first");
    ComplexType ct2(2, 2.2, "second");
    
    array.push_back(ct1);
    array.push_back(ct2);
    EXPECT_EQ(array.size(), 2);
    EXPECT_TRUE(array[0] == ct1);
    EXPECT_TRUE(array[1] == ct2);
    array.clear();
    EXPECT_TRUE(array.empty());
}

TEST(DynamicArrayTest, IteratorOperations) {
    FixedBlockMemoryResource mr(1024);
    DynamicArray<int> array(&mr);
    
    for (int i = 0; i < 5; ++i) {
        array.push_back(i * 10);
    }
    
    auto it = array.begin();
    EXPECT_EQ(*it, 0);
    ++it;
    EXPECT_EQ(*it, 10);
    auto it2 = it++;
    EXPECT_EQ(*it2, 10);
    EXPECT_EQ(*it, 20);
    
    auto begin1 = array.begin();
    auto begin2 = array.begin();
    auto end = array.end();
    
    EXPECT_EQ(begin1, begin2);
    EXPECT_NE(begin1, end);
}

TEST(DynamicArrayTest, IteratorRange) {
    FixedBlockMemoryResource mr(1024);
    DynamicArray<int> array(&mr);
    
    std::vector<int> expected = {1, 2, 3, 4, 5};
    for (int value : expected) {
        array.push_back(value);
    }
    std::vector<int> actual;
    for (int value : array) {
        actual.push_back(value);
    }
    EXPECT_EQ(actual, expected);
}

TEST(DynamicArrayTest, ResizeBehavior) {
    FixedBlockMemoryResource mr(1024);
    DynamicArray<int> array(&mr);
    
    EXPECT_EQ(array.capacity(), 0);
    array.push_back(1);
    EXPECT_GE(array.capacity(), 1);
    
    std::size_t old_capacity = array.capacity();
    while (array.size() < old_capacity) {
        array.push_back(42);
    }
    array.push_back(99);
    EXPECT_GT(array.capacity(), old_capacity);
}

TEST(DynamicArrayTest, OutOfRangeAccess) {
    FixedBlockMemoryResource mr(1024);
    DynamicArray<int> array(&mr);
    
    array.push_back(1);
    array.push_back(2);
    
    EXPECT_NO_THROW(array[0]);
    EXPECT_NO_THROW(array[1]);
    EXPECT_THROW(array[2], std::out_of_range);
    EXPECT_THROW(array[100], std::out_of_range);
}

TEST(DynamicArrayTest, EmptyArrayOperations) {
    FixedBlockMemoryResource mr(1024);
    DynamicArray<int> array(&mr);
    
    EXPECT_TRUE(array.empty());
    EXPECT_EQ(array.size(), 0);
    EXPECT_NO_THROW(array.pop_back());
    EXPECT_THROW(array[0], std::out_of_range);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}