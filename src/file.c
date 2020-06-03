#include "space.h"

//#define DEBUG_DRAWELEMENTFILE

void drawElementFile(struct dashboard_element* pElement) {
#ifdef DEBUG_DRAWELEMENTFILE
	TraceLog(LOG_INFO, "drawElementFile(\"%s\"){", pElement->name);
#endif
	if (pElement == 0) {
		return;
	}
	if (pElement->file == 0) {
		return;
	}
	if (pElement->file->filename == 0) {
		return;
	}

	FILE* input = fopen(pElement->file->filename, "r");

	if (input == 0) {
		TraceLog(LOG_INFO, "drawElementFile ERROR: %s", pElement->file->filename);
		return;
	}

	char buffer[256];
	long width = 0;
	long height = 0;
	long left = 0;
	long top = 0;

	fscanf(input, "%s", buffer);
	fclose(input);
	if (pElement->hsize == 0) {
		if (pElement->vsize == 0) {
			TraceLog(LOG_ERROR, "drawElementFile");
		} else {
			height = getVSize(pElement);
			width = MeasureText(buffer, height);
		}
	} else {
		if (pElement->vsize == 0) {
			width = getHSize(pElement);
			while (MeasureText(buffer, height) < width) {
				++height;
			}
		} else {
			width = getHSize(pElement);
			height = getVSize(pElement);
		}
	}
	setCacheProperty(pElement->name, "width", width);
	setCacheProperty(pElement->name, "height", height);
	getPosition(pElement, &left, &top);
	setCacheProperty(pElement->name, "right", left + width);
	setCacheProperty(pElement->name, "bottom", top + height);
	while ((height > 0) && (MeasureText(buffer, height) > width)) {
		--height;
	}
	DrawText(buffer, left, top, height, pElement->color);
}
