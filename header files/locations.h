#ifndef LOCATIONS_H
#define LOCATIONS_H

#include"headers.h"

typedef enum
{
    CITY,
    FOREST,
    SEA,
    //etc...
}WorldLocations;

typedef enum
{
    COMMERCIAL_ALLEY,
    RESIDENTIAL_ALLEY,

    //etc...
}CityLocations;

typedef enum
{
    SHOP,    
    //etc...
};

typedef enum
{
    ARENA,
    SHOP,
    DOJO,
    COMMERCIAL_ALLEY,
    
    //etc...
}CityLocations;

typedef struct _location
{
    char *name;     //use strdup()
    char *description;
    NamedNPC *namedNpcs;
    CommonNPC *commonNpcs;
    struct _location *parent;
    struct _location *children[];//realloc se size change karenge.
}Location;

// Function to create a new location node
Location *createLocationNode(char *name,char *desc,NamedNPC *namedNpcs,CommonNPC *commonNpcs,Location *parent);

// Function to add a branch (child node) to a location node
void addBranch(Location *child,Location *parent);

// Function to navigate to a specific location node by name
Location* navigate(Location *current, char *name);

//Any other suitable function
#endif