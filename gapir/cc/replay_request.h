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

#ifndef GAPIR_REPLAY_REQUEST_H
#define GAPIR_REPLAY_REQUEST_H

#include "resource.h"

#include <stdint.h>

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace gapir {

class MemoryManager;
class ResourceProvider;
class ServerConnection;

// Class for storing the information about a replay request that came from the server
class ReplayRequest {
public:
    // Creates a new replay request and loads it content from the given gazer connection. Returns
    // the new replay request if loading it was successful or nullptr otherwise. The memory manager
    // is used to store the content of the replay request
    static std::unique_ptr<ReplayRequest> create(const ServerConnection& gazer,
                                                 ResourceProvider* resourceProvider,
                                                 MemoryManager* memoryManager);

    // Get the stack size required by the replay
    uint32_t getStackSize() const;

    // Get the volatile memory size required by the replay
    uint32_t getVolatileMemorySize() const;

    // Get the base address and the size of the constant memory
    const std::pair<const void*, uint32_t>& getConstantMemory() const;

    // Get the list of the resources with their size required by this replay
    const std::vector<Resource>& getResources() const;

    // Get the base address and the size (count of instructions) of the instruction list
    const std::pair<const uint32_t*, uint32_t>& getInstructionList() const;

private:
    ReplayRequest() = default;

    // Parse the replay request from the given memory address and returns true if exactly size
    // amount of bytes were used false otherwise
    bool load(void* data, uint32_t size);

    // Helper functions for loading the replay request from a raw byte buffer
    const uint8_t* loadStackSize(const uint8_t* ptr);
    const uint8_t* loadVolatileMemorySize(const uint8_t* ptr);
    const uint8_t* loadConstantMemory(const uint8_t* ptr);
    const uint8_t* loadResourceIds(const uint8_t* ptr);
    const uint8_t* loadInstructionList(const uint8_t* ptr);

    // The size of the stack required by the replay
    uint32_t mStackSize;

    // The size of the volatile memory required by the replay
    uint32_t mVolatileMemorySize;

    // The base address and the size of the constant memory
    std::pair<const void*, uint32_t> mConstantMemory;

    // The base address and the size of the instruction list
    std::pair<const uint32_t*, uint32_t> mInstructionList;

    // The list of resources (resource id, resource size) used by the replay
    std::vector<Resource> mResources;
};

}  // namespace gapir

#endif  // GAPIR_REPLAY_REQUEST_H
