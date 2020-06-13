#include "drawing.h"
#include "bar.h"
#include "file.h"
#include "image.h"
#include "time.h"

void drawElement(struct dashboard_element* pElement) {
	if (pElement == 0) {
		return;
	}
	if (pElement->bar != 0) {
		drawElementBar(pElement);
	} else if (pElement->file != 0) {
		drawElementFile(pElement);
	} else if (pElement->image != 0) {
		drawElementImage(pElement);
	} else if (pElement->time != 0) {
		drawElementTime(pElement);
	}
	drawElement(pElement->next);
}

void drawDashboard(struct dashboard* pDashboard) {
	if (pDashboard == 0) {
		return;
	}

	struct dashboard_element* pPointer = pDashboard->elements;
	int count = 0;

	while (pPointer != 0) {
		++count;
		pPointer = pPointer->next;
	}
	ClearBackground(pDashboard->background);
	drawElement(pDashboard->elements);
}
