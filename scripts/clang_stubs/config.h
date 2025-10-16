#ifndef SQ_STUB_CONFIG_H
#define SQ_STUB_CONFIG_H

/* Minimal configuration to satisfy libclang parsing when building tooling.
 * These values match a 64-bit little-endian Unix host (macOS arm64). Adjust
 * as needed if running the rewrite on a different architecture.
 */

#define LSB_FIRST 1

#define LP64 1
#define ILP32 0
#define LLP64 0

#define SIZEOF_CHAR 1
#define SIZEOF_SHORT 2
#define SIZEOF_INT 4
#define SIZEOF_LONG 8
#define SIZEOF_LONG_LONG 8
#define SIZEOF_VOID_P 8
#define SIZEOF_FLOAT 4
#define SIZEOF_DOUBLE 8

#endif /* SQ_STUB_CONFIG_H */
