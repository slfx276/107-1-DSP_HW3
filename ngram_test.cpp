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
    ofstream output_file;
    output_file.open(output);
    text_fp.open(text_file);
    string temp;
    vector<char *> text_set;
    int i;
    /* each round is one row of text */
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
            // printf("%s",p);
            text_set.push_back(p);
            p = strtok(NULL, d);
        }   
        // cout<<"\ntext size = "<<text_set.size()<<endl<<endl;

        /* create ZhuYin mapping set */
    
        int Time = text_set.size();
        int i , j , t;
        vector<float> Delta[Time];
        vector<char*> map_before , map_after; 
        string s1 = "<s>" , s2 = "</s>";
        char *start = strdup(s1.c_str()) , *end = strdup(s2.c_str());
        
        /*  from beginning to the end. */
        for(t = 1 ; t < Time ; t++)
        {
            map_before.clear();
            map_after.clear();
            map_before = Get_map_set(text_set[t-1] , mapping);
            map_after = Get_map_set(text_set[t] , mapping);

            // cout<<"start find pair of time = "<<t<<endl;
            // cout<<"map_after.size = "<<map_after.size()<<endl;
            int m1 , m2;
            char *pre , *pro;
            float max_prob = -10000;
            /* find pair of max prob. and then update directly. */
            for(m2 = 0 ; m2 < map_after.size() ; m2++)
            {  
                char* max_preword;
                int preword_index;
                for(m1 = 0 ; m1 < map_before.size() ; m1++)
                {
                    float transition = getBigramProb(map_before[m1] , map_after[m2] , voc  , lm);
                    if( transition > max_prob)
                    {
                        max_prob =  transition;
                        pre = map_before[m1];
                        pro = map_after[m2];
                    }
                }
            }
            /* update text[t-1] and text[t] directly */
            // cout<<max_prob<<" "<<pre<<" "<<pro<<endl;
            text_set[t-1] = pre;
            text_set[t] = pro;
        }
        /* show mapping result */
        cout<<"<s> ";
        output_file<<"<s> ";
        for(i = 0 ; i < text_set.size() ; i++)
        {
            cout<<" "<<text_set[i];
            output_file<<" "<<text_set[i];
        }
        cout<<" </s>"<<endl;
        output_file<<" </s>\n";
    }
    output_file.close();
    text_fp.close();

    return 0;
}