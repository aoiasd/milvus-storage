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

#include "milvus-storage/format/vortex/vortex_io_trace.h"

#include "vortex_bridge.h"

namespace milvus_storage::vortex {

void ResetIOTrace() { ffi::reset_io_trace_ffi(); }

void PrintIOTrace() { ffi::print_io_trace_ffi(); }

void DisableIOTrace() { ffi::disable_io_trace_ffi(); }

bool IsIOTraceEnabled() { return ffi::io_trace_enabled_ffi(); }

}  // namespace milvus_storage::vortex
