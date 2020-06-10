#include "raylib.h"
#include "../src/drawing.h"

int main(int argc, char* argv[]) {
	struct dashboard* pDashboards = loadConfiguration(argc, argv);

	if (pDashboards == 0) {
		TraceLog(LOG_ERROR, "No dashboards.");
	}

	struct dashboard* pPointer = pDashboards;

	InitWindow(g_width, g_height, "dashboard");
	SetConfigFlags(FLAG_VSYNC_HINT);
	//SetTargetFPS(10);
	while (!WindowShouldClose()) {
		BeginDrawing();
		drawDashboard(pPointer);
		EndDrawing();
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			pPointer = pPointer->next;
		}
	}
	CloseWindow();
	return 0;
}
