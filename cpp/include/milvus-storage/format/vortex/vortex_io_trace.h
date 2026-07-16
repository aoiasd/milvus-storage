// Copyright 2023 Zilliz
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

namespace milvus_storage::vortex {

// These functions are no-ops unless milvus-storage is built with
// WITH_VORTEX_IO_TRACE=ON. Trace-enabled builds automatically reset the
// process-wide collector at library initialization and print it at shutdown.
void ResetIOTrace();
void PrintIOTrace();
void DisableIOTrace();
bool IsIOTraceEnabled();

}  // namespace milvus_storage::vortex
