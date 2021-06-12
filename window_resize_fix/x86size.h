#pragma once
#include "stdafx.h"
uint32_t x86size(const byte* pc);
namespace MemTools {
	inline const void* readPtr(const byte* at) {
		return (const void*)*(uint32_t*)at;
	}
	const void* readLEPtr(const void* at);
	const void* readLERPtr(const void* at, const void* anchor);
	inline const void* readRCall(const byte* start) {
		return readLERPtr(start + 1, start + 5);
	}
}
enum x86op {
	x86_rcall = 0xE8,
	x86_rjmp = 0xE9,
	x86_break = 0xCC,
	x86_return = 0xC3,
};
inline bool x86_is_eof(const byte op) {
	return op == x86_break;
}
inline bool x86_is_call(const byte op) {
	return op == x86_rcall || op == x86_rjmp;
}
const byte* x86_find_call_pos(const byte* start, size_t max, const void* call);
const void* x86_find_call_after(const byte* start, size_t max, const void* call);
const void* x86_find_call_before(const byte* start, size_t max, const void* call);
const void* x86_find_call_with_count(const byte* start, size_t max, int count);
const void* x86_find_first_call(const byte* start, size_t max);
const void* x86_find_last_call(const byte* start, size_t max);
int x86_count_call(const byte* start, size_t max, const void* call);
#define x86_stepover(p) {\
	auto n = x86size(p);\
	if (n == 0) return x86_dunno;\
	p += n;\
}
#define x86_dunno std::string("Can't parse instruction (see output)")