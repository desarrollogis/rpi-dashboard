#pragma once
#include "configuration.h"

void setCacheProperty(struct dashboard_element* pElement, const char* objectProperty, const long value);
long getHSize(struct dashboard_element* pElement);
long getVSize(struct dashboard_element* pElement);
void getPosition(struct dashboard_element* pElement, long* pLeft, long* pTop);
