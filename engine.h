//Game engine. Basic classes and functions.
#include <iostream>
#include <math.h>
#define PI 3.14159265

using namespace std;

void Swap(float &a,float &b){
     float temp;
     temp=a;
     a=b;
     b=temp;
}

float Check(bool SorG, float a, float b, float c){
      if(SorG){ 
         if(a>b){
            if(a>c) return a;
            else return c;
         }
         else{ 
           if(b>c) return b;
           else return c;
         }
      }
      else{
         if(a<b){
            if(a<c) return a;
            else return c;
         }
         else if(b<c) return b;
              else return c;
      }
}

int Check_ID(bool SorG, float a, float b, float c){
      if(SorG){ 
         if(a>b){
            if(a>c) return 0;
            else return 2;
         }
         else{ 
           if(b>c) return 1;
           else return 2;
         }
      }
      else{
         if(a<b){
            if(a<c) return 0;
            else return 2;
         }
         else if(b<c) return 1;
              else return 2;
      }
}

float modul(float a){
      if(a<0) return -a;
      else return a;
}

int Check_sign(float a, float b){
    if(a>b) return 1;
    else return -1;   
}

class game_object{
      float x,y,z; //coordinates of the object in the game
      float angle_x,angle_y,angle_z; //angles by all axis
      float mass; //mass of the object
      
public:
       // all constructors
      game_object() 
      : x(0), y(0), z(0), angle_x(0), angle_y(0), angle_z(0), mass(0) 
      {}
      
      game_object(float init_x, float init_y, float init_z) 
      : x(init_x), y(init_y), z(init_z) 
      {}
      
      game_object(
          float init_x, 
          float init_y, 
          float init_z,
          float init_angle_x,
          float init_angle_y,
          float init_angle_z)
      : x(init_x),
        y(init_y), 
        z(init_z),
        angle_x(init_angle_x),
        angle_y(init_angle_y), 
        angle_z(init_angle_z)
      {}     
      
      game_object(
          float init_x, 
          float init_y, 
          float init_z,
          float init_angle_x,
          float init_angle_y,
          float init_angle_z,
          float init_mass) 
      : x(init_x),
        y(init_y), 
        z(init_z),
        angle_x(init_angle_x),
        angle_y(init_angle_y), 
        angle_z(init_angle_z),
        mass(init_mass)
      {}
      
      //getters and setters

      float get_x(){
           return x;
      }
      
      float get_y(){
           return y;
      }

      float get_z(){
           return z;
      }
      
      float get_angle_x(){
           return angle_x;
      }
      
      float get_angle_y(){
           return angle_y;
      }

      float get_angle_z(){
           return angle_z;
      }
      
      float get_mass(){
           return mass;
      }      
      
      void set_x(float new_x){
           x=new_x;
      }
      
      void set_y(float new_y){
           y=new_y;
      }
      
      void set_z(float new_z){
           z=new_z;
      }
      
      void set_angle_x(float new_angle_x){
           angle_x=new_angle_x;
      }
      
      void set_angle_y(float new_angle_y){
           angle_y=new_angle_y;
      }
      
      void set_angle_z(float new_angle_z){
           angle_z=new_angle_z;
      }
      
      void set_mass(float new_mass){
           mass=new_mass;
      }
      
      //physics of the object
      
      void phys_fall(float Vo,float G,int timer){
           z+=(Vo/1000 - G*0.000001*timer)/2;
      }
};

class camera: public game_object{
public:
      void set_angles(float azimuth,float elevation){ //set angles of the camera
           set_angle_z(azimuth);
           set_angle_y(elevation*sin(azimuth*PI/180));
           set_angle_x(elevation*cos(azimuth*PI/180));
      }
      void set_coords(float new_x,float new_y, float new_z){ // set coords of the camera
           set_x(new_x);
           set_y(new_y);
           set_z(new_z);                    
      }
};

class timer{
       int t_counter;
       
public:
       void reset(){
            t_counter=0;
       }
       void set(int new_time){
            t_counter = new_time;
       }
       
       void inc(){
            t_counter++;
       }
       
       int get(){
           return t_counter;
       }
};

class player: public camera{
      float eyes_level;
      float lenght, width, height;
      
public:
       player(): eyes_level(1.7), lenght(0.6), width(0.6), height(1.8){}
       
       float get_eyes_lvl(){
             return eyes_level;
       }
       
       void set_eyes_lvl(float new_lvl){
            eyes_level=new_lvl;
       }
       
              float  get_lenght() {
          return lenght;       
       }
       
       float  get_width() {
          return width;       
       }
       
       float  get_height() {
          return height;        
       }


       void  set_lenght(float new_lenght) {
          lenght = new_lenght;         
       }

       void  set_width(float new_width) {
          width = new_width;         
       }
       
       void  set_height(float new_height) {
          height = new_height;         
       }  
       
       //minimum time is 1 milisecond in physics
       //so we need to convert m/s to m/ms
       
       void move_forward(float speed){
            set_y(get_y()+(speed/1000)*cos(get_angle_z()*PI/180)); 
            set_x(get_x()+(speed/1000)*sin(get_angle_z()*PI/180));
       }
       
       void move_back(float speed){
            set_y(get_y()-(speed/1000)*cos(get_angle_z()*PI/180)); 
            set_x(get_x()-(speed/1000)*sin(get_angle_z()*PI/180));            
       }
       
       void move_left(float speed){
            set_y(get_y()+(speed/1000)*sin(get_angle_z()*PI/180)); 
            set_x(get_x()-(speed/1000)*cos(get_angle_z()*PI/180));            
       }
       
       void move_right(float speed){
            set_y(get_y()-(speed/1000)*sin(get_angle_z()*PI/180)); 
            set_x(get_x()+(speed/1000)*cos(get_angle_z()*PI/180));            
       }
       
       void do_crouch(bool updown, float transition, float min_lvl, float max_lvl){
            if(updown){
                if (get_height() < max_lvl){
                   set_height(get_height()+(transition)/1000);
                   set_eyes_lvl(get_eyes_lvl()+(transition)/1000);
                }
            }
            else{
                if (get_height() > min_lvl){
                   set_height(get_height()-(transition)/1000);
                   set_eyes_lvl(get_eyes_lvl()-(transition)/1000);
                }                     
            }                           
       }
       
      void do_prone(bool updown, float transition){
            if(updown){
/*              if (get_height() < max_lvl){
                   set_height(get_height()+(transition)/1000);
                   set_eyes_lvl(get_eyes_lvl()+(transition)/1000);
                }*/
            }
            else{
/*              if (get_height() > min_lvl){
                   set_height(get_height()-(transition)/1000);
                   set_eyes_lvl(get_eyes_lvl()-(transition)/1000);
                } */                  
            }                           
       } 
       
};

class map : public game_object {
      float lenght, width;

public:
       map(float init_lenght, float init_width): lenght(init_lenght), width(init_width)  {
       }
       
       float  get_lenght() {
          return lenght;       
       }
       
       float  get_width() {
          return width;       
       }

       void  set_lenght(float new_lenght) {
          lenght = new_lenght;         
       }

       void  set_width(float new_width) {
          width = new_width;         
       }
};

class movable_objects : public game_object{
      float lenght, width, height;


public:
       movable_objects(float init_lenght, float init_width, float init_height): lenght(init_lenght), width(init_width), height(init_height)  {
       } 
       
       movable_objects(
                       float init_x, 
                       float init_y, 
                       float init_z,
                        
                       float init_lenght, 
                       float init_width, 
                       float init_height)
                       :
                       lenght(init_lenght), 
                       width(init_width), 
                       height(init_height){
                                           set_x(init_x);
                                           set_y(init_y);
                                           set_z(init_z);
             
       } 
       
       float  get_lenght() {
          return lenght;       
       }
       
       float  get_width() {
          return width;       
       }
       
       float  get_height() {
          return height;        
       }


       void  set_lenght(float new_lenght) {
          lenght = new_lenght;         
       }

       void  set_width(float new_width) {
          width = new_width;         
       }
       
       void  set_height(float new_height) {
          height = new_height;         
       }     
};

class colison{
public:
       colison(){
          coords[0][0]=-2;
          coords[0][1]=0;
          coords[0][2]=0;
          coords[1][0]=2;
          coords[1][1]=0;
          coords[1][2]=0;
          coords[2][0]=2;
          coords[2][1]=2;
          coords[2][2]=5;

          indices[0]=0;
          indices[1]=1;
          indices[2]=2;  
       }
       
       float coords[3][3];
       float indices[3];

};

int PolygonDetect(float &temp,
                    float obj_x, float obj_y, float obj_z,
                    float A_x, float A_y, float A_z,
                    float B_x, float B_y, float B_z,
                    float C_x, float C_y, float C_z)
{
         if((obj_x+0.001>=Check(0, A_x, B_x, C_x)) && 
           (obj_x-0.001<=Check(1, A_x, B_x, C_x)) &&
           (obj_y-0.001<=-Check(0,A_y, B_y, C_y)) &&
           (obj_y+0.001>=-Check(1,A_y, B_y, C_y)) &&
           (obj_z+0.001>=Check(0,A_z, B_z, C_z)) &&
           (obj_z-0.001<=Check(1,A_z, B_z, C_z)))
           {                                                                         
                if((C_y>B_y)&&(C_y<A_y)){
                        Swap(A_x,C_x);
                        Swap(A_y,C_y);
                        Swap(A_z,C_z);                
                }
                else{
                    if((C_y<B_y)&&(C_y>A_y)){
                        Swap(B_x,C_x);
                        Swap(B_y,C_y);
                        Swap(B_z,C_z);
                    }               
                }                
           
                if(B_x<A_x){
                    Swap(A_x,B_x);
                    Swap(A_y,B_y);
                    Swap(A_z,B_z);
                }

                                                                                         
                if((obj_x<B_x-
                modul(B_x-C_x)*Check_sign(B_x,C_x)*
                (-obj_y-B_y)/(C_y - B_y)) &&
                
                (obj_x>A_x+
                modul(A_x-C_x)*Check_sign(C_x,A_x)*
                (-obj_y-A_y)/(C_y - A_y)) &&
                
                (((-obj_y>B_y+
                modul(A_y-B_y)*Check_sign(A_y,B_y)*
                (-obj_x-A_x)/(B_x - A_x)) &&
                ((C_y>A_y) || (C_y>B_y))) ||
                
                ((-obj_y<B_y+
                modul(A_y-B_y)*Check_sign(A_y,B_y)*
                (-obj_x-A_x)/(B_x - A_x)) &&
                ((C_y<A_y) && (C_y<B_y)))))
                
                {
                   temp = B_z+
                   modul(C_z-B_z)*
                   Check_sign(C_z,B_z)*
                   (-obj_y-(B_y+
                   modul(A_y-B_y)*Check_sign(A_y,B_y)*
                   (-obj_x-A_x)/(B_x - A_x)))/(C_y -(B_y+
                   modul(A_y-B_y)*Check_sign(A_y,B_y)*
                   (-obj_x-A_x)/(B_x - A_x)))+
                   modul(B_z-(A_z))*
                   Check_sign(A_z,B_z)*
                   (-obj_x-A_x)/(B_x - A_x);
                   return 0;
                }
                else
                {
                   if((A_x==B_x)&&(B_y==C_x)) return 1;
                   if((A_y==B_y)&&(B_y==C_y)) return 2;
                   if((A_z==B_z)&&(B_z==C_z)) return 3;                       
                   else return -1;
                }
           }
           else return -1;
}

float speed_trans(float acc,float Vo,float Vmax){
      
}
