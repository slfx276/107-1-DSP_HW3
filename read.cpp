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
    printf("%s",w1);
}


int main(int argc , char **argv)
{
    // input **argv processing
    char *text_file , *mapping , *language_model , *o , *output;
    text_file = argv[1];
    mapping = argv[2];
    language_model = argv[3];
    o = argv[4];
    output = argv[5];
    int order;
    if(strcmp(o,"2") == 0)
        order = 2;
    else if(strcmp(o , "3") == 0)
        order = 3;
    
    else
    {
        cout<<"input order is out of range 2 to 3";
        return 0;
    }
    
    cout<<text_file<<" "<<mapping<<" "<<language_model<<" "<<order<<" "<<output<<" "<<endl;

    //  read mapping file
    // ifstream map_file;
    // string line;
    // map_file.open(mapping);
    // while(getline(map_file , line))
    // {
    //     cout<<line<<endl;
    //     cout<<"head = "<<line[0]<<line[1]<<endl;
    //     cout<<"len = " <<line.length()<<endl;
    // }
    // map_file.close();

    // method 2 for char *

    char line[SIZE];
    FILE *fp = fopen("ZhuYin-Big5.map" , "r");
    char *temp;
    vector<char*> path;
    while(fgets(line , SIZE , fp)!=NULL)
    {
        // ¦r¦ê¤Á³Î
        const char *d = " \t\n";
        char *p , Zhu[] ="£x";
        p = strtok(line , d);
        temp = p;
        if(strcmp(p , Zhu) == 0)
        {
            cout<<"find £x"<<endl;
            while(p)
            {
                getBigramProb(p);
                // cout<<p;
                p = strtok(NULL , d);
                path.push_back(p);
            }
            cout<<endl;
        }
    }
    cout<<"temp = "<<temp;
    cout<<endl<<endl;
    int i = 0;
    for(i = 0;i<path.size();i++)
    {
        cout<<path[i];
    }
    fclose(fp);








    // read text file
    ifstream inFile;
    inFile.open("test.txt");
    char c1[3] = {0};
    char c2[3] = {0};
    inFile>>c1[0]>>c1[1];
    while(inFile>>c2[0]>>c2[1])
    {
        // cout<<"1 = "<<c1<<","<<c2<<endl;
        // inFile>>c2[0]>>c2[1];
        c1[0] = c2[0];
        c1[1] = c2[1];
        c1[2] = 0;
    }
    char *s = c2;
    // cout<<s<<endl;
    inFile.close();
    return 0;
}