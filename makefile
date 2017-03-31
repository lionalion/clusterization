BIN_DIR=../bin

CC = g++

all: clust_w2v

clust_w2v : clust_w2v.cpp
	$(CC) clust_w2v.cpp -o ${BIN_DIR}/clust_w2v 

clean:
	pushd ${BIN_DIR} && rm -rf clust_w2v; popd
