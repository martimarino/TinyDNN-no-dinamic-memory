#!/usr/bin/env python3
import pickle
import sys
import struct
# Note that this will work with Python3
def unpickle(file):
    with open(file, 'rb') as fo:
        dict = pickle.load(fo)
    return dict

def dumpData(unpickled,file):
	data = unpickled['data']
	labels = unpickled['labels']
	with open(file,'wb') as fo:
		for i in range(0,len(labels)):
			fo.write(struct.pack('h',labels[i]))
			data[i].tofile(fo)

dumpData(unpickle(sys.argv[1]),"data.bin");
