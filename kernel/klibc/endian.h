#ifndef VORTEX_KLIBC_ENDIAN__
#define VORTEX_KLIBC_ENDIAN__

#define VORTEXLE 1

#include <stdint.h>
#include <klibc/byteswap.h>

static inline uint32_t be32toh(uint32_t big_endian_32bits)
{

#ifdef VORTEXLE
    return bswap_32(big_endian_32bits);
#else
    return big_endian_32bits;
#endif

}

static inline uint32_t le32toh(uint32_t little_endian_32bits)
{
#ifdef VORTEXLE
    return little_endian_32bits;
#else
    return bswap_32(little_endian_32bits);
#endif
}

static inline uint64_t be64toh(uint64_t big_endian_64bits) {
#ifdef VORTEXLE
    return bswap_64(big_endian_64bits);
#else
    return big_endian_64bits;
#endif
}

static inline uint64_t le64toh(uint64_t little_endian_64bits) {
#ifdef VORTEXLE
    return little_endian_64bits;
#else
    return bswap_64(little_endian_64bits);
#endif
}

#endif