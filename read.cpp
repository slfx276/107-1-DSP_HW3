#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <vector>
#define SIZE 3000

using namespace std;

double getBigramProb(const char *w1)
{
    // cout<<w1;
    printf("%s\n\n",w1);
}
vector<char*> Get_map_set(char* ZhuYin)
{
    /* read ecah row */
    ifstream map_file;
    map_file.open("ZhuYin-Big5.map");
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


int main(int argc , char **argv)
{
    /* read text file */
    ifstream text_file;
    text_file.open("seg_1.txt");
    string temp;
    vector<char *> text_set;
    int i;
    while(getline(text_file , temp)) // not read to end
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
        text_set.push_back(text_set[0]);
        cout<<"\ntext size = "<<text_set.size()<<endl<<endl;

        /* create ZhuYin mapping set */
        // vector<char*> map_test; 
        // for(i = 0; i <text_set.size() ; i++)
        // {
        //     int j;
        //     map_test = Get_map_set(text_set[i]);
        //     cout<<"\nZhuYin = "<<text_set[i]<<"  size = "<<map_test.size()<<endl;
        //     for(j=0 ; j < map_test.size() ; j++)
        //     {
        //         cout<<map_test[j];
        //     }
        //     cout<<endl;
        // }
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

    text_file.close();
    return 0;
}