#include <iostream>
#include <fstream>
#include <string>

int LoadColison(char* file_name,float* &arr_v,int* &arr_f, int &i_v,int &i_f)
{
    string buff="";
    i_f=0; 
    i_v=0;
    arr_v=(float*)malloc(sizeof(float));
    arr_f=(int*)malloc(sizeof(float));
    int type=0;
    ifstream file(file_name);
    char c = file.get();
    while (file.good() && c!=EOF){
                     
          if (c=='f'){
               c = file.get();
               if(c==' ') type=1;
          }
          else{
              if (c=='v'){
                 c = file.get();
                 if(c==' ') type=0;
              }
          }
          
          while(((c!='f') && (c!='v')) && (c!=EOF)){
                 if(((c >= '0') && (c <= '9')) || (c == '.') || (c == '-')){
                        buff=buff+c;
                        c = file.get();
                        if (((c < '0') || (c > '9')) && (c != '.') && (c != '-')){
                             switch (type){
                                  case 0:
                                       arr_v=(float*)realloc(arr_v,(i_v+1)*sizeof(int));
                                       arr_v[i_v]=atof(buff.c_str());
                                       i_v++;
                                       break;
                                  case 1:
                                       arr_f=(int*)realloc(arr_f,(i_f+1)*sizeof(int)); 
                                       arr_f[i_f]=atoi(buff.c_str());
                                       i_f++;
                                       break;
                             }
                             buff="";
                        }
                 }
                 else{
                    c = file.get();
                    if(c=='#') while(c!='\n') c = file.get();
                 }
          }
    }
    file.close();
    return 0;
}
