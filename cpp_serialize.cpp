#include <string>
#include <memory>
#include <iostream>
#include <string.h>
#include <vector>

class Serializer
{
    std::vector<uint8_t> buffer;
    long bufferWritePos;
    long bufferReadPos;
    long bufferSize;
    const long header_size = sizeof(long);

    union
    {
        long val;
        uint8_t valb[sizeof(long)];
    } longp;

    union
    {
        int val;
        uint8_t valb[sizeof(int)];
    } intp;

    void set_buffer(long size)
    {
        if ((bufferSize - bufferWritePos) < (size + header_size))
        {
            buffer.resize(bufferSize + size + header_size);
            bufferSize = bufferSize + size + header_size;
        }
    }

    void _write(void *src, long size, bool header)
    {
        set_buffer(size);
        uint8_t *ptr = &buffer[0];

        if (header)
        {
            longp.val = size;
            memcpy(ptr + bufferWritePos, longp.valb, header_size);
            bufferWritePos += header_size;
        }
        memcpy(ptr + bufferWritePos, src, size);
        bufferWritePos += size;
    }

    std::string _read_string()
    {
        uint8_t *ptr = &buffer[0];
        memcpy(longp.valb, ptr + bufferReadPos, header_size);
        long size = longp.val;
        char p[size + 1];
        p[size] = 0;
        memcpy(p, ptr + bufferReadPos + header_size, size);
        bufferReadPos = bufferReadPos + header_size + size;
        return std::string(p);
    }

public:
    Serializer()
    {
        bufferSize = 0;
        bufferWritePos = 0;
        bufferReadPos = 0;
    }

    Serializer(uint8_t *initial_buffer, long size)
    {
        buffer.resize(size);
        memcpy(&buffer[0], initial_buffer, size);
        bufferSize = size;
        bufferWritePos = bufferSize;
        bufferReadPos = 0;
    }

    Serializer(std::vector<uint8_t> raw)
    {
        buffer = raw;
        bufferSize = raw.size();
        bufferWritePos = bufferSize;
        bufferReadPos = 0;
    }

    Serializer(long initial_size)
    {
        buffer.resize(initial_size, 0);
        bufferSize = initial_size;
        bufferReadPos = 0;
        bufferWritePos = 0;
    }

    ~Serializer()
    {
    }

    void write(void *buff, long size)
    {
        _write(buff, size, true);
    }

    void write(std::string s)
    {
        long size = s.size() * sizeof(char);
        const char *ptr = s.data();
        _write((void *)ptr, size, true);
    }

    template <typename T>
    void write(T val)
    {
        union
        {
            T val;
            uint8_t valb[sizeof(T)];
        } converter;

        converter.val = val;
        _write(converter.valb, sizeof(T), false);
    }

    std::vector<uint8_t> get()
    {
        return buffer;
    }

    void read(void *dest, long size)
    {
        uint8_t *ptr = &buffer[0];
        memcpy(dest, ptr + bufferReadPos, size);
        bufferReadPos = bufferReadPos + size;
    }

    template <typename T>
    T read()
    {
        union
        {
            T val;
            uint8_t valb[sizeof(T)];
        } converter;

        read(converter.valb, sizeof(T));
        return converter.val;
    }

    template <typename T>
    T *read_ptr()
    {
        if (bufferReadPos + header_size > bufferSize)
        {
            return nullptr;
        }
        uint8_t *ptr = &buffer[0];
        memcpy(longp.valb, ptr + bufferReadPos, header_size);
        long size = longp.val;

        if (bufferReadPos + header_size + size > bufferSize)
        {
            return nullptr;
        }

        T *ret = new T[size];

        memcpy(ret, ptr + bufferReadPos + header_size, size);
        bufferReadPos = bufferReadPos + header_size + size;
        return ret;
    }
};

template <>
std::string Serializer::read<std::string>()
{
    return _read_string();
}

class TestClass
{

public:
    int IntValue;
    float FloatValue;
    double DoubleValue;
    bool BoolValue;

    std::string StringValue;
    char *VectorPointerVal;

    std::unique_ptr<TestClass> UniqueEmptySelfPtr;
    std::unique_ptr<TestClass> UniqueSelfPtr;

    std::shared_ptr<TestClass> SharedEmptySelfPtr;
    std::shared_ptr<TestClass> SharedSelfPtr;

    std::vector<uint8_t> serialize()
    {
        Serializer s(100);
        s.write(IntValue);
        s.write(FloatValue);
        s.write(DoubleValue);
        s.write(StringValue);
        s.write(BoolValue);
        s.write(VectorPointerVal, 10);
        return s.get();
    }

    void desserialize(std::vector<uint8_t> raw)
    {
        Serializer s(raw);

        IntValue = s.read<int>();
        FloatValue = s.read<float>();
        DoubleValue = s.read<double>();
        StringValue = s.read<std::string>();
        BoolValue = s.read<bool>();
        VectorPointerVal = s.read_ptr<char>();
    }
};

void printTestClass(TestClass &obj)
{
    std::cout << "IntValue: " << obj.IntValue << std::endl;
    std::cout << "FloatValue: " << obj.FloatValue << std::endl;
    std::cout << "DoubleValue: " << obj.DoubleValue << std::endl;
    std::cout << "StringValue: " << obj.StringValue << std::endl;
    std::cout << "BoolValue: " << (obj.BoolValue ? "true" : "false") << std::endl;

    if (obj.SharedSelfPtr)
    {
        std::cout << "SharedSelfPtr->IntValue: " << obj.SharedSelfPtr->IntValue << std::endl;
        std::cout << "SharedSelfPtr->FloatValue: " << obj.SharedSelfPtr->FloatValue << std::endl;
        std::cout << "SharedSelfPtr->DoubleValue: " << obj.SharedSelfPtr->DoubleValue << std::endl;
        std::cout << "SharedSelfPtr->StringValue: " << obj.SharedSelfPtr->StringValue << std::endl;
    }

    if (obj.VectorPointerVal)
    {
        std::cout << "[";
        for (int i = 0; i < 10; i++)
        {
            printf(" %d", obj.VectorPointerVal[i]);
        }
        std::cout << " ]\n";
    }
}

std::vector<uint8_t> build_raw()
{
    TestClass c1;

    c1.IntValue = 12;
    c1.FloatValue = 123.45;
    c1.DoubleValue = 323.45;
    c1.StringValue = "random_string";
    c1.VectorPointerVal = new char[10];
    for (int i = 0; i < 10; i++)
    {
        c1.VectorPointerVal[i] = i;
    }
    c1.BoolValue = true;

    c1.SharedSelfPtr = std::make_shared<TestClass>();
    c1.SharedSelfPtr->IntValue = 100;
    c1.SharedSelfPtr->FloatValue = 200.5;
    c1.SharedSelfPtr->DoubleValue = 300.75;
    c1.SharedSelfPtr->StringValue = "shared_string";
    c1.SharedSelfPtr->BoolValue = true;
    c1.SharedEmptySelfPtr = nullptr;

    printTestClass(c1);

    return c1.serialize();
}

class TestClass2
{

public:
    int IntValue;
    float FloatValue;
    double DoubleValue;
    bool BoolValue;
    std::string StringValue;
    char *VectorPointerVal;
};

int main(int argc, char **argv)
{

    TestClass2 c1;

    c1.IntValue = 12;
    c1.FloatValue = 123.45;
    c1.DoubleValue = 323.45;
    c1.StringValue = "random_string";
    c1.BoolValue = true;
    c1.VectorPointerVal = new char[10];
    for (int i = 0; i < 10; i++)
    {
        c1.VectorPointerVal[i] = i;
    }

    Serializer s;
    s.write(&c1, sizeof(c1));

    TestClass2 *c2 = s.read_ptr<TestClass2>();
    std::cout << "IntValue: " << c2->IntValue << std::endl;
    std::cout << "FloatValue: " << c2->FloatValue << std::endl;
    std::cout << "DoubleValue: " << c2->DoubleValue << std::endl;
    std::cout << "StringValue: " << c2->StringValue << std::endl;
    std::cout << "BoolValue: " << (c2->BoolValue ? "true" : "false") << std::endl;

    // long classSize = sizeof(TestClass);

    // std::vector<uint8_t> raw_data = build_raw();
    // printf("\n\n.......................\n\n");

    // TestClass c2;
    // c2.desserialize(raw_data);

    // printTestClass(c2);
    // return 0;
}