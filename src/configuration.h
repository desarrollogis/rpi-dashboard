#pragma once
#include <time.h>
#include <yaml.h>
#include "raylib.h"

extern int g_width;
extern int g_height;
extern int g_border;

struct dashboard_element {
	yaml_char_t* name;
	yaml_char_t* type;
	//time type
	char* format;
	//file type
	yaml_char_t* filename;
	time_t modification;
	//image type
	Texture2D texture;
	int left_absolute;
	int top_absolute;
	//image type
	yaml_char_t* width;
	yaml_char_t* width_unit;
	yaml_char_t* height;
	yaml_char_t* height_unit;
	yaml_char_t* left;
	yaml_char_t* left_unit;
	yaml_char_t* top;
	yaml_char_t* top_unit;
	yaml_char_t* position;
	Color color;
	struct dashboard_element* next;
};

struct dashboard {
	yaml_char_t* name;
	Color background;
	struct dashboard_element* elements;
	struct dashboard* next;
};

struct dashboard* loadConfiguration(int argc, char* argv[]);
