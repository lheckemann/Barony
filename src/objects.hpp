#pragma once

class Entity; //TODO: Bugger?

void defaultDeconstructor(void* data);
void emptyDeconstructor(void* data);
void entityDeconstructor(void* data);
void lightDeconstructor(void* data);
void mapDeconstructor(void* data);
void stringDeconstructor(void* data);
void listDeconstructor(void* data);
Entity* newEntity(int32_t sprite, uint32_t pos, list_t* entlist);
button_t* newButton(void);
string_t* newString(list_t* list, uint32_t color, char* content, ...);
pathnode_t* newPathnode(list_t* list, int32_t x, int32_t y, pathnode_t* parent, Sint8 pos);
