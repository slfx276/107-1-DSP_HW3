#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <vector>
#include <map>
#include "Ngram.h"
#include <ctime>
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

struct cmp_str  
{  
    bool operator()(char const *a, char const *b)  
    {  
        return strcmp(a, b) < 0;  
    }  
};  

int main(int argc, char *argv[])
{
    double t1 , t2;
    t1 = clock();
    
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
    ifstream text_fp , mapping_fp;
    text_fp.open(text_file);
    mapping_fp.open(mapping);
    string temp;
    vector<string> text_vector , map_vector;
    int i, j;
    /* each round is one row of text */
    while(getline(text_fp , temp)) // not read to end
    {
       text_vector.push_back(temp);
    }
    int totalRow = text_vector.size() ;

    /* read mapping file */
    while(getline(mapping_fp , temp))
    {
        map_vector.push_back(temp);
    }
    text_fp.close();
    mapping_fp.close();

    /* Text processing to char*  */
    vector<char *> text_set[text_vector.size()];
    for(i = 0 ; i < text_vector.size() ; i++)
    {
        /* string change to *char */
        char *text_row = strdup(text_vector[i].c_str());
        const char *d = " \t\n";
        char *p;
        p = strtok(text_row ,d);
        while (p)
        {
            // printf("%s",p);
            text_set[i].push_back(p);
            p = strtok(NULL, d);
        } 
    }
    
    /* Mapping processing to map and *char */
    map<char* , vector<char*> , cmp_str> mapZhuYin;
    map<char* , vector<char*> , cmp_str>::iterator iter1 , iter2;

    for(i = 0 ; i < map_vector.size() ; i++)
    {
        vector<char*> map_set;
        char *ZhuYin;
        char *p;
        char *row = strdup(map_vector[i].c_str());
        // cout<<row<<endl;
        const char *d = " \t\n";

        p = strtok(row , d);
        ZhuYin = p;
        while(p)
        {
            map_set.push_back(p);
            p = strtok(NULL , d);
        }
        map_set.erase(map_set.begin());
        mapZhuYin.insert(pair<char*,vector<char*>>(ZhuYin , map_set));

    }
    // t2 = clock();
    // cout<<"\nCost Time = "<< double(t2 - t1) / CLOCKS_PER_SEC << endl;
    ////////////////////////////////////////////////////////////////////////////////////

    /* text_set[i] (vector<char*>) 裡面存了一行的tesxt 
    mapZhuYin[key] (vector<char*>) 裡面存了mapping 的 Big-5 */

    string s1 = "<s>" , s2 = "</s>";
    char *start = strdup(s1.c_str()) , *end = strdup(s2.c_str());
    /* each round is processing Viterbi for text_set[i] ;  */
    int rowNumber;
    cout<<"totalRow = " << totalRow <<endl;
    for(rowNumber = 0 ; rowNumber < totalRow ; rowNumber++)
    {
        cout<<"rowNumber = "<<rowNumber<<endl;
        /* text_set[rowNumber] 是存現在正在處理的這行之vector */
        int wordLength = text_set[rowNumber].size() , word_MapIndex , wordTime;

        vector<float> vectorDelta[wordLength];
        vector<int> vectorPreword[wordLength];
        /* Delta 0 initializ */
        iter1 = mapZhuYin.find(text_set[rowNumber][0]);
        for(i = 0 ; i < iter1->second.size() ; i++)
        {
            vectorDelta[0].push_back(getBigramProb(start , iter1->second[i] , voc , lm));
        }
        t2 = clock();
        cout<<"\nCost Time = "<< double(t2 - t1) / CLOCKS_PER_SEC << endl;

        /* 每一 round 處理一個字 */
        for( wordTime = 1 ; wordTime < wordLength ; wordTime ++)
        {
            cout<<"wordTime = "<<wordTime<<" ";
            iter1 = mapZhuYin.find(text_set[rowNumber][wordTime-1]);
            iter2 = mapZhuYin.find(text_set[rowNumber][wordTime]);
            char *temp;
            int preIndex , proIndex;
            float maxProb = -10000 , prob;
            char *maxPreword;
            int maxPrewordIndex;
            /* search each word pair of neighbor words  */
            for(proIndex = 0 ; proIndex < iter2->second.size() ; proIndex++)
            {
                maxProb = -10000;
                /* which pre word can maximize Delta[t][q_proIndex] */
                for(preIndex = 0 ; preIndex < iter1->second.size() ; preIndex++)
                {
                    prob = 0;
                    // prob = getBigramProb(iter1->second[preIndex] , iter2->second[proIndex] , voc , lm);
                    //////////////////////////////////
                    VocabIndex wid1 = voc.getIndex(iter1->second[preIndex]);
                    VocabIndex wid2 = voc.getIndex(iter2->second[proIndex]);

                    if(wid1 == Vocab_None)  //OOV
                        wid1 = voc.getIndex(Vocab_Unknown);
                    if(wid2 == Vocab_None)  //OOV
                        wid2 = voc.getIndex(Vocab_Unknown);

                    VocabIndex context[] = { wid1, Vocab_None };
                    prob = lm.wordProb( wid2, context);

                    //////////////////////////////////


                    if(prob + vectorDelta[wordTime-1][preIndex] > maxProb)
                    {
                        maxProb = prob + vectorDelta[wordTime-1][preIndex];
                        maxPrewordIndex = preIndex;
                    }
                }
                /* update Delta[wordTime] and backtracking */
                vectorDelta[wordTime].push_back(maxProb);
                vectorPreword[wordTime].push_back(maxPrewordIndex);
                i = maxPrewordIndex;
            }
            // cout<<proIndex<<" "<<iter1->second[i]<<" ";
            t2 = clock();
            cout<<"=> wordCost Time = "<< double(t2 - t1) / CLOCKS_PER_SEC << endl;
        } // finish one row

        /* Viterni search maximum probability path */

        // for(iterDelta = mapToDelta[wordLength-1].begin() ; iterDelta != mapToDelta[wordLength-1].end() ; iterDelta++)
        // {
        // }

        break;
        cout<<endl;
    }


    ofstream output_file;
    output_file.open(output);
    output_file.close();
    t2 = clock();
    cout<<"\nCost Time = "<< double(t2 - t1) / CLOCKS_PER_SEC << endl;
    return 0;
}
