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

vector<char*> Get_map_set(char* ZhuYin , char* mapping)
{
    /* read ecah row */
    ifstream map_file;
    map_file.open(mapping);
    string s;
    vector<char *> map_set;
    int i;
    while(getline(map_file , s)) // not read to end
    {
        
        /* string change to *char */
        char *row = strdup(s.c_str());
        /* string partition */
        const char *d = " \t\n";
        char * p;
        int hit = 0;

        p = strtok(row , d);
        while (p)
        {
            if(strcmp(p , ZhuYin) == 0 || hit == 1 )
            {
                map_set.push_back(p);
                hit = 1;
            }
            else if(hit == 0)
                break;
            p = strtok(NULL, d);
        }   
        if(hit == 1)   // have found mapping row
            break;
        int i = 0;
    }
    // cout<<"size = "<<map_set.size()<<endl;
    // for(i = 0 ; i < map_set.size();i++)
    // {
    //     cout<<map_set[i];
    // }

    map_file.close();
    map_set.erase(map_set.begin());
    return map_set;
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

    /* read text file */
    ifstream text_fp;
    text_fp.open(text_file);
    string temp;
    vector<char *> text_set;
    int i;
    while(getline(text_fp , temp)) // not read to end
    {
        text_set.clear();
        /* string change to *char */
        char *text_row = strdup(temp.c_str());
        /* string partition */
        const char *d = " \t\n";
        char * p;
        p = strtok(text_row , d);
        while (p)
        {
            printf("%s",p);
            text_set.push_back(p);
            p = strtok(NULL, d);
        }   
        cout<<"\ntext size = "<<text_set.size()<<endl<<endl;



        // 考慮做一個二維vector 放整行?
        /* start Viterbi */
        int Time = text_set.size();


        /* create ZhuYin mapping set */
        
        //print corresponding big-5 mapping set 
        vector<char*> map_test; 
        for(i = 0; i <text_set.size() ; i++)
        {
            int j;
            map_test = Get_map_set(text_set[i] , mapping);
            cout<<"\nZhuYin = "<<text_set[i]<<"  size = "<<map_test.size()<<endl;
            for(j=0 ; j < map_test.size() ; j++)
            {
                cout<<map_test[j];
            }
            
            cout<<endl;
        }
        

    }
    // for(i=0;i<text_set.size() - 1;i++)
    // {
    //     vector<char*> m1,m2;
    //     cout<<text_set[i]<<endl;
    //     m1 = Get_map_set(text_set[i]);
    //     m2 = Get_map_set(text_set[i+1]);
    // }

    // vector<char*> m1 , m2;
    // m1 = Get_map_set(text_set[0]);
    // m2 = Get_map_set(text_set[1]);
    // cout<<"ZhuYin 1 = "<<text_set[0]<<"  size = "<<m1.size()<<endl;
    // for(i=0;i<m1.size();i++)
    // {
    //     cout<<m1[i];
    // }
    // cout<<"\n\nZhuYin 2 = "<<text_set[1]<<"  size = "<<m2.size()<<endl;
    // for(i=0;i<m2.size();i++)
    // {
    //     cout<<m2[i];
    // }



    cout<<"\n\nend"<<endl;
    cout<<endl;

    /* send ZhuYin to "Get_map_set" to get Big-5 mapping set */

    /* assign ZhuYin method
    string s= "ㄒ";
    char *ZhuYin = strdup(s.c_str());
    vector<char*> map_set = Get_map_set(ZhuYin);
    vector<char*> map_set = Get_map_set(text_set[0]);
    cout<<"\nreturn map size = "<<map_set.size()<<endl;
    for(i=0;i<map_set.size();i++)
    {
        cout<<map_set[i];
    }
    */

    text_fp.close();
    return 0;

    return 0;
}