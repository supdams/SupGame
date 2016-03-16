#ifndef DRAWOBJECT_H
#define DRAWOBJECT_H

#include "common.h"


class Object;
class PseuInstance;

class DrawObject
{
public:
    DrawObject();
    ~DrawObject();
    void Draw(void); // call only in threadsafe environment!! (ensure the obj ptr is still valid!)
    void Unlink(void);

    // additionally, we dont use a GetObject() func - that would fuck things up if the object was already deleted.

private:
    void _Init(void);
    Object *_obj;
    bool _initialized : 1;

    PseuInstance *_instance;


};

#endif
