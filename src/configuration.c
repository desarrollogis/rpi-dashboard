#include <limits.h>
#include <unistd.h>
#include <regex.h>
#include "raylib.h"
#include "configuration.h"

long g_width = 800;
long g_height = 600;
long g_border = 50;
yaml_parser_t parser;
yaml_event_t event;

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

struct dashboard_element_file* parseFile() {
	struct dashboard_element_file* pElement = malloc(sizeof(struct dashboard_element_file));

	if (pElement == 0) {
		return 0;
	}
	pElement->filename = 0;

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
					if (strcmp(value, "filename") == 0) {
						pElement->filename = strdup((char *)event.data.scalar.value);
					} else {
						TraceLog(LOG_ERROR, "parseFile");
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
				TraceLog(LOG_ERROR, "parseFile");
				break;
		}
	}
	return pElement;
}

struct dashboard_element_image* parseImage() {
	struct dashboard_element_image* pElement = malloc(sizeof(struct dashboard_element_image));

	if (pElement == 0) {
		return 0;
	}
	pElement->filename = 0;
	pElement->modification = 0;

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
					if (strcmp(value, "filename") == 0) {
						pElement->filename = strdup((char *)event.data.scalar.value);
					} else {
						TraceLog(LOG_ERROR, "parseImage");
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
				TraceLog(LOG_ERROR, "parseImage");
				break;
		}
	}
	return pElement;
}

struct dashboard_element_time* parseTime() {
	struct dashboard_element_time* pElement = malloc(sizeof(struct dashboard_element_time));

	if (pElement == 0) {
		return 0;
	}
	pElement->format = 0;

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
					if (strcmp(value, "format") == 0) {
						pElement->format = strdup((char *)event.data.scalar.value);
					} else {
						TraceLog(LOG_ERROR, "parseTime");
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
				TraceLog(LOG_ERROR, "parseTime");
				break;
		}
	}
	return pElement;
}

struct dashboard_element* parseDashboardElementsProperties() {
	struct dashboard_element* pElement = malloc(sizeof(struct dashboard_element));

	if (pElement == 0) {
		return 0;
	}
	pElement->name = 0;
	pElement->hsize = 0;
	pElement->hposition = 0;
	pElement->hplacement = 0;
	pElement->vsize = 0;
	pElement->vposition = 0;
	pElement->vplacement = 0;
	pElement->color = BLACK;
	pElement->file = 0;
	pElement->image = 0;
	pElement->time = 0;
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
						pElement->name = strdup((char *)event.data.scalar.value);
					} else if (strcmp(value, "hsize") == 0) {
						pElement->hsize = strdup((char*)event.data.scalar.value);
					} else if (strcmp(value, "hposition") == 0) {
						pElement->hposition = strdup((char*)event.data.scalar.value);
					} else if (strcmp(value, "hplacement") == 0) {
						pElement->hplacement = strdup((char*)event.data.scalar.value);
					} else if (strcmp(value, "vsize") == 0) {
						pElement->vsize = strdup((char*)event.data.scalar.value);
					} else if (strcmp(value, "vposition") == 0) {
						pElement->vposition = strdup((char*)event.data.scalar.value);
					} else if (strcmp(value, "vplacement") == 0) {
						pElement->vplacement = strdup((char*)event.data.scalar.value);
					} else if (strcmp(value, "color") == 0) {
						getElementColor(event.data.scalar.value, pElement);
					} else {
						TraceLog(LOG_ERROR, "parseDashboardElementsProperties");
					}
					isValue = false;
				} else {
					strcpy(value, (char*)event.data.scalar.value);
					isValue = true;
				}
				break;
			case YAML_MAPPING_START_EVENT:
				if (isValue) {
					if (strcmp(value, "file") == 0) {
						pElement->file = parseFile();
					} else if (strcmp(value, "image") == 0) {
						pElement->image = parseImage();
					} else if (strcmp(value, "time") == 0) {
						pElement->time = parseTime();
					}
					isValue = false;
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
						pDashboard->name = strdup((char*)event.data.scalar.value);
					} else if (strcmp(value, "background") == 0) {
						getDashboardBackground(event.data.scalar.value, pDashboard);
					} else {
						TraceLog(LOG_ERROR, "parseDashboardProperties: %s", (char*)event.data.scalar.value);
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
					TraceLog(LOG_ERROR, "parseDashboardProperties 2");
				}
				break;
			case YAML_MAPPING_END_EVENT:
				done = true;
				break;
			default:
				TraceLog(LOG_ERROR, "parseDashboardProperties 3");
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
						sscanf((const char*)event.data.scalar.value, "%ld", &g_width);
					} else if (strcmp(value, "height") == 0) {
						sscanf((const char*)event.data.scalar.value, "%ld", &g_height);
					} else if (strcmp(value, "border") == 0) {
						sscanf((const char*)event.data.scalar.value, "%ld", &g_border);
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
