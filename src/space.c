#include <search.h>
#include <regex.h>
#include "configuration.h"

//#define DEBUG_SETCACHEPROPERTY
//#define DEBUG_GETPOSITION

void setCacheProperty(struct dashboard_element* pElement, const char* propertyName, const long value) {
	if (pElement != 0) {
		if (!pElement->_measure) {
			return;
		}
	}

	char elementName[256];

	if (pElement == 0) {
		strcpy(elementName, "screen");
	} else {
		sprintf(elementName, "%s.%s", pElement->_dashboard->name, pElement->name);
	}
#ifdef DEBUG_SETCACHEPROPERTY
	TraceLog(LOG_INFO, "setCacheProperty(\"%s\", \"%s\", %ld)", elementName, propertyName, value);
#endif

	char *buffer = 0;
	ENTRY item;

	buffer = malloc(strlen(elementName) + strlen(propertyName) + 2);
	if (buffer == 0) {
		TraceLog(LOG_ERROR, "setCacheProperty");
	}
	sprintf(buffer, "%s.%s", elementName, propertyName);
	item.key = buffer;
	item.data = (void*)value;
	hsearch(item, ENTER);
#ifdef DEBUG_SETCACHEPROPERTY
	TraceLog(LOG_INFO, "}//setCacheProperty");
#endif
}

bool getCacheProperty(char* object, long* pValue) {
	//TraceLog(LOG_INFO, "getCacheProperty(\"%s\"){", object);
	if (object == 0) {
		//TraceLog(LOG_INFO, "}//getCacheProperty");
		return false;
	}

	ENTRY item;
	ENTRY* pPointer;

	item.key = object;
	pPointer = hsearch(item, FIND);
	if (pPointer == 0) {
		//TraceLog(LOG_INFO, "}//getCacheProperty");
		return false;
	} else {
		*pValue = (long)pPointer->data;
	}
	//TraceLog(LOG_INFO, "}//getCacheProperty");
	return true;
}

bool getCachePropertyFromObject(struct dashboard_element* pElement, const char* propertyName, long* pValue) {
	if (pElement == 0) {
		return false;
	}

	char buffer[256];

	sprintf(buffer, "%s.%s.%s", pElement->_dashboard->name, pElement->name, propertyName);
	return getCacheProperty(buffer, pValue);
}

bool getPercent(char* value, long* pResult) {
	//TraceLog(LOG_INFO, "getPercent(\"%s\"){", value);
	if (value == 0) {
		//TraceLog(LOG_INFO, "}//getPercent");
		return false;
	}

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
	//TraceLog(LOG_INFO, "}//getPercent");
	return found;
}

long getVSize(struct dashboard_element* pElement) {
	long size = 0;

	if (getCacheProperty(pElement->vsize, &size)) {
	} else if (getPercent(pElement->vsize, &size)) {
		long height = 0;

		if (getCacheProperty("screen.height", &height)) {
			size *= height;
			size /= 100;
		}
	} else if (getCacheProperty("screen.height", &size)) {
	} else {
		TraceLog(LOG_ERROR, "getVSize");
	}
	return size;
}

long getHSize(struct dashboard_element* pElement) {
	long size = 0;

	if (getCacheProperty(pElement->hsize, &size)) {
	} else if (getPercent(pElement->hsize, &size)) {
		long width = 0;

		if (getCacheProperty("screen.width", &width)) {
			size *= width;
			size /= 100;
		}
	} else if (getCacheProperty("screen.width", &size)) {
	} else {
		TraceLog(LOG_ERROR, "getHSize");
	}
	return size;
}

void getPosition(struct dashboard_element* pElement, long* pLeft, long* pTop) {
	if (pElement == 0) {
		return;
	}

#ifdef DEBUG_GETPOSITION
	TraceLog(LOG_INFO, "getPosition(\"%s\"){", pElement->name);
#endif
	if (getCacheProperty(pElement->hposition, pLeft)) {
	} else if (getPercent(pElement->hposition, pLeft)) {
		long width = 0;
		long left = 0;

		if (getCacheProperty("screen.width", &width)) {
			*pLeft *= width;
			*pLeft /= 100;
		}
		if (getCacheProperty("screen.left", &left)) {
			*pLeft += left;
		}
	} else {
		if (getCacheProperty("screen.left", pLeft)) {
		}
	}
	if ((pElement->hplacement != 0) && (strcmp(pElement->hplacement, "left") == 0)) {
		long width = 0;

		if (getCachePropertyFromObject(pElement, "width", &width)) {
			*pLeft -= width;
		}
	} else if ((pElement->hplacement != 0) && (strcmp(pElement->hplacement, "center") == 0)) {
		long width = 0;

		if (getCachePropertyFromObject(pElement, "width", &width)) {
			*pLeft += width / 4;
		}
	}
	if (getCacheProperty(pElement->vposition, pTop)) {
		if ((pElement->vplacement != 0) && (strcmp(pElement->vplacement, "top")) == 0) {
			long height = 0;

			if (getCachePropertyFromObject(pElement, "height", &height)) {
				*pTop -= height;
			}
		} else {
		}
	} else if (getPercent(pElement->vposition, pTop)) {
		long height = 0;
		long top = 0;

		if (getCacheProperty("screen.height", &height)) {
			*pTop *= height;
			*pTop /= 100;
		}
		if (getCacheProperty("screen.top", &top)) {
			*pTop += top;
		}
		if ((pElement->vplacement != 0) && (strcmp(pElement->vplacement, "top") == 0)) {
			TraceLog(LOG_ERROR, "getPosition 3");
		} else {
		}
	} else {
		if (getCacheProperty("screen.top", pTop)) {
		}
	}
	if (pElement->_measure) {
		setCacheProperty(pElement, "left", *pLeft);
		setCacheProperty(pElement, "top", *pTop);
	}
#ifdef DEBUG_GETPOSITION
	TraceLog(LOG_INFO, "}//getPosition");
#endif
}
