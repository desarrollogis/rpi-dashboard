#include "drawing.h"

void drawDashboard(struct dashboard* pDashboard) {
	if (pDashboard == 0) {
		return;
	}
	ClearBackground(pDashboard->background);
	//drawElement(pDashboard->elements);
}
