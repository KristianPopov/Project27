#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(){
    
    char str[256];
    string buff="";
    
    cout << "Enter the name of an existing text file: ";
    cin.get (str,256);
    
    ifstream file("model2.txt"); 
    // if ????
    cout << "File successfully opened!" << endl;
    
    int type; // v-0; vt-1; vn-2; f-3;
    int i_f=0, i_v=0, i_vt=0, i_vn=0;
    int i=0;
    
    float arr_f[10000];
    float arr_v[10000][3]; 
    float arr_vt[10000][2];
    float arr_vn[10000][3];
    
    char c = file.get();
    while (file.good() && c!=EOF){
          
          while((c!='v') && (c!='f')){
             if(c=='#'){
                 while(c!='\n'){
                       c=file.get();
                 }
             }
             c=file.get();
          }
                     
          if (c=='f'){
               type=3;
               c = file.get();
          }
          else{ 
              if (c=='v'){
                   c = file.get();
                       if (c=='n'){
                            type=2;
                            c = file.get();
                       }
                       else if (c=='t'){
                            type=1;
                            c = file.get();
                            //cout << "1:[" << c << "]";
                       }
                       else{
                            type=0;
                       }
              }
          }
          //i=0;
          while(((c!='f') && (c!='v')) && (c!=EOF)){
                 if(((c >= '0') && (c <= '9')) || (c == '.') || (c == '-')){      
                        buff=buff+c;
                        c = file.get();
                        if (((c < '0') || (c > '9')) && (c != '.') && (c != '-')){
                             switch (type){
                                  case 0:
                                       arr_v[i_v][i]=atof(buff.c_str());
                                       i++;
                                       if(i==3){i_v++; i=0;}
                                       break;
                                  case 1:
                                       cout<<i_vt<<"-"<<i<<"-"<<buff.c_str()<<endl;
                                       arr_vt[i_vt][i]=atof(buff.c_str());
                                       i++;
                                       if(i==2){i_vt++; i=0;}
                                       break;
                                  case 2:
                                       arr_vn[i_vn][i]=atof(buff.c_str());
                                       i++;
                                       if(i==3){i_vn++; i=0;}
                                       break;
                                  case 3:
                                       arr_f[i_f]=atof(buff.c_str());
                                       i_f++;
                                       break;     
                             }
                             buff="";
                        }
                 }
                 else{
                    c = file.get();
                 }
          }  
                     
          
    }

cout<<arr_v[0][0]<<endl;
cout<<arr_v[0][1]<<endl;
cout<<arr_v[0][2]<<endl;
cout<<arr_v[1][0]<<endl;
cout<<arr_v[1][1]<<endl;
cout<<arr_v[1][2]<<endl<<endl;

  
cout<<arr_vt[0][0]<<endl;
cout<<arr_vt[0][1]<<endl;
cout<<arr_vt[1][0]<<endl;
cout<<arr_vt[1][1]<<endl<<endl;


cout<<arr_vn[0][0]<<endl;
cout<<arr_vn[0][1]<<endl;
cout<<arr_vn[0][2]<<endl;
cout<<arr_vn[1][0]<<endl;
cout<<arr_vn[1][1]<<endl;
cout<<arr_vn[1][2]<<endl<<endl;

cout<<arr_f[0]<<endl;
cout<<arr_f[1]<<endl<<endl;
    
    file.close(); 
    return 0;
}
