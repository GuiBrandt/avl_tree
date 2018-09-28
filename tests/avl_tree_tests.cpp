#include <avl_tree.hpp>
#include <gtest/gtest.h>

TEST(Insert, Leaf) {
    avl_tree<int> t;
    t.insert(0);

    EXPECT_TRUE(t.is_leaf());
    ASSERT_TRUE(t.includes(0));
}

TEST(Insert, NoRebalance) {
    avl_tree<int> t;
    t.insert(1);
    t.insert(2);
    t.insert(0);

    EXPECT_EQ(t.height(), 2);
    ASSERT_TRUE(t.includes(0));
}

TEST(Insert, RepeatedNoRebalance) {
    avl_tree<int> t;
    t.insert(1);
    t.insert(2);

    ASSERT_THROW(t.insert(2), const char*);
}

TEST(Includes, HasDataNoRebalance) {
    avl_tree<int> t;
    t.insert(3);
    t.insert(1);
    t.insert(5);
    t.insert(4);
    t.insert(2);

    EXPECT_EQ(t.height(), 3);

    for (int i = 1; i <= 5; i++)
        ASSERT_TRUE(t.includes(i));
}

TEST(Includes, DoesNotHaveDataNoRebalance) {
    avl_tree<int> t;
    t.insert(3);
    t.insert(1);
    t.insert(5);
    t.insert(4);
    t.insert(2);

    EXPECT_EQ(t.height(), 3);
    ASSERT_FALSE(t.includes(10));
}

TEST(Insert, RebalanceLeft) {
    avl_tree<int> t;
    for (int i = 1; i <= 10; i++)
        t.insert(i);

    EXPECT_EQ(t.height(), 4);

    for (int i = 1; i <= 10; i++)
        ASSERT_TRUE(t.includes(i));
}

TEST(Insert, RepeatedRebalanceLeft) {
    avl_tree<int> t;
    for (int i = 1; i <= 10; i++)
        t.insert(i);

    EXPECT_EQ(t.height(), 4);
    ASSERT_THROW(t.insert(5), const char*);
}

TEST(Insert, RebalanceRight) {
    avl_tree<int> t;

    for (int i = 10; i >= 1; i--)
        t.insert(i);

    EXPECT_EQ(t.height(), 4);
    
    for (int i = 1; i <= 10; i++)
        ASSERT_TRUE(t.includes(i));
}

TEST(Insert, RepeatedRebalanceRight) {
    avl_tree<int> t;

    for (int i = 10; i >= 1; i--)
        t.insert(i);

    EXPECT_EQ(t.height(), 4);
    ASSERT_THROW(t.insert(5), const char*);
}


TEST(Minimum, Empty) {
    avl_tree<float> t;

    ASSERT_THROW(t.min(), const char*);
}

TEST(Minimum, NotEmpty) {
    avl_tree<float> t;
    t.insert(1.0);

    ASSERT_NO_THROW(t.min());
    ASSERT_EQ(t.min(), 1.0);
}

TEST(Minimum, Leaf) {
    avl_tree<float> t;
    t.insert(4);
    t.insert(2);
    t.insert(1);
    t.insert(6);

    ASSERT_EQ(t.min(), 1);
}

TEST(Minimum, MultiLevel) {
    avl_tree<float> t;
    t.insert(4);
    t.insert(2);
    t.insert(7);
    t.insert(6);
    t.insert(8);
    t.insert(3);
    t.insert(5);
    t.insert(1);

    ASSERT_EQ(t.min(), 1);
}

TEST(Maximum, Empty) {
    avl_tree<float> t;

    ASSERT_THROW(t.max(), const char*);
}

TEST(Maximum, NotEmpty) {
    avl_tree<float> t;
    t.insert(1);

    ASSERT_NO_THROW(t.max());
}

TEST(Maximum, Leaf) {
    avl_tree<float> t;
    t.insert(4);
    t.insert(2);
    t.insert(1);
    t.insert(6);

    ASSERT_EQ(t.max(), 6);
}

TEST(Maximum, MultiLevel) {
    avl_tree<float> t;
    t.insert(4);
    t.insert(2);
    t.insert(7);
    t.insert(6);
    t.insert(8);
    t.insert(3);
    t.insert(5);
    t.insert(1);

    ASSERT_EQ(t.max(), 8);
}

TEST(Popleft, Empty) {
    avl_tree<float> t;

    ASSERT_THROW(t.popleft(), const char*);
}

TEST(Popleft, NotEmpty) {
    avl_tree<float> t;
    t.insert(1);

    ASSERT_NO_THROW(t.popleft());
}

TEST(Popleft, Leaf) {
    avl_tree<float> t;
    t.insert(4);
    t.insert(2);
    t.insert(1);
    t.insert(6);

    ASSERT_EQ(t.popleft(), 1);
    ASSERT_FALSE(t.includes(1));
}

TEST(Popleft, MultiLevel) {
    avl_tree<float> t;
    t.insert(4);
    t.insert(2);
    t.insert(7);
    t.insert(6);
    t.insert(8);
    t.insert(3);
    t.insert(5);
    t.insert(1);

    ASSERT_EQ(t.popleft(), 1);
    ASSERT_FALSE(t.includes(1));
}

TEST(Pop, Empty) {
    avl_tree<float> t;

    ASSERT_THROW(t.pop(), const char*);
}

TEST(Pop, NotEmpty) {
    avl_tree<float> t;
    t.insert(1);

    ASSERT_NO_THROW(t.pop());
}

TEST(Pop, Leaf) {
    avl_tree<float> t;
    t.insert(4);
    t.insert(2);
    t.insert(1);
    t.insert(6);

    ASSERT_EQ(t.pop(), 6);
    ASSERT_FALSE(t.includes(6));
}

TEST(Pop, MultiLevel) {
    avl_tree<float> t;
    t.insert(4);
    t.insert(2);
    t.insert(7);
    t.insert(6);
    t.insert(8);
    t.insert(3);
    t.insert(5);
    t.insert(1);

    ASSERT_EQ(t.pop(), 8);
    ASSERT_FALSE(t.includes(8));
}

TEST(Remove, Empty) {
    avl_tree<double> t;
    
    ASSERT_THROW(t.remove(7), const char*);
}

TEST(Remove, HasInfo_Leaf) {
    avl_tree<double> t;
    t.insert(1.0);
    t.insert(2.0);
    t.insert(3.0);
    
    ASSERT_NO_THROW(t.remove(3.0));
    ASSERT_FALSE(t.includes(3.0));
}

TEST(Remove, DoesNotHaveInfo_Leaf) {
    avl_tree<double> t;
    t.insert(1.0);
    t.insert(2.0);
    t.insert(3.0);
    
    ASSERT_THROW(t.remove(10.0), const char*);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    
    return RUN_ALL_TESTS();
}