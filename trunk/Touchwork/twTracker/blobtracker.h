#pragma once
#include "Point.h"
#include "Blob.h"
#include <list>
using namespace std;

class BlobTracker
{
private:
	static int DIST_THRESHOLD;
	static int MOVE_THRESHOLD;
	static int m_GlobalID;

public:
	list<Blob> currentBlobs;
	list<Blob> deletedBlobs;

public:
	BlobTracker(void);
	~BlobTracker(void);
	int UpdateBlobs(list<Blob> * newBlobs);
};