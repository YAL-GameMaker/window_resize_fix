/// @author YellowAfterlife

#include "stdafx.h"
#include "x86size.h"

bool isGMS2;

struct GMLClosure {
	int __unknown[26];
	const byte* impl;
};

#if 0
struct GMLVar {
	union {
		int32_t vInt32;
		int64_t vInt64;
		double vReal;
	};
	int32_t flags = 0;
	int32_t type;
};
#define __GMLFuncArgs__ GMLVar& result, void* self, void* other, int argc, GMLVar* argv
#define __GMLFuncRest__ result, self, other, argc, argv
typedef void(GmlFunc)(__GMLFuncArgs__);

GmlFunc* f_draw_enable_drawevent = nullptr;
void draw_enable_drawevent(bool enable) {
	if (!f_draw_enable_drawevent) return;
	GMLVar result, arg;
	result.type = 0;
	result.vReal = 0;
	arg.type = 0;
	arg.vReal = enable ? 1 : 0;
	f_draw_enable_drawevent(result, nullptr, nullptr, 1, &arg);
}
#endif

int32_t* g_doReset = 0;

GR_D3D_Reset_t* GR_D3D_Reset = nullptr;
dllx double window_resize_fix_reset() {
	if (GR_D3D_Reset) {
		GR_D3D_Reset();
		return true;
	} else return false;
}

void window_command_proc_init(HWND hwnd);

dllx double window_resize_fix_init_raw(
	GMLClosure* _window_set_fullscreen,
	GMLClosure* _display_set_size,
	void* _hwnd,
	double v2
) {
	isGMS2 = v2 > 1.5;
	if (g_doReset != nullptr) return true;
	if (_window_set_fullscreen == nullptr) return false;
	//f_draw_enable_drawevent = (GmlFunc*)_draw_enable_drawevent->impl;
	auto _ptr = _window_set_fullscreen->impl;
	auto impl = (const byte*)x86_find_last_call(_ptr, 128);
	auto at = impl;
	auto lastAt = at;
	while (at < impl + 128) {
		if (x86_is_eof(*at)) break;
		if (*at == x86_return) break;
		lastAt = at;
		auto atn = x86size(at);
		if (atn == 0) return false;
		at += atn;
	}
	g_doReset = (int32_t*)MemTools::readLEPtr(lastAt + 2);
	trace("Reset counter located at %llx", (int64_t)g_doReset);
	//
	GR_D3D_Reset = (GR_D3D_Reset_t*)x86_find_last_call(_display_set_size->impl, 256);
	trace("D3D reset located at %llx", (int64_t)GR_D3D_Reset);
	//
	window_command_proc_init((HWND)_hwnd);
	return true;
}