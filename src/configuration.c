#include <limits.h>
#include <unistd.h>
#include <regex.h>
#include "raylib.h"
#include "configuration.h"

int g_width = 800;
int g_height = 600;
int g_border = 50;
yaml_parser_t parser;
yaml_event_t event;

void getWidth(yaml_char_t* value, struct dashboard_element* pElement) {
	regex_t regex;

	if (!regcomp(&regex, "^([[:alnum:]]*)%$", REG_EXTENDED)) {
		regmatch_t groups[2];

		if (!regexec(&regex, (char*)value, 2, groups, 0)) {
			if (groups[1].rm_so != (size_t)(-1)) {
				pElement->width = (yaml_char_t*)strndup((char*)&value[groups[1].rm_so], groups[1].rm_eo - groups[1].rm_so);
				pElement->width_unit = (yaml_char_t*)strdup("percent");
			}
		}
		regfree(&regex);
	}
}

void getHeight(yaml_char_t* value, struct dashboard_element* pElement) {
	regex_t regex;

	if (!regcomp(&regex, "^([[:alnum:]]*)%$", REG_EXTENDED)) {
		regmatch_t groups[2];

		if (!regexec(&regex, (char*)value, 2, groups, 0)) {
			if (groups[1].rm_so != (size_t)(-1)) {
				pElement->height = (yaml_char_t*)strndup((char*)&value[groups[1].rm_so], groups[1].rm_eo - groups[1].rm_so);
				pElement->height_unit = (yaml_char_t*)strdup("percent");
			}
		}
		regfree(&regex);
	}
}

void getLeft(yaml_char_t* value, struct dashboard_element* pElement) {
	regex_t regex;

	if (!regcomp(&regex, "^([[:alnum:]]*)%$", REG_EXTENDED)) {
		regmatch_t groups[2];

		if (!regexec(&regex, (char*)value, 2, groups, 0)) {
			if (groups[1].rm_so != (size_t)(-1)) {
				pElement->left = (yaml_char_t*)strndup((char*)&value[groups[1].rm_so], groups[1].rm_eo - groups[1].rm_so);
				pElement->left_unit = (yaml_char_t*)strdup("percent");
			}
		}
		regfree(&regex);
	}
}

void getTop(yaml_char_t* value, struct dashboard_element* pElement) {
	regex_t regex;

	if (!regcomp(&regex, "^([[:alnum:]]*)%$", REG_EXTENDED)) {
		regmatch_t groups[2];

		if (!regexec(&regex, (char*)value, 2, groups, 0)) {
			if (groups[1].rm_so != (size_t)(-1)) {
				pElement->top = (yaml_char_t*)strndup((char*)&value[groups[1].rm_so], groups[1].rm_eo - groups[1].rm_so);
				pElement->top_unit = (yaml_char_t*)strdup("percent");
			}
		}
		regfree(&regex);
	}
}

void getElementColor(yaml_char_t* value, struct dashboard_element* pElement) {
	if (strcmp((char*)value, "skyblue") == 0) {
		pElement->color = SKYBLUE;
	}
	if (strcmp((char*)value, "black") == 0) {
		pElement->color = BLACK;
	}
	if (strcmp((char*)value, "white") == 0) {
		pElement->color = WHITE;
	}
}

struct dashboard_element* parseDashboardElementsProperties() {
	struct dashboard_element* pElement = malloc(sizeof(struct dashboard_element));

	if (pElement == 0) {
		return 0;
	}
	pElement->name = 0;
	pElement->type = 0;
	pElement->format = 0;
	pElement->filename = 0;
	pElement->modification = 0;
	//pElement->texture = 0;
	pElement->left_absolute = 0;
	pElement->top_absolute = 0;
	pElement->width = 0;
	pElement->width_unit = 0;
	pElement->height = 0;
	pElement->height_unit = 0;
	pElement->left = 0;
	pElement->left_unit = 0;
	pElement->top = 0;
	pElement->top_unit = 0;
	pElement->position = 0;
	pElement->color = BLACK;
	pElement->next = 0;

	bool done = false;
	bool isValue = false;
	char value[256];

	while (!done) {
		if (!yaml_parser_parse(&parser, &event)) {
			break;
		}
		switch (event.type) {
			case YAML_SCALAR_EVENT:
				if (isValue) {
					if (strcmp(value, "name") == 0) {
						pElement->name = (yaml_char_t *)strdup((char *)event.data.scalar.value);
					} else if (strcmp(value, "type") == 0) {
						pElement->type = (yaml_char_t *)strdup((char *)event.data.scalar.value);
					} else if (strcmp(value, "format") == 0) {
						pElement->format = (yaml_char_t *)strdup((char *)event.data.scalar.value);
					} else if (strcmp(value, "width") == 0) {
						getWidth(event.data.scalar.value, pElement);
					} else if (strcmp(value, "height") == 0) {
						getHeight(event.data.scalar.value, pElement);
					} else if (strcmp(value, "left") == 0) {
						getLeft(event.data.scalar.value, pElement);
					} else if (strcmp(value, "top") == 0) {
						getTop(event.data.scalar.value, pElement);
					} else if (strcmp(value, "position") == 0) {
						pElement->position = (yaml_char_t *)strdup((char *)event.data.scalar.value);
					} else if (strcmp(value, "color") == 0) {
						getElementColor(event.data.scalar.value, pElement);
					} else if (strcmp(value, "filename") == 0) {
						pElement->filename = (yaml_char_t *)strdup((char *)event.data.scalar.value);
					} else {
						TraceLog(LOG_ERROR, "parseDashboardElementsProperties");
					}
					isValue = false;
				} else {
					strcpy(value, (char*)event.data.scalar.value);
					isValue = true;
				}
				break;
			case YAML_MAPPING_END_EVENT:
				done = true;
				break;
			default:
				TraceLog(LOG_ERROR, "parseDashboardElementsProperties");
				break;
		}
	}
	return pElement;
}

struct dashboard_element* parseDashboardElements() {
	struct dashboard_element* pList;
	struct dashboard_element* pPointer;
	bool done = false;
	struct dashboard_element* pItem;

	pList = 0;
	pPointer = 0;
	while (!done) {
		if (!yaml_parser_parse(&parser, &event)) {
			break;
		}
		switch (event.type) {
			case YAML_SEQUENCE_END_EVENT:
				done = true;
				break;
			case YAML_MAPPING_START_EVENT:
				pItem = parseDashboardElementsProperties();
				if (pItem == 0) {
					TraceLog(LOG_ERROR, "parseDashboardElements");
				} else {
					if (pPointer == 0) {
						pList = pPointer = pItem;
					} else {
						pPointer->next = pItem;
						pPointer = pItem;
					}
				}
				break;
			default:
				TraceLog(LOG_ERROR, "parseDashboardElements");
				break;
		}
	}
	return pList;
}

void getDashboardBackground(yaml_char_t* value, struct dashboard* pItem) {
	if (strcmp((char*)value, "skyblue") == 0) {
		pItem->background = SKYBLUE;
	}
	if (strcmp((char*)value, "black") == 0) {
		pItem->background = BLACK;
	}
	if (strcmp((char*)value, "white") == 0) {
		pItem->background = WHITE;
	}
}

struct dashboard* parseDashboardProperties() {
	struct dashboard* pDashboard = malloc(sizeof(struct dashboard));

	if (pDashboard == 0) {
		return 0;
	}
	pDashboard->name = 0;
	pDashboard->background = RAYWHITE;
	pDashboard->elements = 0;
	pDashboard->next = 0;

	bool done = false;
	bool isValue = false;
	char value[256];

	while (!done) {
		if (!yaml_parser_parse(&parser, &event)) {
			break;
		}
		switch (event.type) {
			case YAML_SCALAR_EVENT:
				if (isValue) {
					if (strcmp(value, "name") == 0) {
						pDashboard->name = (yaml_char_t*)strdup((char*)event.data.scalar.value);
					} else if (strcmp(value, "background") == 0) {
						getDashboardBackground(event.data.scalar.value, pDashboard);
					} else {
						TraceLog(LOG_ERROR, "parseDashboardProperties");
					}
					isValue = false;
				} else {
					strcpy(value, (char*)event.data.scalar.value);
					isValue = true;
				}
				break;
			case YAML_SEQUENCE_START_EVENT:
				if (strcmp(value, "elements") == 0) {
					pDashboard->elements = parseDashboardElements();
					isValue = false;
				} else {
					TraceLog(LOG_ERROR, "parseDashboardProperties");
				}
				break;
			case YAML_MAPPING_END_EVENT:
				done = true;
				break;
			default:
				TraceLog(LOG_ERROR, "parseDashboardProperties");
				done = true;
				break;
		}
	}
	return pDashboard;
}

struct dashboard* parseDashboards() {
	struct dashboard* pList;
	struct dashboard* pPointer;
	struct dashboard* pItem;
	bool done = false;

	pList = 0;
	pPointer = 0;
	while (!done) {
		if (!yaml_parser_parse(&parser, &event)) {
			break;
		}
		switch (event.type) {
			case YAML_SEQUENCE_END_EVENT:
				done = true;
				break;
			case YAML_MAPPING_START_EVENT:
				pItem = parseDashboardProperties();
				if (pItem == 0) {
					TraceLog(LOG_ERROR, "parseDashboard");
				} else {
					if (pPointer == 0) {
						pList = pPointer = pItem;
					} else {
						pPointer->next = pItem;
						pPointer = pItem;
					}
					pPointer->next = pList;
				}
				break;
			default:
				TraceLog(LOG_ERROR, "parseDashboard");
				break;
		}
	}
	return pList;
}

struct dashboard* parseProperties() {
	struct dashboard* pDashboards;
	bool done = false;
	bool isValue = false;
	char value[256];

	pDashboards = 0;
	while (!done) {
		if (!yaml_parser_parse(&parser, &event)) {
			break;
		}
		switch (event.type) {
			case YAML_SCALAR_EVENT:
				if (isValue) {
					if (strcmp(value, "width") == 0) {
						sscanf((const char*)event.data.scalar.value, "%d", &g_width);
					} else if (strcmp(value, "height") == 0) {
						sscanf((const char*)event.data.scalar.value, "%d", &g_height);
					} else if (strcmp(value, "border") == 0) {
						sscanf((const char*)event.data.scalar.value, "%d", &g_border);
					} else {
						TraceLog(LOG_ERROR, "parseProperties");
					}
					isValue = false;
				} else {
					strcpy(value, (char*)event.data.scalar.value);
					isValue = true;
				}
				break;
			case YAML_SEQUENCE_START_EVENT:
				if (isValue) {
					if (strcmp(value, "dashboards") == 0) {
						pDashboards = parseDashboards();
					} else {
						TraceLog(LOG_ERROR, "parseProperties");
					}
				} else {
					TraceLog(LOG_ERROR, "parseProperties");
				}
				break;
			case YAML_MAPPING_END_EVENT:
				done = true;
				break;
			default:
				TraceLog(LOG_ERROR, "parseProperties");
				break;
		}
	}
	return pDashboards;
}

struct dashboard* parseDocument() {
	struct dashboard* pDashboards;
	bool done = false;

	pDashboards = 0;
	while (!done) {
		if (!yaml_parser_parse(&parser, &event)) {
			break;
		}
		switch (event.type) {
			case YAML_DOCUMENT_END_EVENT:
				done = true;
				break;
			case YAML_MAPPING_START_EVENT:
				pDashboards = parseProperties();
				break;
			default:
				TraceLog(LOG_ERROR, "parseDocument");
				break;
		}
	}
	return pDashboards;
}

struct dashboard* parseStream() {
	struct dashboard* pDashboards;
	bool done = false;

	pDashboards = 0;
	while (!done) {
		if (!yaml_parser_parse(&parser, &event)) {
			break;
		}
		switch (event.type) {
			case YAML_STREAM_END_EVENT:
				done = true;
				break;
			case YAML_DOCUMENT_START_EVENT:
				pDashboards = parseDocument();
				break;
			default:
				TraceLog(LOG_ERROR, "parseStream");
				break;
		}
	}
	return pDashboards;
}

struct dashboard* parseConfiguration(const unsigned char* buffer, int size) {
	struct dashboard* pDashboards;
	bool done = false;

	pDashboards = 0;
	yaml_parser_initialize(&parser);
	yaml_parser_set_input_string(&parser, buffer, size);
	while (!done) {
		if (!yaml_parser_parse(&parser, &event)) {
			break;
		}
		switch (event.type) {
			case YAML_STREAM_START_EVENT:
				pDashboards = parseStream();
				break;
			case YAML_NO_EVENT:
				done = true;
				break;
			default:
				TraceLog(LOG_ERROR, "parseConfiguration");
				break;
		}
	}
	yaml_parser_delete(&parser);
	return pDashboards;
}

struct dashboard* loadConfiguration(int argc, char* argv[]) {
	FILE* input = 0;

	for (int i = 1; i < argc; ++i) {
		if (strncmp("--config=", argv[i], strlen("--config=")) != 0) {
			continue;
		}

		char* buffer = strdup(&(argv[i][9]));

		if (buffer == 0) {
			continue;
		}
		input = fopen(buffer, "r");
		free(buffer);
	}
	if (input == 0) {
		input = fopen("/etc/rpi-dashboard.yaml", "r");
	}
	if (input == 0) {
		return 0;
	}
	fseek(input, 0L, SEEK_END);

	int size = ftell(input);
	yaml_char_t* buffer = malloc(size + 1);

	if (buffer == 0) {
		fclose(input);
		return 0;
	}
	rewind(input);

	int result = fread(buffer, size, 1, input);

	fclose(input);
	if (result < 1) {
		free(buffer);
		return 0;
	}

	struct dashboard* pDashboards = parseConfiguration(buffer, size);

	free(buffer);
	return pDashboards;
}
