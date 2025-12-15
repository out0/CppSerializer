# CppSerializer

This is a C++ binary serializer / desserializer that can convert classes and variables into a byte array, which
is useful to send through the network.

Using this is as simple as:

```cpp

 std::vector<uint8_t> serialize()
    {
        Serializer s;
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
```

Entire classes can be serialized / desserialized as well, as long as it does not contain any smart pointers.

The problem with smart pointers is that they are going to be duplicated at bit level, thus become invalid and leading to double free errors. If the class has smart pointers, build the serialization manually, writing every property to the Serialize buffer.


```cpp

class TestClass
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

    TestClass c1;

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

    TestClass *c2 = s.read_ptr<TestClass>();
    std::cout << "IntValue: " << c2->IntValue << std::endl;
    std::cout << "FloatValue: " << c2->FloatValue << std::endl;
    std::cout << "DoubleValue: " << c2->DoubleValue << std::endl;
    std::cout << "StringValue: " << c2->StringValue << std::endl;
    std::cout << "BoolValue: " << (c2->BoolValue ? "true" : "false") << std::endl;

}

```
