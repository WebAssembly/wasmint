#include "HeapData.h"

wasm_module::HeapData::HeapData(uint32_t startSize, uint32_t maxSize) : startSize_(startSize * pageSize_),
                                                                        maxSize_(maxSize * pageSize_) {
}