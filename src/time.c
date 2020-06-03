#include "space.h"

void drawElementTime(struct dashboard_element* pElement) {
	if (pElement == 0) {
		return;
	}
	if (pElement->time == 0) {
		return;
	}
	if (pElement->time->format == 0) {
		pElement->time->format = strdup("%H:%M:%S");
	}

	time_t now = time(0);
	struct tm* ts = localtime(&now);
	char buffer[256];
	long hsize = 0;
	long vsize = 0;
	long left = 0;
	long top = 0;

	strftime(buffer, 256, pElement->time->format, ts);
	if (pElement->hsize == 0) {
		if (pElement->vsize == 0) {
			TraceLog(LOG_ERROR, "drawElementTime 1");
		} else {
			vsize = getVSize(pElement);
			hsize = MeasureText(buffer, vsize);
		}
	} else {
		if (pElement->vsize == 0) {
			hsize = getHSize(pElement);
			while (MeasureText(buffer, vsize) < hsize) {
				++vsize;
			}
		} else {
			hsize = getHSize(pElement);
			vsize = getVSize(pElement);
		}
	}
	setCacheProperty(pElement->name, "width", hsize);
	setCacheProperty(pElement->name, "height", vsize);
	getPosition(pElement, &left, &top);
	setCacheProperty(pElement->name, "right", left + hsize);
	setCacheProperty(pElement->name, "bottom", top + vsize);
	while ((vsize > 0) && (MeasureText(buffer, vsize) > hsize)) {
		--vsize;
	}
	DrawText(buffer, left, top, vsize, pElement->color);
}
