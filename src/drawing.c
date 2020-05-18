#include <search.h>
#include "drawing.h"
#include "time.h"
#include "file.h"
#include "image.h"

void drawElement(struct dashboard_element* pElement) {
	if (pElement == 0) {
		return;
	}
	if (strcmp(pElement->type, "time") == 0) {
		drawElementTime(pElement);
	} else if (strcmp(pElement->type, "file") == 0) {
		drawElementFile(pElement);
	} else if (strcmp(pElement->type, "image") == 0) {
		drawElementImage(pElement);
	} else if (strcmp(pElement->type, "paragraph") == 0) {
		//drawElementParagraph(pElement);
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
