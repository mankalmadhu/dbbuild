#ifndef COMPILER_H
#define COMPILER_H

#include "storage_types.h"

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement);

#endif // COMPILER_H
