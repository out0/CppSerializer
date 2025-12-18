#ifndef __SERIALIZER_CPP_H
#define __SERIALIZER_CPP_H

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

    void set_buffer(long size);

public:
    Serializer();

    Serializer(uint8_t *initial_buffer, long buffer_size);

    Serializer(std::vector<uint8_t> initial_buffer);

    Serializer(long initial_size);

    ~Serializer();

    /// @brief Writes the content to the internal buffer
    /// @param src memory data to be read and written to the internal buffer
    /// @param size_in_bytes number of bytes being read (watch out for overflows!)
    void write_raw(void *src, long size_in_bytes);

    // Reads size_in_bytes from the internal buffer to dest
    /// @param src memory position that will receive bytes from the internal buffer
    /// @param size_in_bytes number of bytes being read (watch out for overflows!)
    void read(void *dest, long size_in_bytes);

    /// @brief Writes a string input to the internal buffer
    /// @param s string
    void write(std::string s);

    /// @brief Writes a string input (char* format)
    /// @param p string
    void write(char *p);

        /// @brief Writes a string input (const char * format)
    /// @param p string
    void write(const char *p);

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

    std::vector<uint8_t> data();

    template <typename T>
    void write(T *arr, long count)
    {
        write_raw(arr, count * sizeof(T));
    }

    template <typename T>
    T* read(int count)
    {
        T * res = new T[count];
        for (int i = 0; i < count; i++) {
            res[i] = read<T>();
        }
        return res;
    }
};

template <>
std::string Serializer::read();

template <>
char *Serializer::read();

template <>
const char *Serializer::read();

#endif