#ifndef HashMap_h
#define HashMap_h

typedef struct HashMap HashMap;

HashMap * createMap(long capacity);

void insertMap(HashMap * table, void * key, void * value);

void eraseMap(HashMap * table, void * key);

void * searchMap(HashMap * table, void * key);

void * firstMap(HashMap * table);

void * nextMap(HashMap * table);

void enlarge(HashMap * map);

long size(HashMap * map);

long capacity(HashMap * map);
#endif /* HashMap_h */
