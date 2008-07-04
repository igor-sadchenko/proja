#pragma once

class BlobTracker
{
public:
	static int DIST_THRESHOLD;
	static int MOVE_THRESHOLD;
	static int m_GlobalID;

public:
	list<twBlob> currentBlobs;
	list<twBlob> deletedBlobs;

public:
	virtual int UpdateBlobs(list<twBlob> * newBlobs) = 0;
};

class BeltRangeTracker:public BlobTracker
{
public:
	BeltRangeTracker(void);
	~BeltRangeTracker(void);
	int UpdateBlobs(list<twBlob> * newBlobs);
	
};