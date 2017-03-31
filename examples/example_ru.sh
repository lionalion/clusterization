DATA_DIR=txt
BIN_DIR=../bin
MD_DIR=../mod

FILE=example_ru.txt

if [ ! -e $BIN_DIR ]; then
	make;
fi

$BIN_DIR/clust_w2v -fl $DATA_DIR/$FILE -w2v $MD_DIR/ru.w2v  -th2 0.5 -th3 4 -th4 0.3 -out tmp.out
perl out_claster_log.pl tmp.out $FILE $FILE.log
rm tmp.out