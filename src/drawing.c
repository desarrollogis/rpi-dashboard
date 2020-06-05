#include <search.h>
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
	hcreate((count + 1) * 6);

	ENTRY item;

	item.key = strdup("screen.width");
	item.data = (void*)(g_width - g_border * 2);
	hsearch(item, ENTER);
	item.key = strdup("screen.height");
	item.data = (void*)(g_height - g_border * 2);
	hsearch(item, ENTER);
	item.key = strdup("screen.left");
	item.data = (void*)g_border;
	hsearch(item, ENTER);
	item.key = strdup("screen.top");
	item.data = (void*)g_border;
	hsearch(item, ENTER);
	item.key = strdup("screen.right");
	item.data = (void*)(g_width - g_border);
	hsearch(item, ENTER);
	item.key = strdup("screen.bottom");
	item.data = (void*)(g_height - g_border);
	hsearch(item, ENTER);
	ClearBackground(pDashboard->background);
	drawElement(pDashboard->elements);
	hdestroy();
	//TraceLog(LOG_ERROR, "drawDashboard");
}
