#include "hash_functions.h"

size_t polynomial_rolling_hash::operator() (std::string const & str) const {
    /* TODO */
    const int b = 19;
    const size_t m = 3298534883309ul;
    size_t hash = 0;
    size_t p = 1;

    for(char c: str){
        hash += c * p;
        p = (p * b) % m;
    }

    return hash;
}

size_t fnv1a_hash::operator() (std::string const & str) const {
    /* TODO */
    const size_t prime = 0x00000100000001B3;
    const size_t basis = 0xCBF29CE484222325;
    size_t hash = basis;

    for (char c : str) {
        hash = hash ^ static_cast<size_t>(c);
        hash = hash * prime;
    }

    return hash;
}
