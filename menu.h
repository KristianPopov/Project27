class button{
   float x,y;
   float height, width;
   bool slct,enbl,prss;
   
public:
   button(
       float init_x,
       float init_y,
       float init_width,
       float init_height,
       float init_state)
       :x(init_x),
       y(init_y),
       width(init_width),
       height(init_height),
       enbl(init_state),
       slct(false),
       prss(false){};  
       
   float get_x(){
         return x;
   }
   
   void set_x(float new_x){
        x=new_x;
   }
   
   float get_y(){
         return y;
   }
   
   void set_y(float new_y){
        y=new_y;
   }             
       
   float get_height(){
         return height;
   }
   
   void set_height(float new_height){
        height=new_height;
   }
   
   float get_width(){
         return width;
   }
   
   void set_width(float new_width){
        width=new_width;
   }
   
   bool selected(){
        return slct;
   }
   
   void select(){
        slct=true;
   } 
   
   void deselect(){
        slct=false; 
   }
   
   bool enabled(){
        return enbl;
   }
   
   void enable(){
        enbl=true;
   }
   
   void disable(){
        enbl=false;
   }
   
   bool pressed(){
        return prss;
   }
   
   int detect(float mouse_x,float mouse_y,bool click){
       if((enbl)&&(mouse_x>x)&&(mouse_x<x+width)&&(mouse_y>y)&&(mouse_y<y+height)){
           slct=true;
           if(click){
              prss=true;
              return 2;
           }
           else{ 
              prss=false;
              return 1;
           }
       }
       else{
           slct=false;
           return 0;
       }
   }
};
