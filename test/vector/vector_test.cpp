#define LIBME_VECTOR_DEBUG

#include <libme/vector.hpp>

#include <gtest/gtest.h>

#include <string.h>

struct Object {
  virtual bool verify() const = 0;
};

struct Object1 : public Object {

  float x, y;

  Object1()
    : x(0.83F), y(7.45F) { }

  bool verify() const override
  {
    return x == 0.83F && y == 7.45F;
  }

};

struct Object2 : public Object {

  me::vector<int> arr;

  Object2()
    : arr({1, 2, 3, 4, 5, 6, 7, 8, 9}) { }
  /* Vector elements is valid after init here */

  bool verify() const override
  {
    if (arr.size() != 9)
      return false;

    return arr[0] == 1 && arr[1] == 2 && arr[2] == 3 && arr[3] == 4 && arr[4] == 5 && arr[5] == 6 && arr[6] == 7 && arr[7] == 8 && arr[8] == 9;
  }

};

struct Object3 : public Object {

  int x;
  const char* name;

  Object3()
    : x(38), name("Men vad är de") {}

  bool verify() const override
  {
    return x == 38 && strcmp(name, "Men vad är de") == 0;
  }

};



class VectorTest : public ::testing::Test {

protected:

  void SetUp() override
  {
  }

  void TearDown() override
  {
  }

  me::vector<Object1> v1;
  me::vector<Object2> v2;
  me::vector<Object3> v3;
  me::vector<Object3*> v4;

};

TEST_F(VectorTest, IsEmptyInitially)
{
  EXPECT_EQ(v1.size(), 0);
}

TEST_F(VectorTest, Modifying)
{
  using Obj = Object2;
  me::vector<Object2> &v = v2;

  v.push_back(Obj());

  EXPECT_EQ(v.size(), 1);

  ASSERT_TRUE(v.at(0).verify());

  v.push_back(Obj());
  v.push_back(Obj());

  EXPECT_EQ(v.size(), 3);

  ASSERT_TRUE(v.at(1).verify());
  ASSERT_TRUE(v.at(2).verify());

  for (const Obj &o : v)
    ASSERT_TRUE(o.verify());

  v.erase(v.end() - 1);

  EXPECT_EQ(v.size(), 2);

  for (const Obj &o : v)
    ASSERT_TRUE(o.verify());

  v.emplace_back();
  v.emplace_back();

  EXPECT_EQ(v.size(), 4);

  for (const Obj &o : v)
    ASSERT_TRUE(o.verify());

  v.reserve(7);
  EXPECT_EQ(v.size(), 4);
  EXPECT_EQ(v.capacity(), 7);

  v.push_back(Obj());

  EXPECT_EQ(v.size(), 5);
  EXPECT_EQ(v.capacity(), 7);

  for (const Obj &o : v)
    ASSERT_TRUE(o.verify());

  v.erase(v.begin());

  EXPECT_EQ(v.size(), 4);
  EXPECT_EQ(v.capacity(), 7);

  for (const Obj &o : v)
    ASSERT_TRUE(o.verify());

  ASSERT_FALSE(v.is_empty());
}

TEST_F(VectorTest, Access)
{
}


int main(int , char** )
{
  ::testing::InitGoogleTest();
  int result = RUN_ALL_TESTS();
  if (result == 0)
  {
    printf("success!\n");
    return 0;
  }
  return 0;
}
