#include <gtest/gtest.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include "../include/serializer.h"

TEST(SerializerppBasic, TestInt)
{
    Serializer s;

    int i = 123;
    s.write(i);

    auto data = s.data();

    Serializer s2(data);

    ASSERT_EQ(s2.read<int>(), i);
}

TEST(SerializerppBasic, TestManyInt)
{
    Serializer s;

    for (int i = -100; i < 100; i++)
        s.write(i);

    auto data = s.data();

    Serializer s2(data);

    for (int i = -100; i < 100; i++)
        ASSERT_EQ(s2.read<int>(), i);
}

TEST(SerializerppBasic, TestBasicTypes)
{
    Serializer s;

    s.write((short)-12);
    s.write((unsigned short)12);
    s.write((int)123);
    s.write((int)-123);
    s.write((int)0);
    s.write((unsigned int)123);
    s.write((unsigned int)0);
    s.write((long)998765576);
    s.write((long)-998765576);
    s.write((unsigned long)998765576);
    s.write((float)123.123);
    s.write((float)-223.123);
    s.write((float)0.0);
    s.write((double)133.123);
    s.write((double)-133.123);
    s.write((double)0.0);
    s.write(true);
    s.write(false);
    s.write('a');

    auto data = s.data();

    Serializer s2(data);

    ASSERT_EQ(s2.read<short>(), (short)-12);
    ASSERT_EQ(s2.read<unsigned short>(), (unsigned short)12);
    ASSERT_EQ(s2.read<int>(), (int)123);
    ASSERT_EQ(s2.read<int>(), (int)-123);
    ASSERT_EQ(s2.read<int>(), (int)0);
    ASSERT_EQ(s2.read<unsigned int>(), (unsigned int)123);
    ASSERT_EQ(s2.read<unsigned int>(), (unsigned int)0);
    ASSERT_EQ(s2.read<long>(), (long)998765576);
    ASSERT_EQ(s2.read<long>(), (long)-998765576);
    ASSERT_EQ(s2.read<unsigned long>(), (unsigned long)998765576);
    ASSERT_FLOAT_EQ(s2.read<float>(), (float)123.123);
    ASSERT_FLOAT_EQ(s2.read<float>(), (float)-223.123);
    ASSERT_FLOAT_EQ(s2.read<float>(), (float)0.0);
    ASSERT_DOUBLE_EQ(s2.read<double>(), (double)133.123);
    ASSERT_DOUBLE_EQ(s2.read<double>(), (double)-133.123);
    ASSERT_DOUBLE_EQ(s2.read<double>(), (double)0.0);
    ASSERT_EQ(s2.read<bool>(), true);
    ASSERT_EQ(s2.read<bool>(), false);
    ASSERT_EQ(s2.read<char>(), 'a');
}
