#pragma once

class BlobTracker
{
public:
	static int DIST_THRESHOLD;
	static int MOVE_THRESHOLD;
	static int m_GlobalID;

public:
	list<Blob> currentBlobs;
	list<Blob> deletedBlobs;

public:
	virtual int UpdateBlobs(list<Blob> * newBlobs) = 0;
};

class RangeBlobTracker:public BlobTracker
{
public:
	RangeBlobTracker(void);
	~RangeBlobTracker(void);
	int UpdateBlobs(list<Blob> * newBlobs);
};