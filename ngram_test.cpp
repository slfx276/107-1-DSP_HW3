#include <stdio.h>
#include "Ngram.h"
#include <fstream>
#include <iostream>
#include <string.h>
#include <vector>
#define SIZE 2600
using namespace std;

// Get P(W2 | W1) -- bigram
double getBigramProb(const char *w1, const char *w2 , Vocab voc , Ngram lm)
{
     VocabIndex wid1 = voc.getIndex(w1);
     VocabIndex wid2 = voc.getIndex(w2);

     if(wid1 == Vocab_None)  //OOV
         wid1 = voc.getIndex(Vocab_Unknown);
     if(wid2 == Vocab_None)  //OOV
         wid2 = voc.getIndex(Vocab_Unknown);

     VocabIndex context[] = { wid1, Vocab_None };
     return lm.wordProb( wid2, context);
}
char * Select_Big5(char* c1 , char* c2 , char* mapping , Vocab voc , Ngram lm)
{
    cout<<"PATH = "<<c1 <<c2<<endl;
    char map_row[SIZE];

    cout<<"enter1";
    FILE *mapping_fp = fopen(mapping , "r");
    cout<<"enter2";
    static vector<char*> mapOne;
    cout<<"enter3";
    static vector<char*> mapTwo;
    cout<<"enter4";
    const char *d = " \t\n";
    cout<<"enter5";
    int q , f1 = 0 ,f2 = 0;
    cout<<"enter";
    while(fgets(map_row , SIZE , mapping_fp)!=NULL)
    {
        // ¦r¦ê¤Á³Î
        char* w;
        cout<<map_row<<endl;
        w = strtok(map_row , d);
        // hit mapping row for c1
        if(strcmp(w , c1) == 0)
        {
            cout<<"map row = "<<map_row<<endl;
            f1 = 1;
            cout<<"find "<<c1<<endl;
            while(w)
            {
                mapOne.push_back(w);
                w = strtok(NULL , d);
            }
            mapOne.erase(mapOne.begin());

            // print mapping row
            cout<<"mapOne row = ";
            for(q = 0 ; q < mapOne.size() ; q++)
            {
                cout<<mapOne.at(q);
            }
            cout<<endl<<endl;
        }
    }    
    fclose(mapping_fp);
    mapping_fp = fopen(mapping , "r");
    while(fgets(map_row , SIZE , mapping_fp)!=NULL)
    {
        // ¦r¦ê¤Á³Î
        char* w;
        w = strtok(map_row , d);
        // hit mapping row for c2
        if(strcmp(w , c2) == 0)
        {
            
            f2 = 1;
            cout<<"find "<<c2<<endl;
            while(w)
            {
                mapTwo.push_back(w);
                w = strtok(NULL , d);
            }
            mapTwo.erase(mapTwo.begin());
            // print mapping row inside
            cout<<"mapTwo row = ";
            for(q = 0 ; q < mapTwo.size() ; q++)
            {
                cout<<mapTwo[q];
            }
            cout<<endl;

            cout<<"size = "<<mapTwo.size()<<"twice = ";
            for(q = 0 ; q < mapTwo.size() ; q++)
            {
                cout<<mapTwo[q];
            }
            cout<<endl;
        }
    }
    // cout<<mapTwo.at(0);
    cout<<"mapTwo size = "<<mapTwo.size()<<"  third = ";
    for(q = 0 ; q < mapTwo.size() ; q++)
    {
        cout<<mapTwo[q];
    }
    cout<<"end";
    cout<<endl;

    cout<<"final"<<mapTwo.at(0);

    fclose(mapping_fp);
    return "";
}

int main(int argc, char *argv[])
{
    /* input **argv processing */
    char *text_file , *mapping , *language_model , *o , *output;
    text_file = argv[1];
    mapping = argv[2];
    language_model = argv[3];
    o = argv[4];
    output = argv[5];
    int ngram_order;
    if(strcmp(o,"2") == 0)
        ngram_order = 2;
    else if(strcmp(o , "3") == 0)
        ngram_order = 3;
    else
    {
        cout<<"input order is out of range 2 to 3";
        return 0;
    }

    /* read language model */
    Vocab voc;
    Ngram lm( voc, ngram_order );
    {
        File lmFile( language_model , "r" );
        lm.read(lmFile);
        lmFile.close();
    }

    /* read text file. */
    char text_line[SIZE];
    FILE *text_fp = fopen(text_file , "r");
    int i = 0;
    vector<char*> path;
    /* each round is 1 row in "text_file" */
    while(fgets(text_line , SIZE , text_fp)!=NULL)
    {
        //////////// read text row ////////////////
	    /* text partition */
        char *p;    
        // vector<char*> path;
        const char *d = " \t\n";
        p = strtok(text_line , d);
        while(p)
        {
    	    cout<<p;
            path.push_back(p);
	        p = strtok(NULL , d);
        }
        cout<<endl;
        // path.pop_back();    // delete the last empty character

        /* print text Vector */
        cout<<endl<<"result:"<<path.size()<<endl;
        for(i = 0;i<path.size();i++)
        {
            cout<<path.at(i);
        }
        cout<<endl;
        /* bigram prob. */
        // for(i = 0;i<path.size()-1;i++)
        // {
        //     cout<<i<<getBigramProb(path[i] , path[i+1] , voc , lm)<<endl;
        // }
        cout<<endl<<endl;
        path.clear();
     }

    cout<<endl;
    Select_Big5("§A", "£²" , mapping , voc , lm);
    fclose(text_fp);
    cout<<endl<<endl;
   



    return 0;
}