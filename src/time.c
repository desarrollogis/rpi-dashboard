#include "configuration.h"

int g_lastTop = 0;

void drawElementTime(struct dashboard_element* pElement) {
	if (pElement == 0) {
		return;
	}
	if (pElement->format == 0) {
		pElement->format = strdup("%H:%M");
		return;
	}

	time_t now = time(0);
	struct tm* ts = localtime(&now);
	char buffer[256];
	int left = g_border;
	int top = (g_lastTop == 0) ? g_border : g_lastTop;
	int width = g_width - g_border * 2;
	int height = g_height - g_border * 2;

	strftime(buffer, 256, pElement->format, ts);
	if (pElement->width != 0) {
		if (strcmp((char*)pElement->width_unit, "percent") == 0) {
			sscanf((char*)pElement->width, "%d", &width);
			width = width * (g_width - g_border * 2) / 100;
		}
	}
	if (pElement->height != 0) {
		if (strcmp((char*)pElement->height_unit, "percent") == 0) {
			sscanf((char*)pElement->height, "%d", &height);
			height = height * (g_height - g_border * 2) / 100;
		}
	}
	if (pElement->left != 0) {
		if (strcmp((char*)pElement->left_unit, "percent") == 0) {
			sscanf((char*)pElement->left, "%d", &left);
			left = g_border + left * (g_width - g_border * 2) / 100;
		}
	}
	if (pElement->top != 0) {
		if (strcmp((char*)pElement->top_unit, "percent") == 0) {
			sscanf((char*)pElement->top, "%d", &top);
			top = g_border + top * (g_height - g_border * 2) / 100;
		}
	}
	while ((height > 0) && (MeasureText(buffer, height) > width)) {
		--height;
	}
	if (pElement->width == 0) {
		width = MeasureText(buffer, height);
	}
	DrawText(buffer, left + width / 2 - MeasureText(buffer, height) / 2, top, height, pElement->color);
	//g_lastTop = top + height;
}
