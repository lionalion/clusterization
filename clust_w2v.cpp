//the author: L.Solovev

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>

#define _CRT_SECURE_NO_WARNINGS ;
using namespace std;

void error (const char * text1, const char  * text2 = ""){
	std::cerr << text1 << ' ' << text2 << endl;
	exit(1);
}
void postClast (std::vector < std::vector < int > > vIdStr,std::vector < std::vector <std::map<int,float> > > vMapCls, int th3, float th4, std::vector < std::vector < int > > & vOutIdStr, std::vector < std::vector <std::map<int,float> > > & vOutMapCls){
	std::multimap <int,int> SizeMapCls;
	int n=0;//number new claster
	for (int k=0; k<vMapCls.size(); k++)
		SizeMapCls.insert(std::pair<int,int>( vMapCls[k].size(),k )); //ранжируем по размеру кол-ва док в кластере	

	std::vector <int> vCheck;			
	for (std::multimap<int,int>::const_reverse_iterator rit = SizeMapCls.rbegin(); rit != SizeMapCls.rend(); ++rit){		
		bool flg_x=false;
		int x = (*rit).second;
		for (int q=0; q<vCheck.size(); q++){
			if (vCheck[q] == x)
				flg_x=true;
		}
		if (vIdStr[x].size() > th3 && flg_x==false){//третий порог + проверка уже объединенных кластеров	
			vOutIdStr.resize( vOutIdStr.size()+1 );
			vOutMapCls.resize( vOutMapCls.size()+1 );
			for (int h=0; h< vIdStr[x].size(); h++){							
				vOutIdStr[n].push_back(vIdStr[x][h]); // id строк кластера							
				vOutMapCls[n].push_back(vMapCls[x][h]); // {класс-вес} кслатера
			}
			for ( std::multimap<int,int>::const_reverse_iterator tir = rit; tir != SizeMapCls.rend(); ++tir){
				bool flg_y=false;
				int y = (*tir).second;
				for (int q=0; q<vCheck.size(); q++){
					if (vCheck[q] == y)
						flg_y=true;
				}
				if (x!=y && flg_y==false){
					int cnt=0;
					if (vIdStr[y].size() > th3){
						for (int w=0; w<vIdStr[x].size(); w++){
							for (int z=0; z<vIdStr[y].size(); z++){
								if (vIdStr[x][w] == vIdStr[y][z]) {
									cnt++;
									break;
								}
							}
						}
						//объединяем кластеры с одинаковым набором документов
						if ( cnt > vIdStr[y].size()*th4){//порог th4
							//out: vIdStr[x]+vIdStr[y] - IDstr; vMapCls[y]+vMapCls[y] - class&weight 
							for (int g=0; g< vIdStr[y].size(); g++){ 
								//тут нужна проверка на наличие дока в новом кластере
								bool be=false;
								for (int j=0; j< vOutIdStr[n].size(); j++){
									if(vOutIdStr[n][j] == vIdStr[y][g])
										be=true;
								}
								if (be==false){
									vOutIdStr[n].push_back(vIdStr[y][g]);// добавляем id строк кластера
									vOutMapCls[n].push_back(vMapCls[y][g]);// добавляем {класс-вес} клаcтера
								}
							}
							vCheck.push_back(y);
						}
					}
				}
			}	
			n++;					
		}		
	}
}
void firstClast (std::vector < std::map <int,float> > vCls, std::vector < int > vId, float th2, std::vector < std::vector <std::map<int,float> > > & vMapCls, std::vector < std::vector < int > > & vIdStr){
	//vCls & vId ранжировать по длине 
	std::multimap<int,int>  vMapRang;//длина вектора - его id
	for (int id=0; id<vCls.size(); id++)
		vMapRang.insert(std::pair<int,int>(vCls[id].size(),id));

	int cnt=0;
	for ( std::multimap<int,int>::const_reverse_iterator fst = vMapRang.rbegin(); fst != vMapRang.rend(); ++fst){	
		
		bool flg=false;
		for (int a=0; a<vIdStr.size();a++){
			for (int b=0; b<vIdStr[a].size();b++){
				int tmp=(*fst).second;
				if(vId[ (*fst).second ] == vIdStr[a][b]){//проверка на наличие id кластера в существующих: если док уже есть в др. кластере, то он не будет образовывать новый (уменьшение размерности)
					flg=true;
					break;
				}
			}
			if (flg==true)
				break;
		}
		
		if (flg==false){
			std::map <int,float> cls1;
			cls1=vCls[ (*fst).second ];
			vMapCls.resize(vMapCls.size()+1);
			vIdStr.resize(vIdStr.size()+1);
			vMapCls[cnt].push_back(cls1);
			vIdStr[cnt].push_back(vId[(*fst).second ]);

			for ( std::multimap<int,int>::const_reverse_iterator scnd = fst; scnd != vMapRang.rend(); ++scnd){
				if (fst!=scnd){
					std::map<int,float>::const_iterator itv = vCls[ (*fst).second ].begin();			
					std::map <int,float> cls2;			
					cls2=vCls[ (*scnd).second ];
					int comp=0;
					for ( std::map<int,float>::const_iterator itr = cls2.begin(); itr != cls2.end(); ++itr){
						std::map<int,float>::const_iterator itv = cls1.begin();
						itv = cls1.find( (*itr).first );//сравниваем классы первого вхождения с последующими
						if(itv != cls1.end() )
							comp++;
					}
					//проверка второго порога th2 (кол-во совпадений классов в двух документах)
					if( cls1.size()*th2 < comp){
						vMapCls[cnt].push_back(cls2);
						vIdStr[cnt].push_back(vId[(*scnd).second ]);
					}
				}
			}
			cnt++;
		}
	}
	vCls.clear();	
}
int main(int argc, char * argv[])
{
	if (argc < 7 || argc > 15) {error ("number of argv isn't correct: -fl input_file -w2v classes_w2v -n N -th1 n -th2 n -th3 n -th4 n -out output_file!\n"
		"format input_file - IDdocs(numbers)\\ttext\n"
		"format classes_w2v - word\\tclass\\tweight\n"
		"n - the numbers of docs in one epoch; default eof\n"
		"th1 - the min numbers of w2v classes  in claster; default = 4;\n"
		"th2 - the proximity of clasters 0-1; default = 0.7;\n"
		"th3 - the min numbers of docs in clasters; default = 3;\n"
		"th4 - the similarity of clasters according to the docs; default = 0.6;\n"
		);
		exit(0);}
	string line;
	string strText;
	char * namefile; 
	char * w2vfile; 
	ofstream out;
	int epo=-1;
	int th1=0; 
	float th2=0; 
	int th3=0;
	float th4=0;

	string  argum = argv [1];
	if ("-fl" == argum)
		namefile = argv [2];
	else{
		error ("No input file! First argument: -fl input");
		exit(0);
	}
	string  arguw = argv [3];
	if ("-w2v" == arguw)
		w2vfile = argv [4];
	else{
		error ("No w2v file! Second argument: -w2v inw2v");
		exit(0);
	}
	string outFile;
	bool flgoutfile = false;
	bool rang=false;
	for (int i=5; i< argc-1; i++){
		string  snum = argv [i];
		if ("-out" == snum){
				outFile = (argv[i+1]);
				flgoutfile = true;
				i++;
		}
		else if ("-n" == snum){
			epo = atoi (argv[i+1]);
			i++;
		}
		else if ("-th1" == snum){
			th1 = atoi (argv[i+1]);
			i++;
		}
		else if ("-th2" == snum){
			th2 = atof (argv[i+1]);
			i++;
		}
		else if ("-th3" == snum){
			th3 = atoi (argv[i+1]);
			i++;
		}
		else if ("-th4" == snum){
			th4 = atoi (argv[i+1]);
			i++;
		}
	}
	//default 
	if (th1==0)
		th1=4;
	if (th2==0)
		th2=0.7;
	if (th3==0)
		th3=3;
	if (th4==0)
		th4=0.6;

	if (flgoutfile == false){
			error ("no output file!");
			exit(0);
	}
	else
		out.open(outFile.c_str());

	map <string, string> w2v;

	ifstream myfile(namefile, ifstream::in );
	if (!myfile) {
		error ("No intext file!",namefile); 	
		exit (1);
	}
	//ifstream wfile (w2vfile , ifstream::in );
	ifstream wfile(w2vfile,ios::binary|ios::in);
	if (!wfile) {
		error ("No inw2v file!",w2vfile); 	
		exit (1);
	}

	if (wfile.is_open()){		
		char szBuffer[256];
		int size = sizeof(szBuffer);
		//memset(szBuffer, 0, size);
		while( wfile.read(szBuffer, size)){
		//while (!wfile.eof()){
			//char tmp = (char) wfile.get();
			line += szBuffer;
		}
	}

		
	if (wfile.is_open()){	
		while (getline(wfile, line)) {
			string strCls;
			string strWrd;
			int nPosTab;
			int nPosEnd = line.length();
			int nPos=0;
			while (nPos < line.length()){
				nPosTab = line.find('\t', nPos);
				if (nPosEnd==-1)
					nPosEnd=line.length();
				if (-1 != nPosTab  )		{
					strWrd = line.substr(nPos,nPosTab-nPos);		
					strCls = line.substr(nPosTab-nPos+1,nPosEnd-nPos);
					nPos=nPosEnd+1;
				}
				else if (nPos != nPosEnd) {
					error("format is not correct!");
					exit(1);
				}				
			}
			w2v.insert ( std::pair<string,string>(strWrd,strCls) );
		}
	}
		
	std::vector < std::vector < int > > vOutIdStr;
	std::vector < std::vector <std::map<int,float> > > vOutMapCls;
	std::vector < std::vector < int > > vIdStr;
	std::vector < int > vId;
	std::vector < std::vector <std::map<int,float> > > vMapCls;
	std::vector < std::map <int,float> > vCls; 
	int idstr;//string-doc numbers
	int cntepo=0; //счетчик эпох
	if (myfile.is_open()){				
		while (getline(myfile, line)) {
			cntepo++;
			std::map <int,float> cls;//map for each doc:class,weight 
			int cntmap=0;
			string strWrd;
			string strID;
			int nPosTab;
			int nPosSpace;
			int nPosEnd = line.length();
			int nPos=0;			
			while (nPos < line.length()){
				nPosTab = line.find('\t', nPos);
				nPosSpace = line.find(' ', nPos);
				if (nPosEnd==-1)
					nPosEnd=line.length();
				else if (-1 != nPosTab  && nPosTab < nPosEnd)		{
					strID = line.substr(nPos,nPosTab-nPos);		
					char *buffer = new char[strID.length() + 1];
					strcpy(buffer,  strID.c_str());
					idstr = strtod(buffer,NULL);
					nPos=nPosTab+1;
				}
				else if (-1 != nPosSpace  && nPosSpace < nPosEnd)		{
					strWrd = line.substr(nPos,nPosSpace-nPos);		
					nPos=nPosSpace+1;
				}
				else if (-1 == nPosSpace && nPos <= nPosEnd)	{
					strWrd = line.substr(nPos,nPosEnd-nPos);
					nPos=nPosEnd+1;
				}
				else if (nPos != nPosEnd) {
					error("format is not correct!");
					exit(1);
				}
				if (strWrd != "" ){
					std::multimap<string,string>::const_iterator itMap = w2v.begin();///создаем итератор на начало map
					itMap = w2v.find(strWrd);
					if(itMap != w2v.end() ){						
						string strTmp = (*itMap).second;
						int n=0;
						int nTab;
						int nEnd=strTmp.length();
						float wgt;
						int ncls;
						string strCh;
						while (n < strTmp.length()){
							nTab = strTmp.find('\t', n);
							if (-1 != nTab  && nTab < nEnd)		{
								strCh = strTmp.substr(n,nTab-n);		
								char *buffer = new char[strCh.length() + 1];
								strcpy(buffer, strCh.c_str());
								ncls = strtod(buffer,NULL);
								n=nTab+1;
							}
							else if (-1 == nTab && n <= nEnd)	{
								strCh = strTmp.substr(n,nEnd-n);
								char *buffer = new char[strCh.length() + 1];
								strcpy(buffer,  strCh.c_str());
								wgt = strtod(buffer,NULL);
								n=nEnd+1;
							}
						}
							
						if (0==cntmap){//map initialization 
							cls.insert ( std::pair<int,float>(ncls,wgt) );
							cntmap=1;
							//cnth1++;
						}
						else{//здесь суммируем веса слов по классам и записывать в map
							std::map<int,float>::const_iterator it = cls.begin();
							it = cls.find(ncls);
							if(it != cls.end() ){	
								cls.at(ncls) = (*it).second+wgt;								
							}
							else{
								cls.insert ( std::pair<int,float>(ncls,wgt) );
								//cnth1++;
							}
						}					
					}
				}	
			}
			if (cls.size()+1 > th1){//первый порог
				vCls.push_back( cls );	
				vId.push_back( idstr );
			}

			if (epo != -1 && cntepo > epo ){
				cout << "." ;
				firstClast(vCls, vId, th2,  vMapCls, vIdStr);
				vCls.clear();
				vId.clear();
				postClast (vIdStr,vMapCls, th3, th4, vOutIdStr, vOutMapCls);
				vIdStr.clear();
				vMapCls.clear();	
				if (vOutIdStr.size() != vOutMapCls.size() ){
					error("something strange: size of outID and outMap is't equal!");
					exit(1);
				}	
				cntepo=0;
			}			
		} //end while
	} //close file
	//если строк меньше, но файл уже закончился \n\neof
	if(vCls.size()==0 ){
		error ("No words or other lang!\n");
		exit(0);
	}
	else if ( epo == -1 ){
		cout << "beginning clasterization \n" ;
		firstClast(vCls, vId, th2,  vMapCls, vIdStr);
		vCls.clear();
		vId.clear();
		cout << "post clastering \n" ;
		postClast (vIdStr,vMapCls, th3, th4, vOutIdStr, vOutMapCls);
		vIdStr.clear();
		vMapCls.clear();	
		if (vOutIdStr.size() != vOutMapCls.size() ){
			error("something strange: size of outID and outMap is't equal!");
			exit(1);
		}			
	}
	else{
		cout << "\npost clastering \n" ;
		std::vector < std::vector < int > > vOutIdStr_epo;
		std::vector < std::vector <std::map<int,float> > > vOutMapCls_epo;
		postClast (vOutIdStr,vOutMapCls, th3, th4, vOutIdStr_epo, vOutMapCls_epo);
		vOutIdStr.clear();
		vOutMapCls.clear();	
		vOutIdStr.resize(vOutIdStr_epo.size());
		vOutIdStr=vOutIdStr_epo;
		vOutMapCls.resize(vOutMapCls_epo.size());
		vOutMapCls=vOutMapCls_epo;
		if (vOutIdStr.size() != vOutMapCls.size() ){
			error("something strange: size of outID and outMap is't equal!");
			exit(1);
		}		
	}
	//out vOutIdStr,vOutMapCls
	for (int c=0; c<vOutMapCls.size(); c++){
		out <<  c << ": " ;
		for (int x=0; x<vOutMapCls[c].size(); x++){
			std::map<int,float> ::const_iterator it = vOutMapCls[c][x].begin(); it != vOutMapCls[c][x].end(); ++it;
			out <<  vOutIdStr[c][x] << " " << (*it).second  << "; ";
		}			
		out <<  "\n" ;
	}
	out << endl;
}


