#ifndef JSONREADER_H
#define JSONREADER_H
#include "joint.h"

class JSONReader
{
public:
    JSONReader();
    bool ParseToSkeleton(QString iFileName, Skeleton& iSkeleton);
protected:

};

#endif // JSONREADER_H
