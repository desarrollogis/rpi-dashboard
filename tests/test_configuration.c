#include "raylib.h"
#include "../src/configuration.h"

const int g_screenWidth = 0;
const int g_screenHeight = 0;
const int g_border = 0;

int main(int argc, char* argv[]) {
	struct dashboard* pDashboards = loadConfiguration(argc, argv);

	if (pDashboards == 0) {
		TraceLog(LOG_ERROR, "No dashboards.");
	}
	return 0;
}
