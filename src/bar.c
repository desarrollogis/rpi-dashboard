#include "space.h"

float min(float a, float b) {
	if (a < b) {
		return a;
	}
	return b;
}

float max(float a, float b) {
	if (a > b) {
		return a;
	}
	return b;
}

void drawElementBar(struct dashboard_element* pElement) {
	if (pElement == 0) {
		return;
	}
	if (pElement->bar == 0) {
		return;
	}
	if (pElement->bar->min == pElement->bar->max) {
		return;
	}

	FILE* input = fopen(pElement->bar->filename, "r");

	if (input == 0) {
		TraceLog(LOG_INFO, "drawElementBar ERROR filename: %s", pElement->bar->filename);
		return;
	}

	char buffer[256];

	buffer[0] = 0;
	if (fscanf(input, "%s", buffer) < 1) {
		TraceLog(LOG_INFO, "drawElementBar ERROR value");
		fclose(input);
		return;
	}
	fclose(input);

	float value = 0.0;

	if (sscanf(buffer, "%f", &value) < 1) {
		TraceLog(LOG_INFO, "drawElementBar ERROR value");
		return;
	}
	value = max(value, pElement->bar->min);
	value = min(value, pElement->bar->max);
	value = (value - pElement->bar->min) / (pElement->bar->max - pElement->bar->min);

	long hsize = 0;
	long vsize = 0;
	long left = 0;
	long top = 0;
	long position;
	Color color = BLACK;

	hsize = getHSize(pElement);
	vsize = getVSize(pElement);
	setCacheProperty(pElement, "width", hsize);
	setCacheProperty(pElement, "height", vsize);
	getPosition(pElement, &left, &top);
	setCacheProperty(pElement, "right", left + hsize);
	setCacheProperty(pElement, "bottom", top + vsize);
	position = vsize * value;
	if (pElement->bar->_value < position) {
		if ((position - pElement->bar->_value) < 10) {
			++pElement->bar->_value;
		} else {
			pElement->bar->_value += 10;
		}
	} else if (pElement->bar->_value > position) {
		if ((pElement->bar->_value - position) < 10) {
			--pElement->bar->_value;
		} else {
			pElement->bar->_value -= 10;
		}
	}

	float percent = (float)pElement->bar->_value / (float)vsize;

	if (percent <= 0.2) {
		color = GREEN;
	} else if (percent <= 0.4) {
		color = BLUE;
	} else if (percent <= 0.6) {
		color = YELLOW;
	} else if (percent <= 0.8) {
		color = ORANGE;
	} else if (percent <= 1.0) {
		color = RED;
	}
	DrawRectangle(left, top + (vsize - pElement->bar->_value), hsize, pElement->bar->_value, color);
	pElement->_measure = false;
}
