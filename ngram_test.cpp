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
            printf("%s",p);
            text_set.push_back(p);
            p = strtok(NULL, d);
        }   
        cout<<"\ntext size = "<<text_set.size()<<endl<<endl;

        /* create ZhuYin mapping set */
    
        int Time = text_set.size();
        int i , j , t;
        vector<char*> q[Time];
        vector<int> track_index[Time];
        vector<float> Delta[Time];
        vector<char*> map_before , map_after; 
        vector<char*> map_test;
        string s1 = "<s>" , s2 = "</s>";
        char *start = strdup(s1.c_str()) , *end = strdup(s2.c_str());
        
        /* Viterbi parameter from beginning to the end. */
        for(t = 0 ; t < Time-1 ; t++)
        {
            map_before.clear();
            map_after.clear();
            map_before = Get_map_set(text_set[t] , mapping);
            map_after = Get_map_set(text_set[t+1] , mapping);
            /* initialize Delta[0] */
            if(t==0)
            {
                // Delta[0] have map_before.size() elements.
                for(j = 0 ; j < map_before.size() ; j++)
                {
                    Delta[0].push_back(getBigramProb( start , map_before[j] , voc , lm));
                }
                // for(j = 0 ; j < Delta[0].size() ; j++)
                // {
                //     cout<<"Dealta "<<map_before[j]<<" = "<<Delta[0][j]<<endl;
                // }
            }
            cout<<"finish Delta[0] initialization. time = 1/"<<t<<endl;
            int m1 , m2;
            /* create other Time-1 Delta */
            cout<<"map_after.size = "<<map_after.size()<<endl;
            for(m2 = 0 ; m2 < map_after.size() ; m2++)
            {  
                float max_prob = -10000;
                char* max_preword;
                int preword_index;
                /* update Delta[t][m2] */
                for(m1 = 0 ; m1 < map_before.size() ; m1++)
                {
                    printf("(%d.%d), ",m1,m2);
                    float transition = getBigramProb(map_before[m1] , map_after[m2] , voc  , lm);
                    if(Delta[t][m1] + transition > max_prob)
                    {
                        max_prob = Delta[t][m1] + transition;
                        max_preword = map_before[m1];
                        preword_index = m1;
                    }
                }
                
                /* store for back tracking */
                Delta[t+1].push_back(max_prob);
                track_index[t+1].push_back(preword_index);
                /* use element of track_index[t+1] as index seqrch in q[t] */
                q[t].push_back(max_preword);
            }
            cout<<endl;
            for(j = 0 ; j < q[0].size() ; j++)
            {
                cout<<"q[0].size() = "<<q[0].size()<<"  "<<j<<" = "<<q[0][j];
            }
            cout<<endl;
            // map_before.clear();
            // map_after.clear();
        }

        /* select Viterbi path */
        int Viterbi_final_path_index = 0;
        float Viterbi_prob = -10000;
        for(i = 0 ; i < Delta[Time-1].size() ; i++)
        {
            if(Delta[Time-1][i] > Viterbi_prob)
            {
                Viterbi_final_path_index = i;
                Viterbi_prob = Delta[Time-1][i];
            }
        }
        cout<<"\nViterbi select = "<<Viterbi_final_path_index<<endl;
        cout<<"\n\nend";
        //////////////////////
        // break;
        ///////////////////////////
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
    string s= "££";
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