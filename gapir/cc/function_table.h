/*
 * Copyright (C) 2017 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GAPIR_FUNCTION_TABLE_H
#define GAPIR_FUNCTION_TABLE_H

#include "core/cc/log.h"

#include <stdint.h>
#include <functional>
#include <unordered_map>

namespace gapir {

class Stack;

// FunctionTable provides a mapping of function id to a VM function.
class FunctionTable {
public:
    // General signature for functions callable by the interpreter with a function call
    // instruction. The first argument is a pointer to the stack of the Virtual Machine and the
    // second argument is true if the caller expect the return value of the function to be pushed
    // to the stack. The function should return true if the function call was successful, false
    // otherwise.
    typedef std::function<bool(Stack*, bool)> Function;

    // The function identifier. These are part of the protocol between the server and the replay
    // system, and so must remain consistent.
    typedef uint16_t Id;

    // Inserts a function into the table.
    inline void insert(Id id, Function func);

    // Returns a function from the table, or nullptr if there is no function with the specified
    // identifier.
    inline Function* lookup(Id id);

private:
    // Map of the supported function ids to the actual function implementations
    std::unordered_map<Id, Function> mFunctions;
};

inline FunctionTable::Function* FunctionTable::lookup(Id id) {
    auto func = mFunctions.find(id);
    if (func == mFunctions.end()) {
        return nullptr;
    }
    return &func->second;
}

inline void FunctionTable::insert(Id id, Function func) {
    if (mFunctions.count(id) != 0) {
        GAPID_FATAL("Duplicate functions inserted into table");
    }
    mFunctions[id] = func;
}

}  // namespace gapir

#endif  // GAPIR_FUNCTION_TABLE_H
