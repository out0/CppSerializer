#include <gtest/gtest.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include "../include/serializer.h"

TEST(SerializerppRaw, TestRawReadWrite)
{
    Serializer s;

    uint8_t payload[100];
    for (int i = 0; i < 100; i++)
        payload[i] = i;

    s.write_raw(payload, 100 * sizeof(uint8_t));

    // Write data is consistent
    std::vector<uint8_t> data = s.data();

    for (int i = 0; i < 100; i++)
        if (data[i] != i)
            FAIL();

    // Read data is consistent
    uint8_t *dest = new uint8_t[100];
    s.read(dest, 100 * sizeof(uint8_t));

    for (int i = 0; i < 100; i++)
        if (dest[i] != i)
            FAIL();
    
    delete []dest;
}

