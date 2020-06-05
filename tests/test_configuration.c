#include "raylib.h"
#include "../src/configuration.h"

void printBar(struct dashboard_element_bar* pPointer) {
	if (pPointer == 0) {
		return;
	}
	printf("\t\t\t\t\t\"bar\": {\n");
	printf("\t\t\t\t\t\t\"min\": %f,\n", pPointer->min);
	printf("\t\t\t\t\t\t\"max\": %f,\n", pPointer->max);
	printf("\t\t\t\t\t\t\"filename\": \"%s\"\n", pPointer->filename);
	printf("\t\t\t\t\t}\n");
}

void printFile(struct dashboard_element_file* pPointer) {
	if (pPointer == 0) {
		return;
	}
	printf("\t\t\t\t\tfile: {\n");
	printf("\t\t\t\t\t\tfilename: \"%s\"\n", pPointer->filename);
	printf("\t\t\t\t\t}\n");
}

void printImage(struct dashboard_element_image* pPointer) {
	if (pPointer == 0) {
		return;
	}
	printf("\t\t\t\t\timage: {\n");
	printf("\t\t\t\t\t\tfilename: \"%s\"\n", pPointer->filename);
	printf("\t\t\t\t\t}\n");
}

void printTime(struct dashboard_element_time* pPointer) {
	if (pPointer == 0) {
		return;
	}
	printf("\t\t\t\t\ttime: {\n");
	printf("\t\t\t\t\t\tformat: \"%s\"\n", pPointer->format);
	printf("\t\t\t\t\t}\n");
}

void printElements(struct dashboard_element* pElements) {
	struct dashboard_element* pPointer = pElements;

	while (pPointer) {
		printf("\t\t\t\t{\n");
		printf("\t\t\t\t\t\"name\": \"%s\",\n", pPointer->name);
		printf("\t\t\t\t\t\"hsize\": \"%s\",\n", pPointer->hsize);
		printf("\t\t\t\t\t\"hposition\": \"%s\",\n", pPointer->hposition);
		printf("\t\t\t\t\t\"hplacement\": \"%s\",\n", pPointer->hplacement);
		printf("\t\t\t\t\t\"vsize\": \"%s\",\n", pPointer->vsize);
		printf("\t\t\t\t\t\"vposition\": \"%s\",\n", pPointer->vposition);
		printf("\t\t\t\t\t\"vplacement\": \"%s\",\n", pPointer->vplacement);
		printBar(pPointer->bar);
		printFile(pPointer->file);
		printImage(pPointer->image);
		printTime(pPointer->time);
		printf("\t\t\t\t}%c\n", (pPointer->next == 0) ? ' ' : ',');
		pPointer = pPointer->next;
	}
}

int main(int argc, char* argv[]) {
	struct dashboard* pDashboards = loadConfiguration(argc, argv);

	if (pDashboards == 0) {
		TraceLog(LOG_ERROR, "No dashboards.");
	}

	struct dashboard* pPointer = pDashboards;

	printf("{\n");
	printf("\t\"width\": %ld,\n", g_width);
	printf("\t\"height\": %ld,\n", g_height);
	printf("\t\"border\": %ld,\n", g_border);
	printf("\t\"dashboards\": [\n");
	do {
		printf("\t\t{\n");
		printf("\t\t\t\"name\": \"%s\",\n", pPointer->name);
		printf("\t\t\t\"elements\": [\n");
		printElements(pPointer->elements);
		printf("\t\t\t]\n");
		printf("\t\t}");
		pPointer = pPointer->next;
		if (pPointer == pDashboards) {
			printf("\n");
		} else {
			printf(",\n");
		}
	} while(pPointer != pDashboards);
	printf("\t]\n");
	printf("}\n");
	return 0;
}
