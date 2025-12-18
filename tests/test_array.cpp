#include <gtest/gtest.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include "../include/serializer.h"

TEST(SerializerppArray, IntArray)
{
    Serializer s;

    int arr[100];
    for (int i = 0; i < 100; i++)
        arr[i] = i;
    
    s.write(arr, 100);

    auto data = s.data();

    Serializer s2(data);

    int *arr2 = s2.read<int>(100);
    
    for (int i = 0; i < 100; i++)
        ASSERT_EQ(arr2[i], arr[i]);
}
