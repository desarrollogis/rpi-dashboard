#include <search.h>
#include <regex.h>
#include "configuration.h"

void setCacheProperty(const char* objectName, const char* objectProperty, const long value) {
	ENTRY item;
	char *buffer = 0;

	buffer = malloc(strlen(objectName) + strlen(objectProperty) + 2);
	if (buffer == 0) {
		TraceLog(LOG_ERROR, "setCacheProperty");
	}
	sprintf(buffer, "%s.%s", objectName, objectProperty);
	item.key = buffer;
	item.data = (void*)value;
	hsearch(item, ENTER);
	TraceLog(LOG_INFO, "setCacheProperty: %s.%s = %ld", objectName, objectProperty, value);
}

bool getCacheProperty(char* object, long* pValue) {
	ENTRY item;
	ENTRY* pPointer;

	item.key = object;
	pPointer = hsearch(item, FIND);
	if (pPointer == 0) {
		return false;
	} else {
		*pValue = (long)pPointer->data;
	}
	return true;
}

bool getCachePropertyFromObject(const char* objectName, const char* objectProperty, long* pValue) {
	char *buffer = malloc(strlen(objectName) + strlen(objectProperty) + 2);

	if (buffer == 0) {
		TraceLog(LOG_ERROR, "getCachePropertyFromObject");
	}
	sprintf(buffer, "%s.%s", objectName, objectProperty);
	return getCacheProperty(buffer, pValue);
}

bool getPercent(char* value, long* pResult) {
	regex_t regex;

	if (regcomp(&regex, "^([[:alnum:]]*)%$", REG_EXTENDED) != 0) {
		TraceLog(LOG_ERROR, "getPercent");
	}

	regmatch_t groups[2];
	bool found = false;

	if (regexec(&regex, value, 2, groups, 0) == 0) {
		if (groups[1].rm_so != (size_t)(-1)) {
			char* buffer = strndup(&value[groups[1].rm_so], groups[1].rm_eo - groups[1].rm_so);

			sscanf(buffer, "%ld", pResult);
			found = true;
		}
	}
	regfree(&regex);
	return found;
}

long getVSize(struct dashboard_element* pElement) {
	long size = 0;

	if (getCacheProperty(pElement->vsize, &size)) {
	} else if (getPercent(pElement->vsize, &size)) {
		long height = 0;

		if (getCacheProperty("screen.height", &height)) {
			size = size * height / 100;
		}
	} else {
		TraceLog(LOG_ERROR, "getVSize");
	}
	return size;
}

void getPosition(struct dashboard_element* pElement, long* pLeft, long* pTop) {
	if (getCacheProperty(pElement->hposition, pLeft)) {
		if (strcmp(pElement->hplacement, "left") == 0) {
			long width = 0;

			if (getCachePropertyFromObject(pElement->name, "width", &width)) {
				*pLeft -= width;
			}
		} else {
			TraceLog(LOG_ERROR, "getPosition 1");
		}
	} else if (getPercent(pElement->hposition, pLeft)) {
		long width = 0;
		long left = 0;

		if (getCacheProperty("screen.width", &width)) {
			*pLeft *= width / 100;
		}
		if (getCacheProperty("screen.left", &left)) {
			*pLeft += left;
		}
		if (strcmp(pElement->hplacement, "left") == 0) {
			TraceLog(LOG_ERROR, "getPosition 1a");
		} else {
		}
	} else {
		TraceLog(LOG_ERROR, "getPosition 2");
	}
	if (getCacheProperty(pElement->vposition, pTop)) {
		if (strcmp(pElement->vplacement, "top") == 0) {
			long height = 0;

			if (getCachePropertyFromObject(pElement->name, "height", &height)) {
				*pTop -= height;
			}
		} else {
			//TraceLog(LOG_ERROR, "getPosition 3");
		}
	} else {
		TraceLog(LOG_ERROR, "getPosition 4");
	}
	setCacheProperty(pElement->name, "left", *pLeft);
	setCacheProperty(pElement->name, "top", *pTop);
}

void drawElementTime(struct dashboard_element* pElement) {
	if (pElement == 0) {
		return;
	}
	if (pElement->format == 0) {
		pElement->format = strdup("%H:%M:%S");
		return;
	}

	time_t now = time(0);
	struct tm* ts = localtime(&now);
	char buffer[256];
	int hsize = 0;
	int vsize = 0;
	long left = 0;
	long top = 0;

	strftime(buffer, 256, pElement->format, ts);
	if (pElement->hsize == 0) {
		if (pElement->vsize == 0) {
			TraceLog(LOG_ERROR, "drawElementTime");
		} else {
			vsize = getVSize(pElement);
			hsize = MeasureText(buffer, vsize);

		}
	} else {
		if (pElement->vsize == 0) {
			TraceLog(LOG_ERROR, "drawElementTime");
		} else {
			TraceLog(LOG_ERROR, "drawElementTime");
		}
	}
	setCacheProperty(pElement->name, "width", hsize);
	setCacheProperty(pElement->name, "height", vsize);
	getPosition(pElement, &left, &top);
	while ((vsize > 0) && (MeasureText(buffer, vsize) > hsize)) {
		--vsize;
	}
	DrawText(buffer, left, top, vsize, pElement->color);
}
