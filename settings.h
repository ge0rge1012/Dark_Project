#pragma once

class settings 
{
private:
	int width = 640;
	int height = 480;
	int frames_per_second = 60;
public:
	settings();
	void set_width(int new_width);
	void set_height(int new_height);
	int get_width();
	int get_height();
	int get_fps();
};