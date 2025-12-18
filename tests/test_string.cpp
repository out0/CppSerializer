#include <gtest/gtest.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include "../include/serializer.h"

TEST(SerializerppString, TestString)
{
    Serializer s;

    std::string str = "aaaaaa";
    s.write(str);
    ASSERT_TRUE(strcmp(str.c_str(), s.read<const char *>()) == 0);   
    s.write(str);
    ASSERT_TRUE(strcmp(str.c_str(), s.read<char *>()) == 0);
    s.write(str);
    ASSERT_EQ(std::string(str), s.read<std::string>());       
}

TEST(SerializerppString, TestStringDirectWrite)
{
    Serializer s;
    s.write("aaaaaa");
    ASSERT_EQ("aaaaaa", s.read<std::string>());
}

TEST(SerializerppString, TestStringDirectWriteConstStrChar)
{
    Serializer s;
    s.write("aaaaaa");
    ASSERT_TRUE(strcmp("aaaaaa", s.read<char *>()) == 0);
}

TEST(SerializerppString, TestStringDirectWriteStrChar)
{
    Serializer s;
    s.write("aaaaaa");
    ASSERT_TRUE(strcmp("aaaaaa", s.read<const char *>()) == 0);
}

TEST(SerializerppString, TestStringDirectWriteArrayStrChar)
{
    Serializer s;
    char str[] = "aaaaaaa";
    s.write(str);
    ASSERT_TRUE(strcmp(str, s.read<const char *>()) == 0);   
    s.write(str);
    ASSERT_TRUE(strcmp(str, s.read<char *>()) == 0);
    s.write(str);
    ASSERT_EQ(std::string(str), s.read<std::string>());   
}