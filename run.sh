FILE=example.csv
./clust_w2v  -fl $FILE -w2v models/ru.w2v  -th1 4 -th2 0.5 -th3 3 -th4 0.3 -out tmp.out
perl out_claster_log.pl tmp.out $FILE $FILE.log
rm *.out