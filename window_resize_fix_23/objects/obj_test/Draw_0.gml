draw_set_font(fnt_test);
draw_set_color(c_white);
//show_debug_message("draw")

var enable = window_resize_fix_get_enabled();
var txt = "Currently using ";
if (enable) txt += "fixed "; else txt += "normal ";
txt += "resize#Fixed resize is "
if (!avail) txt += "not ";
txt += "available.#[1] to toggle fixed resize.";
txt = string_hash_to_newline(txt);
draw_text(5, 5, txt);
if (keyboard_check_pressed(ord("1"))) {
    window_resize_fix_set_enabled(!enable);
}