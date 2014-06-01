#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
using namespace std;

//------------------------------------------------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
int number_of_lines(string filename){
     string line;
     ifstream myfile(filename.c_str());
     int number_of_lines=0;
     while(getline(myfile, line)){
        ++number_of_lines;
     }
     return number_of_lines;
     myfile.close();
}
//------------------------------------------------------------------------------
string config_file="configs";
string user="";
//------------------------------------------------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
class curr_users{
      string curr_users_list[99];
      int i, i_temp;
public:
       curr_users(): i(0){};
//------------------------------------------------------------------------------       
       void view_users_list(){
            int i;
            cout << endl << "============================================" << endl;
            cout << "USER LIST (" << number_of_users() << " users total):" << endl << endl;
            for(i=0;i<number_of_users();i++){
                  cout << "User ID: " << i << " - " << get_user(i) << endl;
            }
            cout << "============================================" << endl;
       }
//------------------------------------------------------------------------------       
       int number_of_users(){
            return i;
       }
//------------------------------------------------------------------------------   
       void add_user(string user){
           curr_users_list[i]=user;
           i++;
       }
//------------------------------------------------------------------------------       
       string get_user(int i){
           return curr_users_list[i];
       }
//------------------------------------------------------------------------------ 
       int get_user(string user){
           for(i_temp=0;i_temp<i;i_temp++){
               if(curr_users_list[i_temp]==user){
                   break;
               };
           }
       return i_temp;
       }
//------------------------------------------------------------------------------
       void change_user(int i, string new_user){
           curr_users_list[i]=new_user;       
       }
//------------------------------------------------------------------------------
       void change_user(string user, string new_user){
           curr_users_list[get_user(user)]=new_user;
       }
//------------------------------------------------------------------------------       
       void delete_user(int i_temp){
           while(i_temp+2<i){
               change_user(i_temp,get_user(i_temp+1));
           }
       }
//------------------------------------------------------------------------------       
       void load_userlist(){
            string line;
            string filename="users.txt";
            ifstream myfile(filename.c_str());
            while(getline(myfile, line)){
                add_user(line);
            }
       }
//------------------------------------------------------------------------------       
       void save_userlist(){
            string filename="users.txt";
            ofstream myfile;
            myfile.open (filename.c_str());
            if (myfile.is_open()){
                int i;
                for(i=0;i<number_of_users();i++){
                    myfile << get_user(i) << endl;
                }
            myfile.close();
            }
       }
}users;
//------------------------------------------------------------------------------
//==============================================================================
//------------------------------------------------------------------------------
class config{
//Controls----------------------------------------------------------------------
    int key_move_forward;
    int key_move_left;
    int key_move_back;
    int key_move_right;
    int key_fire;
    int key_prone;
    int key_crouch;
    int key_jump;
//Settings----------------------------------------------------------------------
    int setting_fullscreen;
    int setting_screen_lenght;
    int setting_screen_height;
public:
//Constructors------------------------------------------------------------------
    config(){
        load_configs(config_file,1);
    }

    config(string config_file, string user=""){
        load_configs(config_file,0,user);
    }
//Getters (controls)------------------------------------------------------------
    int get_key_move_forward(){return key_move_forward;}
    int get_key_move_left(){return key_move_left;}
    int get_key_move_back(){return key_move_back;}
    int get_key_move_right(){return key_move_right;}
    int get_key_fire(){return key_fire;}
    int get_key_prone(){return key_prone;}
    int get_key_crouch(){return key_crouch;}
    int get_key_jump(){return key_jump;}
//Getters (settings)------------------------------------------------------------
    int get_setting_fullscreen(){return setting_fullscreen;}
    int get_setting_screen_lenght(){return setting_screen_lenght;}
    int get_setting_screen_height(){return setting_screen_height;}
//Setters (controls)------------------------------------------------------------
    void set_key_move_forward(int new_key){key_move_forward=new_key;}
    void set_key_move_left(int new_key){key_move_left=new_key;}
    void set_key_move_back(int new_key){key_move_back=new_key;}
    void set_key_move_right(int new_key){key_move_right=new_key;}
    void set_key_fire(int new_key){key_fire=new_key;}
    void set_key_prone(int new_key){key_prone=new_key;}
    void set_key_crouch(int new_key){key_crouch=new_key;}
    void set_key_jump(int new_key){key_jump=new_key;}
//Setters (settings)------------------------------------------------------------
    void set_setting_fullscreen(int new_key){setting_fullscreen=new_key;}
    void set_setting_screen_lenght(int new_key){setting_screen_lenght=new_key;}
    void set_setting_screen_height(int new_key){setting_screen_height=new_key;}
//Load Configs------------------------------------------------------------------
    void load_configs(string configs_file, bool default_configs, string user=""){
         if(default_configs){
//Default controls--------------------------------------------------------------
            set_key_move_forward(87);
            set_key_move_left(65);
            set_key_move_back(83);
            set_key_move_right(68);
            set_key_fire(1);
            set_key_prone(17);
            set_key_crouch(67);
            set_key_jump(32);
//Default settings--------------------------------------------------------------
            set_setting_fullscreen(0);
            set_setting_screen_lenght(640);
            set_setting_screen_height(480);
         }
         else{
             if(user!=""){
                  user="_"+user;
             }
             string filename=configs_file+user+".txt";
             ifstream file(filename.c_str());
             int i=0;
             string value="";
             string setting="";
             char c=file.get();
             while (file.good() && c!=EOF){
                  if(c=='/'){
                       while(c!='\n'){
                               c=file.get();
                       }
                  }
                  if(c=='\n'){
                       c=file.get();
                  }
                  while((c!=' ') && (c!='/') && (c!='\n') && (c!=EOF)){
                       value+=c;
                       c=file.get();
                  }
                  if((c==' ') || (c=='\n')){
                      if(setting != ""){
//Change values of keys---------------------------------------------------------
                            if (setting=="key_move_forward"){
                               set_key_move_forward(atoi(value.c_str()));
                            }
                            if (setting=="key_move_left"){
                               set_key_move_left(atoi(value.c_str()));
                            }
                            if (setting=="key_move_back"){
                               set_key_move_back(atoi(value.c_str()));
                            }
                            if (setting=="key_move_right"){
                               set_key_move_right(atoi(value.c_str()));
                            }
                            if (setting=="key_fire"){
                               set_key_fire(atoi(value.c_str()));
                            }
                            if (setting=="key_prone"){
                               set_key_prone(atoi(value.c_str()));
                            }
                            if (setting=="key_crouch"){
                               set_key_crouch(atoi(value.c_str()));
                            }
                            if (setting=="key_jump"){
                               set_key_jump(atoi(value.c_str()));
                            }
//Change values of settings-----------------------------------------------------
                            if (setting=="setting_fullscreen"){
                               set_setting_fullscreen(atoi(value.c_str()));
                            }
                            if (setting=="setting_screen_lenght"){
                               set_setting_screen_lenght(atoi(value.c_str()));
                            }
                            if (setting=="setting_screen_height"){
                               set_setting_screen_height(atoi(value.c_str()));
                            }
                            value="";
                      }
                      setting=value;
                      value="";
                      c=file.get();
                      i++;
                  }
            }
         file.close();
         }     
    }
//Save Configs------------------------------------------------------------------
    void save_configs(string config_file, string user=""){
        if(user!=""){
            user="_"+user;
        }
        string filename=config_file+user+".txt";
        ofstream myfile;
        myfile.open (filename.c_str());
        if (myfile.is_open()){
            myfile << "//Controls\n";
//Save keys---------------------------------------------------------------------
            myfile << "key_move_forward " << get_key_move_forward() << endl;
            myfile << "key_move_left " << get_key_move_left() << endl;
            myfile << "key_move_back " << get_key_move_back() << endl;
            myfile << "key_move_right " << get_key_move_right() << endl;
            myfile << "key_fire " << get_key_fire() << endl;
            myfile << "key_prone " << get_key_prone() << endl;
            myfile << "key_crouch " << get_key_crouch() << endl;
            myfile << "key_jump " << get_key_jump() << endl;
            myfile << "//Settings\n";
//Save settings-----------------------------------------------------------------
            myfile << "setting_fullscreen " << get_setting_fullscreen() << endl;
            myfile << "setting_screen_lenght " << get_setting_screen_lenght() << endl;
            myfile << "setting_screen_height " << get_setting_screen_height() << endl;
            myfile.close();
        }
    }
};

// BMW e36 & e30 FTW!
