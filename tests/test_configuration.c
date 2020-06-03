#include "raylib.h"
#include "../src/configuration.h"

void printImage(struct dashboard_element_image* pPointer) {
	if (pPointer == 0) {
		return;
	}
	printf("\t\t\t\t\timage: {\n");
	printf("\t\t\t\t\t\tfilename: \"%s\"\n", pPointer->filename);
	printf("\t\t\t\t\t}\n");
}

void printElements(struct dashboard_element* pElements) {
	struct dashboard_element* pPointer = pElements;

	while (pPointer) {
		printf("\t\t\t\t{\n");
		printf("\t\t\t\t\tname: \"%s\",\n", pPointer->name);
		printf("\t\t\t\t\thsize: \"%s\",\n", pPointer->hsize);
		printf("\t\t\t\t\thposition: \"%s\",\n", pPointer->hposition);
		printf("\t\t\t\t\thplacement: \"%s\",\n", pPointer->hplacement);
		printf("\t\t\t\t\tvsize: \"%s\",\n", pPointer->vsize);
		printf("\t\t\t\t\tvposition: \"%s\",\n", pPointer->vposition);
		printf("\t\t\t\t\tvplacement: \"%s\",\n", pPointer->vplacement);
		printImage(pPointer->image);
		printf("\t\t\t\t}%c\n", (pPointer->next == 0) ? '\0' : ',');
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
	printf("\twidth: %ld,\n", g_width);
	printf("\theight: %ld,\n", g_height);
	printf("\tborder: %ld,\n", g_border);
	printf("\tdashboards: [\n");
	do {
		printf("\t\t{\n");
		printf("\t\t\tname: \"%s\",\n", pPointer->name);
		printf("\t\t\telements: [\n");
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
