#include "settings.h"

settings::settings() {}
void settings::set_height(int new_height) {}
void settings::set_width(int new_width) {}
int settings::get_width() {
	return width;
}
int settings::get_height() {
	return height;
}
int settings::get_fps() {
	return frames_per_second;
}
