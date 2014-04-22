#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

template <class T> class Point3D
{
    private:
        T x, y, z;
 
    public:
        // Default constructor
        Point3D() { x = y = z = 0; };
        // Three parameter constructor
        Point3D(T xValue, T yValue, T zValue)
        {
            x = xValue;
            y = yValue;
            z = zValue;
        }
 
        // ------------ Getters and setters ------------
 
        void set(const T &xValue, const T &yValue, const T &zValue)
        {
            x = xValue;
            y = yValue;
            z = zValue;
        }
 
        T getX() const { return x; }
        T getY() const { return y; }
        T getZ() const { return z; }
 
        void setX(const T &xValue) { x = xValue; }
        void setY(const T &yValue) { y = yValue; }
        void setZ(const T &zValue) { z = zValue; }
};

template <class T> class Point2D
{
    private:
        T x, y;
 
    public:
        // Default constructor
        Point2D() { x = y = 0;};
        // 2 parameter constructor
        Point2D(T xValue, T yValue)
        {
            x = xValue;
            y = yValue;
        }
 
        // ------------ Getters and setters ------------
 
        void set(const T &xValue, const T &yValue, const T &zValue)
        {
            x = xValue;
            y = yValue;
        }
 
        T getX() const { return x; }
        T getY() const { return y; }
 
        void setX(const T &xValue) { x = xValue; }
        void setY(const T &yValue) { y = yValue; }
};

int objLoadModel(char* file_name,
                 float* &arr_v,
                 float* &arr_vt,
                 float* &arr_vn,
                 int* &arr_f, 
                 int &i_v, 
                 int &i_vt, 
                 int &i_vn,
                 int &i_f)
{
    string buff="";
    int i=0;
    i_f=0; 
    i_v=0;
    i_vt=0; 
    i_vn=0;
    arr_v=(float*)malloc(sizeof(float));
    arr_vt=(float*)malloc(sizeof(float));
    arr_vn=(float*)malloc(sizeof(float));
    arr_f=(int*)malloc(sizeof(float));
    int type=0;
    ifstream file(file_name);
    char c = file.get();
    while (file.good() && c!=EOF){
                     
          if (c=='f'){
               c = file.get();
               if(c==' ') type=3;
          }
          else{
              if (c=='v'){
                   c = file.get();
                       if (c=='n'){
                            i=0;
                            type=2;
                            c = file.get();
                       }
                       else if (c=='t'){
                            type=1;
                            i=0;
                            c = file.get();
                            //cout << "1:[" << c << "]";
                       }
                       else{
                            type=0;
                       }
              }
              else type = -1;
          }
          //i=0;
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
                                       //cout<<i_vt<<"-"<<i<<"-"<<buff.c_str()<<endl;
                                       arr_vt=(float*)realloc(arr_vt,(i_vt+1)*sizeof(int));
                                       arr_vt[i_vt]=atof(buff.c_str());
                                       i_vt++;
                                       break;
                                  case 2:
                                       arr_vn=(float*)realloc(arr_vn,(i_vn+1)*sizeof(int));
                                       arr_vn[i_vn]=atof(buff.c_str());
                                       i_vn++;
                                       break;
                                  case 3:
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

int objLoadModel(char* file_name,
                 vector < Point3D <float> > &vec_v,
                 vector < Point2D <float> > &vec_vt,
                 vector < Point3D <float> > &vec_vn,
                 vector <float> &vec_f)
{
    float temp_v[3];
    float temp_vt[2];
    float temp_vn[3];
    string buff="";
    int i=0;
    int type=0;
    ifstream file(file_name);
    char c = file.get();
    while (file.good() && c!=EOF){
                     
          if (c=='f'){
               c = file.get();
               if(c==' ') type=3;
          }
          else{
              if (c=='v'){
                   c = file.get();
                       if (c=='n'){
                            i=0;
                            type=2;
                            c = file.get();
                       }
                       else if (c=='t'){
                            type=1;
                            i=0;
                            c = file.get();
                            //cout << "1:[" << c << "]";
                       }
                       else{
                            type=0;
                       }
              }
              else type = -1;
          }
          //i=0;
          while(((c!='f') && (c!='v')) && (c!=EOF)){
                 if(((c >= '0') && (c <= '9')) || (c == '.') || (c == '-')){
                        buff=buff+c;
                        c = file.get();
                        if (((c < '0') || (c > '9')) && (c != '.') && (c != '-')){
                             switch (type){
                                  case 0:
                                       temp_v[i]=atof(buff.c_str());
                                       i++;
                                       if(i==3){
                                           i=0;
                                           vec_v.push_back(Point3D <float> (temp_v[0],temp_v[1],temp_v[2]));
                                       }
                                       break;
                                  case 1:
                                       //cout<<i_vt<<"-"<<i<<"-"<<buff.c_str()<<endl;
                                       temp_vt[i]=atof(buff.c_str());
                                       i++;
                                       if(i==2){
                                           i=0;
                                           vec_vt.push_back(Point2D <float> (temp_vt[0],temp_v[1]));
                                       }
                                       break;
                                  case 2:
                                       temp_vn[i]=atof(buff.c_str());
                                       i++;
                                       if(i==3){
                                           i=0;
                                           vec_vn.push_back(Point3D <float> (temp_vn[0],temp_vn[1],temp_vn[2]));
                                       }
                                       break;
                                  case 3:
                                       vec_f.push_back(atof(buff.c_str()));
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
