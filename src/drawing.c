#include "drawing.h"
#include "time.h"

void drawElement(struct dashboard_element* pElement) {
	if (pElement == 0) {
		return;
	}
	if (strcmp((char*)pElement->type, "time") == 0) {
		drawElementTime(pElement);
	} else if (strcmp((char*)pElement->type, "file") == 0) {
		//drawElementFile(pElement);
	} else if (strcmp((char*)pElement->type, "image") == 0) {
		//drawElementImage(pElement);
	} else if (strcmp((char*)pElement->type, "paragraph") == 0) {
		//drawElementParagraph(pElement);
	}
	drawElement(pElement->next);
}

void drawDashboard(struct dashboard* pDashboard) {
	if (pDashboard == 0) {
		return;
	}
	ClearBackground(pDashboard->background);
	drawElement(pDashboard->elements);
}
