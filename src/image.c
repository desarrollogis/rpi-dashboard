#include <sys/stat.h>
#include "space.h"

void adjustImage(long width, long height, Image* pImage) {
	long newWidth, newHeight;

	newWidth = width;
	newHeight = (newWidth * pImage->height) / pImage->width;
	if (newHeight > height) {
		newHeight = height;
		newWidth = (newHeight * pImage->width) / pImage->height;
	}
	ImageResize(pImage, newWidth, newHeight);
}

void drawElementImage(struct dashboard_element* pElement) {
	if (pElement == 0) {
		return;
	}
	if (pElement->image == 0) {
		return;
	}
	if (pElement->image->filename == 0) {
		return;
	}

	long width = getHSize(pElement);
	long height = getVSize(pElement);
	long left = 0;
	long top = 0;

	setCacheProperty(pElement->name, "width", width);
	setCacheProperty(pElement->name, "height", height);
	getPosition(pElement, &left, &top);
	setCacheProperty(pElement->name, "right", left + width);
	setCacheProperty(pElement->name, "bottom", top + height);

	struct stat result;

	if (stat(pElement->image->filename, &result) == 0) {
		if (result.st_mtime == pElement->image->modification) {
			left = left + width / 2 - pElement->image->texture.width / 2;
			top = top + height / 2 - pElement->image->texture.height / 2;
			DrawTexture(pElement->image->texture, left, top, WHITE);
			return;
		}
		pElement->image->modification = result.st_mtime;
	}

	Image icon = LoadImage(pElement->image->filename);

	adjustImage(width, height, &icon);
	UnloadTexture(pElement->image->texture);
	pElement->image->texture = LoadTextureFromImage(icon);
	UnloadImage(icon);
	left = left + width / 2 - pElement->image->texture.width / 2;
	top = top + height / 2 - pElement->image->texture.height / 2;
	DrawTexture(pElement->image->texture, left, top, WHITE);
}
