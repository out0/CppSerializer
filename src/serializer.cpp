#include "../include/serializer.h"

void Serializer::set_buffer(long size)
{
    if ((bufferSize - bufferWritePos) < (size + header_size))
    {
        buffer.resize(bufferSize + size + header_size);
        bufferSize = bufferSize + size + header_size;
    }
}

// void Serializer::_write(void *src, long size, bool header)
// {
//     set_buffer(size);
//     uint8_t *ptr = &buffer[0];

//     if (header)
//     {
//         longp.val = size;
//         memcpy(ptr + bufferWritePos, longp.valb, header_size);
//         bufferWritePos += header_size;
//     }
//     memcpy(ptr + bufferWritePos, src, size);
//     bufferWritePos += size;
// }

Serializer::Serializer()
{
    bufferSize = 0;
    bufferWritePos = 0;
    bufferReadPos = 0;
}

Serializer::Serializer(uint8_t *initial_buffer, long size)
{
    buffer.resize(size);
    memcpy(&buffer[0], initial_buffer, size);
    bufferSize = size;
    bufferWritePos = bufferSize;
    bufferReadPos = 0;
}

Serializer::Serializer(std::vector<uint8_t> raw)
{
    buffer = raw;
    bufferSize = raw.size();
    bufferWritePos = bufferSize;
    bufferReadPos = 0;
}

Serializer::Serializer(long initial_size)
{
    buffer.resize(initial_size, 0);
    bufferSize = initial_size;
    bufferReadPos = 0;
    bufferWritePos = 0;
}

Serializer::~Serializer()
{
}

void Serializer::write_raw(void *src, long size)
{
    set_buffer(size);
    uint8_t *ptr = &buffer[0];
    memcpy(ptr + bufferWritePos, src, size);
    bufferWritePos += size;
}
void Serializer::read(void *dest, long size)
{
    uint8_t *ptr = &buffer[0];
    memcpy(dest, ptr + bufferReadPos, size);
    bufferReadPos = bufferReadPos + size;
}

void Serializer::write(std::string s)
{
    long size = s.size() * sizeof(char);
    const char *ptr = s.data();
    write(size);
    write_raw((void *)ptr, size);
}

void Serializer::write(char *p)
{
    long size = strlen(p) * sizeof(char);
    write(size);
    write_raw((void *)p, size);
}

void Serializer::write(const char *p)
{
    long size = strlen(p) * sizeof(char);
    write(size);
    write_raw((void *)p, size);
}

template <>
std::string Serializer::read()
{
    long size = read<long>();
    std::string str(size, 0);
    read(str.data(), size);
    return str;
}

template <>
char *Serializer::read()
{
    long size = read<long>();
    char *str = new char[size];

    uint8_t *ptr = &buffer[0];
    read(str, size);
    return str;
}
template <>
const char *Serializer::read()
{
    return read<char *>();
}

// long Serializer::_read_size_in_header()
// {
//     uint8_t *ptr = &buffer[0];
//     memcpy(longp.valb, ptr + bufferReadPos, header_size);
//     return longp.val;
// }

template <typename T>
void write(T val)
{
    union
    {
        T val;
        uint8_t valb[sizeof(T)];
    } converter;

    converter.val = val;
    write_raw(converter.valb, sizeof(T));
}

std::vector<uint8_t> Serializer::data()
{
    return buffer;
}
