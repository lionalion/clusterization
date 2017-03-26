#Texts clusterization based of word2vec #

The clustering models were obtained by the word2vec (-classes) method and smoothed (normalized) in a certain way.
Features of the algorithm:
- Clustering occurs not by words, but by semantic classes;
- independence of the result from the length of documents;
- independence of the results from the volume of the material;
- automatic determination of the number of clusters;
- no need to define primary clusters;
- high speed of work.

////////////////////
Build under linux 
g++ clust_w2v.cpp -o clust_w2v

///////////////////

run:
./clust_w2v -fl input_file -w2v classes_w2v -n N -th1 n -th2 n -th3 n -th4 n -out output_log

		format input_file - "IDdocs(numbers)\\ttext\n"		
		format classes_w2v - "word\\tclass\\tweight\n"		
		n - the numbers of docs in one epoch; default eof		
		th1 - the min numbers of w2v classes  in claster; default = 4;		
		th2 - the proximity of clasters 0-1; default = 0.7;		
		th3 - the min numbers of docs in clasters; default = 3;		
		th4 - the similarity of clasters according to the docs; default = 0.6; 

the format input_file:
	IDdocs\tbla-bla\n
	
IDdocs - the number
(see for example)
 
the format output_log:
number_cluster: IDdocs1 weight1;IDdocs2 weight2;...IDdocsN weightN

//////////////////////

The script "out_claster_log.pl"  integrates the results of clustering and source text (input_file)

run:
perl out_claster_log.pl output_log input_file output_file
coding utf-8
recommened 5 version perl (or higher)

/////////////////////

for example - run.sh 

/////////////////////
The trains corpus:
english - Reuters  (http://trec.nist.gov/data/reuters/reuters.html) 1996-1997 ~ 180 million words
russian - (private collection) 2013-2014 ~ 410 million words

