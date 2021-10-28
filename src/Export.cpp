// #include "stdInclude.hpp"

#include <bitset>
#include <iostream>
#include <sstream>

template <typename T>
void writeBitset(T &bs, int size, std::ostream &dest)
{
    char buffer;
    int i = 0;
    for (i = 0; i < size; i++) {
        char bit = ((char)bs[i]) << (i%8);
        buffer |= bit;
        if (i % 8 == 7) {
            dest.write(&buffer, 1);
            buffer = 0;
        }
    }
    i %= 8;
    buffer = buffer << (8 - i);
    dest.write(&buffer, 1);
}

template <typename T>
void writeBitset(T &bs, int size, std::stringstream &dest)
{
    char buffer;
    int i = 0;
    for (i = 0; i < size; i++) {
        char bit = ((char)bs[i]) << (i%8);
        buffer |= bit;
        if (i % 8 == 7) {
            dest.write(&buffer, 1);
            buffer = 0;
        }
    }
    i %= 8;
    buffer = buffer << (8 - i);
    dest.write(&buffer, 1);
}

// int main(int argc, char const *argv[])
// {
//     std::bitset<20> bs;
//     bs.set();
//     std::stringstream st;
//     writeBitset(bs, bs.size(), st);
//     auto str = st.str();
//     for (auto &&c : str) {
//         for (int i = 0; i < 8; i++) {
//             printf("%d", !!((c << i) & 0x80));
//         }
//         printf("\n");
//     }
//     return 0;
// }
