DATA_DIR=txt
BIN_DIR=../bin
MD_DIR=../mod

FILE=yandex.news

if [ ! -e $BIN_DIR ]; then
	make
fi

$BIN_DIR/clust_w2v -fl $DATA_DIR/$FILE -w2v $MD_DIR/rdt.w2v  -th2 0.25 -th3 4 -th4 0.3 -out tmp.out
perl out_claster_log.pl tmp.out $DATA_DIR/$FILE $FILE.clust
perl clust_statistics.pl $DATA_DIR/$FILE tmp.out $FILE.log
rm tmp.out
