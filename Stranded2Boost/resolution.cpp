#include "resolution.h"
#include <windows.h>
#include <vector>
#include <string>
#include <algorithm>

struct Resolution {
	std::string name;
	int width, height;
	
	bool operator<(Resolution &other){
		return ((this->width < other.width) ? (this->width < other.width) : (this->height < other.height));
	}
	bool operator==(Resolution &other) {
		return ((this->width == other.width) && (this->height == other.height));
	}
};

static std::vector<Resolution> resolutions;

void ResolutionsInit()
{
	DEVMODE dm = { 0 };
	dm.dmSize = sizeof(dm);
	int iModeNum;
	Resolution tmp, *p;
	for (iModeNum = 0; EnumDisplaySettings(NULL, iModeNum, &dm) != 0; iModeNum++) {
		resolutions.push_back(tmp);
		p = &resolutions.back();
		p->width = static_cast<int>(dm.dmPelsWidth);
		p->height = static_cast<int>(dm.dmPelsHeight);
		p->name = std::to_string(p->width) + 'x' + std::to_string(p->height);

	}
	std::sort(resolutions.begin(), resolutions.end());
	resolutions.erase(std::unique(resolutions.begin(), resolutions.end()),resolutions.end());
}

// returns available resolution count - 1
BBDECL int BBCALL GetResolutionMaxId() {
	return resolutions.size() ? resolutions.size() - 1 : 0;
}
//returns id of resolution with specific width
BBDECL int BBCALL GetResolutionId(int width) {
	for (unsigned int i = 0; i < resolutions.size(); i++) {
		if (resolutions[i].width == width) {
			return static_cast<int>(i);
		}
	}
	return 0;
}
//returns name of resolution with specific id
BBDECL const char* BBCALL GetResolutionName(int id) {
	if ((id >= 0) && (id < static_cast<int>(resolutions.size()))) {
		return resolutions[id].name.c_str();
	}
	return "";
}
//returns width of resolution with specific id
BBDECL int BBCALL GetResolutionX(int id) {
	if ((id >= 0) && (id < static_cast<int>(resolutions.size()))) {
		return resolutions[id].width;
	}
	return -1;
}
//returns height of resolution with specific id
BBDECL int BBCALL GetResolutionY(int id) {
	if ((id >= 0) && (id < static_cast<int>(resolutions.size()))) {
		return resolutions[id].height;
	}
	return -1;
}