//Game engine. Basic classes and functions.

#define PI 3.14159265

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
      
      void phys_fall(){}
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
public:
       player(): eyes_level(1.7){}
       
       float get_eyes_lvl(){
             return eyes_level;
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
