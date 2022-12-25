#ifndef _CONSTANTS_HPP_
#define _CONSTANTS_HPP_

#include <cstdint>

namespace memory_manager {
typedef int8_t byte;

const char* const kDebugFlag = "BADCAFE";

const int kFullPage = -1;

const int kFSACount = 6;

const int k16Bytes = 1 << 4;
const int k32Bytes = k16Bytes << 1;
const int k64Bytes = k32Bytes << 1;
const int k128Bytes = k64Bytes << 1;
const int k256Bytes = k128Bytes << 1;
const int k512Bytes = k256Bytes << 1;
const int kMegabyte = 1 << 20;
const int kMinMegabytes = 10 * kMegabyte;
}  // namespace memory_manager

#endif  // _CONSTANTS_HPP_