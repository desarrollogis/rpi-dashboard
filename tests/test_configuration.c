#include "raylib.h"
#include "../src/configuration.h"

const int g_screenWidth = 0;
const int g_screenHeight = 0;

extern int g_width;
extern int g_height;
extern int g_border;

int main(int argc, char* argv[]) {
	struct dashboard* pDashboards = loadConfiguration(argc, argv);

	if (pDashboards == 0) {
		TraceLog(LOG_ERROR, "No dashboards.");
	}

	struct dashboard* pPointer = pDashboards;

	printf("{\n");
	printf("\twidth: %d,\n", g_width);
	printf("\theight: %d,\n", g_height);
	printf("\tborder: %d,\n", g_border);
	printf("\tdashboards: [\n");
	do {
		printf("\t\t{\n");
		printf("\t\t\tname: \"%s\",\n", pPointer->name);
		printf("\t\t}");
		pPointer = pPointer->next;
		if (pPointer == pDashboards) {
			printf("\n");
		} else {
			printf(",\n");
		}
	} while(pPointer != pDashboards);
	printf("\t]\n");
	printf("}\n");
	return 0;
}
