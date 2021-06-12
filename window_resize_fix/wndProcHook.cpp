#include "stdafx.h"
extern int32_t* g_doReset;
extern GR_D3D_Reset_t* GR_D3D_Reset;
extern bool isGMS2;

#define RECTW(r) (r.right - r.left)
#define RECTH(r) (r.bottom - r.top)
#define RECTP(r) r.left, r.top, r.right - r.left, r.bottom - r.top

void draw_enable_drawevent(bool enable);

///
enum class window_resize_fix_mode {
	copy,
	stretch_aspect,
	stretch_full,
};
struct {
	bool enable = true;
	window_resize_fix_mode mode = window_resize_fix_mode::stretch_aspect;
	double width = 4;
	double height = 3;
	HBRUSH background = NULL;
} config;

struct {
	HBITMAP bitmap = NULL;
	bool sizing = false;
	HDC bitHDC = NULL;
	RECT bitRect{};
	bool justResized = false;
} static state;

WNDPROC window_command_proc_base = nullptr;
LRESULT window_command_proc_hook(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	//printf("msg=%d\n", msg); fflush(stdout);
	if (config.background == NULL) {
		config.background = CreateSolidBrush(RGB(0, 0, 0));
	}
	switch (msg) {
		case WM_SIZING: {
			if (!state.sizing && config.enable) {
				static HDC screen = nullptr;
				if (!screen) {
					screen = GetDC(NULL);
					state.bitHDC = CreateCompatibleDC(screen);
				}
				if (state.bitmap != NULL) {
					DeleteObject(state.bitmap);
					state.bitmap = NULL;
				}
				GetClientRect(hwnd, &state.bitRect);
				if (config.mode == window_resize_fix_mode::stretch_aspect) {
					auto wndW = RECTW(state.bitRect);
					auto wndH = RECTH(state.bitRect);
					auto scrW = config.width;
					auto scrH = config.height;
					auto scale = min((double)wndW / scrW, (double)wndH / scrH);
					auto dstW = (int)(scrW * scale);
					auto dstH = (int)(scrH * scale);
					auto dstX = (wndW - dstW) / 2;
					auto dstY = (wndH - dstH) / 2;
					state.bitRect.left += dstX;
					state.bitRect.top += dstY;
					state.bitRect.right = state.bitRect.left + dstW;
					state.bitRect.bottom = state.bitRect.top + dstH;

					//trace("wnd=(%d, %d) scr=(%d, %d)", wndW, wndH, (int)scrW, (int)scrH);
					//trace("dst=(%d, %d, %d, %d)", dstX, dstY, dstW, dstH)
					//trace("state.bitRect=(%d, %d, %d, %d)", RECTP(state.bitRect));
				}
				state.bitmap = CreateCompatibleBitmap(screen, RECTW(state.bitRect), RECTH(state.bitRect));
				SelectObject(state.bitHDC, state.bitmap);
				FillRect(state.bitHDC, &state.bitRect, config.background);
				//PrintWindow(hwnd, lastBitHDC, 0);
				POINT windowPos{ 0, 0 };
				ClientToScreen(hwnd, &windowPos);
				BitBlt(state.bitHDC, 0, 0, RECTW(state.bitRect), RECTH(state.bitRect),
					screen, windowPos.x + state.bitRect.left, windowPos.y + state.bitRect.top, SRCCOPY);
				//
				state.bitRect.right -= state.bitRect.left;
				state.bitRect.bottom -= state.bitRect.top;
				state.bitRect.left = 0;
				state.bitRect.top = 0;
				state.sizing = true;
				//trace("resize start");
			}
			break;
		};
		case WM_EXITSIZEMOVE: {
			state.sizing = false;
			state.justResized = true;
			if (isGMS2 && config.enable) GR_D3D_Reset();
			//draw_enable_drawevent(false);
			//trace("resize end");
			break;
		};
		case WM_ERASEBKGND: {
			if (config.enable && state.sizing && state.bitmap) {
				RECT cr; GetClientRect(hwnd, &cr);
				if (0) trace("resize draw %d,%d %d;%d %d,%d %d;%d",
					cr.left, cr.top, RECTW(cr), RECTH(cr),
					state.bitRect.left, state.bitRect.top, RECTW(state.bitRect), RECTH(state.bitRect));
				auto hdc = (HDC)wParam;
				auto mode = config.mode;
				auto crW = RECTW(cr);
				auto crH = RECTH(cr);
				auto scrW = RECTW(state.bitRect);
				auto scrH = RECTH(state.bitRect);
				auto scrX = state.bitRect.left;
				auto scrY = state.bitRect.top;
				FillRect(hdc, &cr, config.background);
				if (mode == window_resize_fix_mode::stretch_aspect) {
					auto scale = min((double)crW / scrW, (double)crH / scrH);
					auto dstW = (int)(scale * scrW);
					auto dstH = (int)(scale * scrH);
					auto dstX = (crW - dstW) / 2;
					auto dstY = (crH - dstH) / 2;
					if (0) trace("src=(%d, %d, %d, %d) cr=(%d, %d) dst=(%d, %d, %d, %d)",
						scrX, scrY, scrW, scrH, crW, crH,
						dstX, dstY, dstW, dstH);

					StretchBlt(hdc, dstX, dstY, dstW, dstH,
						state.bitHDC, scrX, scrY, scrW, scrH,
						SRCCOPY);
				} else if (mode == window_resize_fix_mode::stretch_full) {
					StretchBlt(hdc, 0, 0, crW, crH,
						state.bitHDC, scrX, scrY, scrW, scrH,
						SRCCOPY);
				} else {
					BitBlt(hdc, 0, 0, scrW, scrH, state.bitHDC, scrX, scrY, SRCCOPY);
				}
				return TRUE;
			}
			/*
			HDC hdc = (HDC)wParam;
			static HBRUSH background = CreateSolidBrush(RGB(80, 80, 80));
			RECT cr; GetClientRect(hwnd, &cr);
			SelectObject(hdc, background);
			FillRect(hdc, &cr, background);
			return TRUE;
			//*/
			break;
		};
	}
	auto result = CallWindowProc(window_command_proc_base, hwnd, msg, wParam, lParam);
	if (msg == WM_EXITSIZEMOVE && config.enable) {
		//trace("Resize end");
		if (*g_doReset > 1) *g_doReset = 1;
	}
	return result;
}

void window_command_proc_init(HWND hwnd) {
	window_command_proc_base = (WNDPROC)SetWindowLongPtr(hwnd, GWL_WNDPROC, (LONG_PTR)window_command_proc_hook);
}

/// ->bool
dllx double window_resize_fix_get_just_resized() {
	auto result = state.justResized;
	state.justResized = false;
	return result;
}

/// ->bool
dllx double window_resize_fix_get_enabled() {
	return config.enable && g_doReset != nullptr;
}

/// (enable:bool)
dllx double window_resize_fix_set_enabled(double enable) {
	config.enable = enable > 0.5;
	return 1;
}

/// (width:number, height:number)
dllx double window_resize_fix_set_aspect_ratio(double width, double height) {
	config.width = width;
	config.height = height;
	return 1;
}

///
dllx double window_resize_fix_set_mode(double mode) {
	config.mode = (window_resize_fix_mode)(int)mode;
	return 1;
}