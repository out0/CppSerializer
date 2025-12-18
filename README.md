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
        return s.data();
    }

    void desserialize(std::vector<uint8_t> raw)
    {
        Serializer s(raw);

        IntValue = s.read<int>();
        FloatValue = s.read<float>();
        DoubleValue = s.read<double>();
        StringValue = s.read<std::string>();
        BoolValue = s.read<bool>();
        VectorPointerVal = s.read<char>(10);
    }
```

Entire classes (or any memory address) can be serialized / desserialized as well, but you will have bad behavior if you copy
smart pointers, std::strings or anything that handles memory automatically, since you'll be copying their internal address as well. So, smart pointers are going to be duplicated at bit level, thus become invalid and leading to double free errors. If the class has smart pointers, build the serialization manually, writing every property to the Serialize buffer.

