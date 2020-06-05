#pragma once
#include <time.h>
#include <yaml.h>
#include "raylib.h"

extern long g_width;
extern long g_height;
extern long g_border;

struct dashboard_element_bar {
	float min;
	float max;
	char* filename;
	long _value;
};

struct dashboard_element_file {
	char* filename;
};

struct dashboard_element_image {
	char* filename;
	time_t modification;
	Texture2D texture;
};

struct dashboard_element_time {
	char* format;
};

struct dashboard_element {
	char* name;
	char* hsize;
	char* hposition;
	char* hplacement;
	char* vsize;
	char* vposition;
	char* vplacement;
	Color color;
	struct dashboard_element_bar* bar;
	struct dashboard_element_file* file;
	struct dashboard_element_image* image;
	struct dashboard_element_time* time;
	struct dashboard_element* next;
};

struct dashboard {
	char* name;
	Color background;
	struct dashboard_element* elements;
	struct dashboard* next;
};

struct dashboard* loadConfiguration(int argc, char* argv[]);
