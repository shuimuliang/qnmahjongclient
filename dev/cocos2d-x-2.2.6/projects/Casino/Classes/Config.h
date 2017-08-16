#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <map>
#include "../CCEGLViewProtocol.h"

extern std::string DOWNLOAD_ROOT;
extern std::string CONFIG_JSON;
extern std::string SDCARD_PATH;
extern std::string DOWNLOAD_DIR; // note: this directory need to be modified with a big version release

// read from json
extern unsigned int INNER_VERSION;
extern std::string SHOW_VERSION;
extern std::string VERSION_SERVER_IP;
extern std::string RESOUCE_JSON;

// init paths and read config
void readConfig();

#endif
