#pragma once

void glLoadTexture(SDL_Surface* image, int texnum);
SDL_Surface* loadImage(char* filename);
voxel_t* loadVoxel(char* filename2);
int loadMap(char* filename, map_t* destmap, list_t* entlist);
int loadConfig(char* filename);
int saveMap(char* filename);
char* readFile(char* filename);
std::list<std::string> directoryContents(const char* directory);
FILE *openDataFile(const char *const filename, const char * const mode);
DIR * openDataDir(const char *const);
bool dataPathExists(const char *const);
bool completeDataPath(char *dest, const char * const path);
extern char datadir[1024];
