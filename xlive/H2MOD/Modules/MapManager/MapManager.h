#pragma once

#include <set>

/**
* Class used for downloading maps (for peers) and hosting map downloads (for peer host and dedi host)
* NOTE - there are g_debug conditions in front of every trace call, this is because I want to avoid any string creation (since its expensive)
* during game time, unless g_debug is on.
*/
class MapManager {
public:
	//client and server functions below
	void reloadAllMaps();
	bool loadMapInfo(std::wstring& mapFileLocation);
	std::wstring getMapName();

	//client functions/data below
	void applyHooks();
	std::string getMapFilenameToDownload();
	void clearMapFileNameToDownload();
	void setMapFileNameToDownload(std::string mapFilenameToDownload);
	void setMapFileNameToDownload(std::wstring mapFilenameToDownload);
	bool downloadFromRepo(std::string mapFilename);
	std::wstring clientMapFilename;

	//server functions below
	void getMapFilename(std::wstring& buffer);

private:

	bool requestMapUrl = false;
	std::string mapFilenameToDownload;
};

extern MapManager* mapManager;