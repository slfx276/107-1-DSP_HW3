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
    FILE *mapping_fp = fopen(mapping , "r");
    static vector<char*> mapOne , mapTwo;
    const char *d = " \t\n";
    int q , f1 = 0 ,f2 = 0;;
    char *w;
    while(fgets(map_row , SIZE , mapping_fp)!=NULL)
    {
        // ¦r¦ê¤Á³Î
        w = strtok(map_row , d);
        // hit mapping row for c1
        if(strcmp(w , c1) == 0)
        {
            cout<<"map row = "<<map_row<<endl;
            f1 = 1;
            cout<<"find "<<c1<<endl;
            while(w)
            {
                // cout<<w;
                mapOne.push_back(w);
                w = strtok(NULL , d);
            }
            mapOne.erase(mapOne.begin());

            // print mapping row
            cout<<"mapOne row = ";
            for(q = 0 ; q < mapOne.size() ; q++)
            {
                cout<<mapOne[q];
            }
            cout<<endl<<endl;
        }
        else if(f1 == 1)
            break;
    }    
    fclose(mapping_fp);
    mapping_fp = fopen(mapping , "r");
    while(fgets(map_row , SIZE , mapping_fp)!=NULL)
    {
        // ¦r¦ê¤Á³Î
        w = strtok(map_row , d);
        cout<<"w = "<<w<<endl;
        cout<<"map row = "<<map_row<<endl;
        // hit mapping row for c2
        if(strcmp(w , c2) == 0)
        {
            f2 = 1;
            cout<<"find "<<c2<<endl;
            while(w)
            {
                // cout<<w;
                mapTwo.push_back(w);
                w = strtok(NULL , d);
            }
            mapTwo.erase(mapTwo.begin());
            // print mapping row
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
        else if(f2 == 1)
            break;

    }
    // cout<<mapTwo.at(0);
    cout<<"mapTwo size = "<<mapTwo.size()<<" third = ";
    for(q = 0 ; q < mapTwo.size() ; q++)
    {
        cout<<mapTwo[q];
    }
    cout<<"end";
    cout<<endl;
    // search pair
    int i,j;
    // for(i = 0 ; i < mapOne.size() ; i++)
    //     for(j = 0 ; j < mapTwo.size() ; j++)
    //     {
    //         cout<<mapOne[i];
    //         cout<<mapTwo[j];
    //         cout<<" = "<<getBigramProb(mapOne[i] , mapTwo[j] , voc , lm)<<endl;
    //         cout<<endl;
    //         cout<<"here = "<<mapOne[i]<<mapTwo[j]<<endl;
    //     }

    cout<<"final"<<mapTwo.at(0);

    fclose(mapping_fp);
    return "";
}



int main(int argc, char *argv[])
{
    // input **argv processing
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
    

    // read language model
    Vocab voc;
    Ngram lm( voc, ngram_order );

    {
        const char lm_filename[] = "./corpus.lm";
        File lmFile( language_model , "r" );
        lm.read(lmFile);
        lmFile.close();
    }

    //
    /*
    ifstream inFile;
    inFile.open(text_file);
    char bigram1[3] = {0};
    char bigram2[3] = {0};
    inFile>>bigram1[0]>>bigram1[1];
    
    while(inFile>>bigram2[0]>>bigram2[1])
    {
        if(strcmp(bigram2 , "\n") == 0 || strcmp(bigram1 , "\n") == 0)
        {
            cout<<"line over";
        }
        cout<<"c = "<<bigram1<<","<<bigram2<<endl;
        // get bigram prob.
        printf("Log Prob(±wªÌ|¬r-Å}) = %f\n", getBigramProb(bigram1 , bigram2 , voc , lm));
        
        // window shifting
        bigram1[0] = bigram2[0];
        bigram1[1] = bigram2[1];
        bigram1[2] = 0;
    }
    inFile.clsoe();
    */


    char text_line[SIZE];
    FILE *text_fp = fopen(text_file , "r");
    char *p;

    // each round is 1 row in "text_file"
    while(fgets(text_line , SIZE , text_fp)!=NULL)
    {
        //////////// read text row ////////////////
        vector<char*> path;
	    // ¦r¦ê¤Á³Î
        const char *d = " \t\n";
        //char *p ;
        p = strtok(text_line , d);
	
        while(p)
        {
    	    cout<<p<<"hi"<<endl;
            path.push_back(p);
	        p = strtok(NULL , d);
        }

        // print text Vector
        cout<<endl;
        path.pop_back();
        int q = 0;
        for(q = 0;q<path.size()-1;q++)
        {
            cout<<q<<getBigramProb(path[q] , path[q+1] , voc , lm)<<endl;
        }

        //////////// read mapping ///////////////
        // char map_row[SIZE];
        // FILE *mapping_fp = fopen("ZhuYin-Big5.map" , "r");
        // vector<char*> map;
        // while(fgets(map_row , SIZE , mapping_fp)!=NULL)
        // {
        //     // ¦r¦ê¤Á³Î
        //     char *w , Zhu[] ="£x";
        //     w = strtok(map_row , d);
        //     // hit mapping row 
        //     if(strcmp(w , path[0]) == 0)
        //     {
        //         cout<<"find "<<path[0]<<endl;
        //         while(w)
        //         {
        //             // cout<<w;
        //             // map.push_back(w);
        //             w = strtok(NULL , d);
        //         }

        //         // map.pop_back();

        //         // print mapping row
        //         cout<<"map row = ";
                
        //         for(q = 0 ; q < map.size() ; q++)
        //         {
        //             cout<<map[q];
        //         }
        //         cout<<endl;
        //     }
        //     else
        //     {
        //         continue;
        //     }
        //     fclose(mapping_fp);
        // }


        //test
        cout<<endl<<endl;
        Select_Big5("£²", "£²" , mapping , voc , lm);
        // cout<<"PATH"<<path[0]<<path[1]<<endl;
        // cout<<getBigramProb(path[0] , path[1] , voc , lm);
        ///////////////////////////





     }
    fclose(text_fp);
    cout<<endl<<endl;
   



    return 0;
}

