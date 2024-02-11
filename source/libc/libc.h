//
// Seeds libc (Seeds C Libary)
//

#pragma once

#include "libc/io/io.h"    // io (like cout() and cin(), aswell as file ops)
#include "libc/util/mem.h" // memory and string utilities
#include "krnl/cmd/cmd.h"  // just gives the 'int cmd(const char* str)' command, basically system();

// integer types
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long int uint64_t;

typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int int64_t;

// booleans (:
typedef unsigned char bool;
#define true 1
#define false 0

#define null (void*)0 // i just used null instead of NULL cus it looks cooler

// return numbers
// note: standard return type is 'int'
#define RETURN_SUCCESS      0x00
#define RETURN_FAILURE      0x01

#define ERR_FAILURE         0x01
#define ERR_CMD_UNKNOWN_CMD 0x02
