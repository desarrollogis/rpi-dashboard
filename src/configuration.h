#pragma once
#include <time.h>
#include <yaml.h>
#include "raylib.h"

extern long g_width;
extern long g_height;
extern long g_border;

struct dashboard_element {
	char* name;
	yaml_char_t* type;
	//time type
	char* format;
	//file type
	yaml_char_t* filename;
	time_t modification;
	//image type
	Texture2D texture;
	//image type
	char* hsize;
	char* hposition;
	char* hplacement;
	char* vsize;
	char* vposition;
	char* vplacement;
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
