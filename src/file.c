#include "space.h"

#define LINE_MAX 1024

void drawElementFile(struct dashboard_element* pElement) {
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

	char buffer[LINE_MAX];
	long width = 0;
	long height = 0;
	long left = 0;
	long top = 0;
	long oldTop;
	long oldHeight;

	buffer[0] = 0;
	if (fgets(buffer, sizeof buffer, input) == 0) {
		return;
	}
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
	oldTop = top;
	oldHeight = height;
	while ((height > 0) && (MeasureText(buffer, height) > width)) {
		--height;
	}
	DrawText(buffer, left, top, height, pElement->color);
	while (fgets(buffer, sizeof buffer, input) != 0) {
		top += height;
		height = oldHeight;
		while ((height > 0) && (MeasureText(buffer, height) > width)) {
			--height;
		}
		if ((top + height) > (oldTop + oldHeight)) {
			break;
		}
		DrawText(buffer, left, top, height, pElement->color);
	}
	fclose(input);
}
