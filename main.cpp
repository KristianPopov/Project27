/**************************
 * Includes
 *
 **************************/

#include <time.h>
#include <windows.h>
#include <process.h>
#include "engine.h" //Game engine
#include "menu.h"
#include "GL_Rendering.h"
#include <vector>
#include "obj_loader.h"
#include "col_loader.h"
#include "configs.h"
#include <string>


/**************************
 * Function Declarations
 *
 **************************/
void XY2Polar(RECT ds,int scr_x,int scr_y, float &azimuth,float &elevation,unsigned int res_x, unsigned int res_y, float sens);
char* SpecKeys(char key);
void ChangeResolution(config &configs,RECT ds,char res_type);
void Phys_Thread(void* PARAMS); 
void Timing_Thread(void* PARAMS);
void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC);
void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC);
LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
WPARAM wParam, LPARAM lParam);
/**************************
 * Global Objects Declarations
 *
 **************************/
 
config conf; 

SYSTEMTIME phystime;
POINT pt;

int score=0;
target ciganin(-2,-2,0,0.3,1,1.5);
target ciganin2(5,3,1,0.3,1,1.5);
projectile proj[10000];
int count_proj=0;
colison col;
player main_player;
map test_map(50,50);
movable_objects movable[10] = {movable_objects(-10, -5, 0, 1, 1, 1), 
                               movable_objects(-10, 20, 0, 1, 1, 2),
                               movable_objects(-4, 7, 0, 2, 2, 2),
                               movable_objects(-4, 4, 0, 2, 1, 1),
                               movable_objects(12, 18, 0, 0.5, 0.5, 0.5),
                               movable_objects(10, -15, 0, 1, 1, 1),
                               movable_objects(4, -23, 0, 2, 2, 2),
                               movable_objects(-8, -14, 0, 2, 1, 1),
                               movable_objects(14, -18, 0, 0.5, 0.5, 0.5),
                               movable_objects(21, 0, 0, 1, 1, 1),};
timer Timer;
timer PhysTimer;
timer FireTimer;

bool fullscreen = false;
unsigned int res_x=800;
unsigned int res_y=600;


bool move_forward=0, move_back=0, move_right=0, move_left=0, crouch=0, jump=0, prone=0, fire=0;

bool advert=true;
bool ingame=false;
bool inmenu=true;
bool menu_power=true;
bool click=false;
bool key_return=false;
bool opt_menu=false;

bool key_pressed=false;
bool mouse_key_pressed=false;
char num_key_pressed;

float test_x=0,test_y=0;
float test_width=0.1,test_height=0.1;

bool boom,effect,eff2;

float temp_x,temp_y,temp_z;


    float* col_v;
    int* col_f;
    int col_i_v=0, col_i_f=0;

/**************************
 * WinMain
 *
 **************************/

int WINAPI WinMain (HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine,
                    int iCmdShow)
{
    WNDCLASS wc;
    HWND hWnd;
    HDC hDC;
    HGLRC hRC;        
    MSG msg;
    BOOL bQuit = FALSE;
    ShowCursor(FALSE);
    float theta = 0.0f;
    POINT pt;
    
    float colorBlue[] = { 0.0f, 0.0f, 1.0f, 0.5f };
    float specReflection[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float ambientLight0[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    float diffuseLight0[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    float specularLight0[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float position0[] = { 100, 100, 0,  1 };
    float azth=0,elev=0;
    GLfloat maxAniso = 16.0f;
    
    Timer.reset();
    PhysTimer.reset();
    FireTimer.reset();
    
    bool adv_stop=true;
    
    conf.load_configs("config",0,"");
    
    fullscreen=conf.get_setting_fullscreen();
    res_x=conf.get_setting_screen_lenght();
    res_y=conf.get_setting_screen_height();
    
    main_player.set_coords(0,0,10);
//    ciganin.set_coords(0,0,0);
    //ciganin.set_dimensions();
    
    //waveOutSetVolume(0,-1);
    
    camera cam;
    
    button start_game_btn(-0.78,-0.37,0.52,0.15,1);
    button options_btn(-0.78,-0.56,0.52,0.15,1);
    button exit_btn(-0.64,-0.82,0.25,0.25,1);
    button restart_opt(0.04,-0.14,1.26,0.04,1);
    button close_opt(0.04,-0.2,1.26,0.04,1);
    button res_detect_btn(0.97,0.64,0.33,0.04,1);
    button change_res_btn(0.04,0.64,0.92,0.04,1);
    button full_scr_opt_btn(0.04,0.57,1.26,0.04,1);
    button move_f_opt_btn(0.04,0.41,1.26,0.04,1);
    button move_b_opt_btn(0.04,0.35,1.26,0.04,1);
    button move_l_opt_btn(0.04,0.29,1.26,0.04,1);
    button move_r_opt_btn(0.04,0.23,1.26,0.04,1);
    button prone_opt_btn(0.04,0.17,1.26,0.04,1);
    button crouch_opt_btn(0.04,0.11,1.26,0.04,1);
    button jump_opt_btn(0.04,0.05,1.26,0.04,1);
    button fire_opt_btn(0.04,-0.01,1.26,0.04,1);
    
    
    char res_type=0;
    char full_scr_flag=conf.get_setting_fullscreen();
    
    /* register window class */
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "GLSample";
    RegisterClass (&wc);
    
    
    if (fullscreen)												// Attempt Fullscreen Mode?
    {
    	DEVMODE dmScreenSettings;								// Device Mode
    	memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
    	dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
    	dmScreenSettings.dmPelsWidth	= res_x;				// Selected Screen Width
    	dmScreenSettings.dmPelsHeight	= res_y;				// Selected Screen Height
    	dmScreenSettings.dmBitsPerPel	= 32;					// Selected Bits Per Pixel
    	dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

    	// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
    	if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
    	{
    		// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
    		if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
    		{
    			fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
    		}
    		else
    		{
    			// Pop Up A Message Box Letting User Know The Program Is Closing.
    			MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
    			return FALSE;									// Return FALSE
    		}
    	}
    }
    
    /* create main window */
    RECT ds;
    GetWindowRect(GetDesktopWindow(), &ds);
    hWnd = CreateWindow (
      "GLSample", "OpenGL Sample", 
      WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
      (ds.right/2-res_x/2), (ds.bottom/2-res_y/2), res_x, res_y,
      NULL, NULL, hInstance, NULL);
    LONG lStyle = GetWindowLong(hWnd, GWL_STYLE);
    lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
    SetWindowLong(hWnd, GWL_STYLE, lStyle);
    SetWindowPos(hWnd, NULL, 0,0,0,0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);    

    /* enable OpenGL for the window */
    EnableOpenGL (hWnd, &hDC, &hRC);
    
    model pametnik;
    model bottle;
    model model_target;
    model car;
    model hands;
    float menu_x,menu_y;
    
     float Ssx=91; //Start Screen moving background Y position
     float Ssy=91; //Start Screen moving background Y position
     float Sspomx,Sspomy; //X,Y correction units per one milisecond
     float Ssi=1;
     int bgtone=2880;
     int pow_mess_time=0;
     char* conv_c_str = (char*)malloc(8);
     string conv_str="";
    
    int result = objLoadModel("pametnik2.obj", pametnik.arr_v, pametnik.arr_vt, pametnik.arr_vn, pametnik.arr_f, pametnik.num_v, pametnik.num_vt, pametnik.num_vn, pametnik.num_f);
    objLoadModel("models/trabant_low.obj", car.arr_v, car.arr_vt, car.arr_vn, car.arr_f, car.num_v, car.num_vt, car.num_vn, car.num_f);
    objLoadModel("models/bottle.obj", bottle.arr_v, bottle.arr_vt, bottle.arr_vn, bottle.arr_f, bottle.num_v, bottle.num_vt, bottle.num_vn, bottle.num_f);
    objLoadModel("models/target1.obj", model_target.arr_v, model_target.arr_vt, model_target.arr_vn, model_target.arr_f, model_target.num_v, model_target.num_vt, model_target.num_vn, model_target.num_f);
    objLoadModel("models/hands_makarov.obj", hands.arr_v, hands.arr_vt, hands.arr_vn, hands.arr_f, hands.num_v, hands.num_vt, hands.num_vn, hands.num_f);
    LoadColison("pametnik.col",col_v,col_f, col_i_v,col_i_f);
    
    PlaySound("Sounds/Powered.wav", NULL, SND_ASYNC);
    
    /*initialize OpenGL */
    GL_init(res_x,res_y);

    /* Load some textures */
    GLuint sad = loadTexture(true, "Textures/powered_tex.dds" );
    GLuint crate = loadTexture(true, "Textures/crate001.dds" );
    GLuint fence = loadTexture(true, "Textures/fence.dds" );
    GLuint ground = loadTexture(true, "Textures/ground.dds" );
    GLuint crosshair = loadTexture(true, "Textures/crosshair_2.dds" );
    GLuint bullet = loadTexture(true, "Textures/bullet.dds" );
    GLuint tiles = loadTexture(true, "Textures/tiles.dds" );
    GLuint cracked_monument = loadTexture(true, "Textures/cracked_monument.dds" );
    GLuint cracked_concrete2 = loadTexture(true, "Textures/cracked_concrete2.dds" );
    GLuint menu_tex = loadTexture(true, "Textures/menu.dds" );
    GLuint old_house = loadTexture(true, "Textures/old_house.dds" );
    GLuint old_shop = loadTexture(true, "Textures/old_shop.dds" );
    GLuint background = loadTexture(true, "Textures/background.dds" );
    GLuint rain = loadTexture(true, "Textures/rain.dds" );
    GLuint cursor = loadTexture(true, "Textures/cursor.dds" );
    GLuint lamp = loadTexture(true, "Textures/lamp.dds" );
    GLuint lamp_on = loadTexture(true, "Textures/lamp_on.dds" );
    GLuint font_c = loadTexture(true, "Textures/font_cyrilic.dds" );
    GLuint font = loadTexture(true, "Textures/font.dds" );
    GLuint red_lamp_off = loadTexture(true, "Textures/red_lamp_off.dds" );
    GLuint red_lamp_on = loadTexture(true, "Textures/red_lamp_on.dds" );
    GLuint newvehicleenv128 = loadTexture(true, "Textures/newvehicleenv128.dds" );
    GLuint target_tex = loadTexture(true, "Textures/target.dds" );
    GLuint target2_tex = loadTexture(true, "Textures/target2.dds" );
    GLuint trabi_tex = loadTexture(true, "Textures/trabi_tex.dds" );
    GLuint hands_tex = loadTexture(true, "Textures/hands_tex.dds" );
    

    GLuint skybox[5] = {
           loadTexture(true, "Textures/skybox_top.dds" ),
           loadTexture(true, "Textures/skybox_left.dds" ),
           loadTexture(true, "Textures/skybox_front.dds" ),
           loadTexture(true, "Textures/skybox_right.dds" ),
           loadTexture(true, "Textures/skybox_back.dds" )
    };
    
    /*starts timing thread*/
    //HANDLE h = (HANDLE)_beginthread((void(*)(void*))Timing_Thread, 0, NULL);
    /*starts physics thread*/
    HANDLE h2 = (HANDLE)_beginthread((void(*)(void*))Phys_Thread, 0, NULL);

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage (&msg);
                DispatchMessage (&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */
        	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	        glLoadIdentity();
	        
	     if((!menu_power)&&(bgtone>140)) bgtone-=20;
         else if((menu_power)&&(bgtone<2880)) bgtone+=20;
         
         if((eff2)&&(ingame)){
         glEnable(GL_FOG);
           {
             GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0};
        
              glFogi (GL_FOG_MODE, GL_EXP);
              glFogfv (GL_FOG_COLOR, fogColor);
              glFogf (GL_FOG_DENSITY, 0.15);
              glHint (GL_FOG_HINT, GL_DONT_CARE);
              glFogf (GL_FOG_START, 1.0);
              glFogf (GL_FOG_END, 5.0);
           }
        }
        else glDisable(GL_FOG);
         
           GetCursorPos( &pt );
           
           //cout<<pt.x<<' '<<pt.y<<endl;
             
             
         if(ingame){
             XY2Polar(ds,pt.x,pt.y,azth,elev,res_x,res_y,0.1);
             
             glPushMatrix();
              if(FireTimer.get()<200)glRotatef((FireTimer.get()+10)/50,1,0,0);
              RenderModel(0,-0.25,-0.35,hands,hands_tex,0,0,0,0,0,0,0,0);
             glPopMatrix();
             
             glPushMatrix ();
             
             glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
              glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);
             
             main_player.set_angles(azth,elev);
          
             CameraUse(cam,azth,elev,main_player.get_x(),main_player.get_y(),main_player.get_eyes_lvl()+main_player.get_z());
    
             
             //cout<<main_player.get_x()<<' '<<main_player.get_y()<<' ' << main_player.get_z()<<endl;
             
                 glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
                	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
                	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight0);
                 glLightfv(GL_LIGHT0, GL_POSITION, position0);
                 
              RenderSkybox(skybox);
              RenderMap(fence,ground);
              glColor3f(1,1,1);     
              RenderMovable(movable,crate);
              //RenderColison(col);
              
              RenderModel(0,0.01,0,pametnik,tiles,cracked_monument,cracked_concrete2,0,0,90,27,0,0);
              RenderModel(ciganin.get_x(),ciganin.get_z(),-ciganin.get_y(),model_target,target_tex,0,0,0,0,0,0,0,0);
              RenderModel(ciganin2.get_x(),ciganin2.get_z(),-ciganin2.get_y(),model_target,target2_tex,0,0,0,0,0,0,0,0);
 /*             glPushMatrix();
              glScalef(0.01,0.01,0.01);
              glRotatef(90,1,0,0);
              RenderModel(1000,1000,-100,car,trabi_tex,0,0,0,0);
              glPopMatrix();*/
              RenderProjectile(proj,main_player,bullet,count_proj);
              
/*              glPushMatrix();
            //glTranslatef(0,0.5,0);
              //glBlendFunc(GL_SRC_ALPHA,GL_ONE);
              glColor4f(1,1,1,1);
              glBlendFunc(GL_ONE, GL_ONE);
              glEnable(GL_TEXTURE_GEN_S);
              glEnable(GL_TEXTURE_GEN_T);
              glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
              glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
              //RenderModel(i_f,arr_v,arr_vt,arr_vn,arr_f,tiles,cracked_monument,cracked_concrete2,90,27);
              RenderModel(0,0.5,0,bottle,newvehicleenv128,0,0,0,0);
              glDisable(GL_TEXTURE_GEN_T);
              glDisable(GL_TEXTURE_GEN_S);
              glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
              //cout<<proj.get_x()<<' '<<proj.get_y()<<' '<<proj.get_z()<<endl;
              glPopMatrix();*/
              glPopMatrix();
          }

          glPushMatrix();
           glScalef(0.04,0.04,1);
           
           glDisable(GL_LIGHTING);
           WriteText(-1.0,-0.7,0.205,"UBIJSTWA:",0,0,0,1,NULL,font_c);
           WriteText(-1.0,-0.7,0.2,"UBIJSTWA:",1,1,1,1,NULL,font_c);
           itoa(score,conv_c_str,10);
           WriteText(-0.64,-0.65,0.305,conv_c_str,0,0,0,1,NULL,font_c);
           WriteText(-0.64,-0.65,0.3,conv_c_str,1,1,1,1,NULL,font_c);
           glEnable(GL_LIGHTING);
           
           if(inmenu){
                menu_x=((float)pt.x-(float)ds.right/2)/(res_x/(((float)res_x/(float)res_y)*2));
                menu_y=-((float)pt.y-(float)ds.bottom/2)/(res_x/((float)((float)res_x/(float)res_y)*2));
                
                //cout<<menu_x<<' '<<menu_y<<endl;     
                    if((click)&&(menu_x>-0.77)&&(menu_x<-0.52)&&(menu_y<0.45)&&(menu_y>0.20)){
                          click=false;
                          pow_mess_time = phystime.wMinute*60 + phystime.wSecond;
                          PlaySound("Sounds/painstick2.wav", NULL, SND_ASYNC);
                          menu_power=false;
                          if(opt_menu) opt_menu=false;                  
                    }
                    
                    if(start_game_btn.detect(menu_x,menu_y,click)==2){
                      inmenu=false;
                      ingame=true;
                      PlaySound("Sounds/night_hunter.wav", NULL, SND_ASYNC);
                    }
                    if(options_btn.detect(menu_x,menu_y,click)==2){
                      opt_menu=true;
                    }
                    
                    if(exit_btn.detect(menu_x,menu_y,click)==2){
                       PostQuitMessage(0);
                    }            
                      
                glPushMatrix();
                glTranslatef(Ssx/100,Ssy/100,0);
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, old_shop);
                glBegin(GL_QUADS);
                glColor4f(1.0f, 1.0f, 1.0f, 0.5f); 
                glTexCoord2f(0.0, 1.0);
                glVertex3f(-2.6f, -2.0f, -0.12);
                glTexCoord2f(1.0, 1.0);
                glVertex3f(2.6f, -2.0f, -0.12);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(2.6f, 2.0f,  -0.12);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(-2.6f, 2.0f, -0.12);
                glEnd();
                glDisable(GL_TEXTURE_2D);
                glPopMatrix();
                
                if (Ssx>90) Sspomx=-0.01;
                if (Ssx<(-90)) Sspomx=0.01;
                if (Ssy>90) Sspomy=-0.001;
                if (Ssy<(-90)) Sspomy=0.001;
                Ssx+=Sspomx;
                Ssy+=Sspomy;
                Ssi+=0.01;
                
                glBindTexture(GL_TEXTURE_2D, rain);
                glEnable(GL_TEXTURE_2D);
                glBegin(GL_QUADS);
                glColor4f(1,1,1,0.7);           
                glTexCoord2f(0.0, 0.5-Ssi);
                glVertex3f(-0.8f, -1.0f, -0.11);
                glTexCoord2f(0.5, 0.5-Ssi);
                glVertex3f(1.8f, -1.0f, -0.11);
                glTexCoord2f(0.5, 0.0-Ssi);
                glVertex3f(1.8f, 1.0f,  -0.11);
                glTexCoord2f(0.0, 0.0-Ssi);
                glVertex3f(-0.8f, 1.0f, -0.11);
                glEnd();
                glDisable(GL_TEXTURE_2D);
                
                glBindTexture(GL_TEXTURE_2D, background);
               glEnable(GL_TEXTURE_2D);
               glBegin(GL_QUADS);
                glColor4f(1.0f, 1.0f, 1.0f, 0.7f); 
                glTexCoord2f(0.0, 1.0);
                glVertex3f(-0.8f, -1.0f, -0.11);
                glTexCoord2f(1.0, 1.0);
                glVertex3f(1.8f, -1.0f, -0.11);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(1.8f, 1.0f,  -0.11);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(-0.8f, 1.0f, -0.11);
               glEnd();
               glDisable(GL_TEXTURE_2D);
                      
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, menu_tex);
                glBegin(GL_QUADS);
                glColor4f(cos(bgtone*PI/180), cos(bgtone*PI/180), cos(bgtone*PI/180), 1.0f); 
                glTexCoord2f(0.0, 1.0);
                glVertex3f(-1.3f, -1.0f, -0.11);
                glTexCoord2f(1.0, 1.0);
                glVertex3f(1.3f, -1.0f, -0.11);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(1.3f, 1.0f,  -0.11);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(-1.3f, 1.0f, -0.11);
                glEnd();
                glDisable(GL_TEXTURE_2D);
                
                glBegin(GL_QUADS);
                glColor4f(0.0f, 0.0f, 0.0f, 1.0f); 
                glVertex3f(-1.8f, -1.0f, -0.11);
                glVertex3f(-1.29f, -1.0f, -0.11);
                glVertex3f(-1.29f, 1.0f,  -0.11);
                glVertex3f(-1.8f, 1.0f, -0.11);
                glEnd();
                
                /*glPushMatrix();
                glTranslatef(test_x,test_y,0);
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, 0);
                glBegin(GL_QUADS);
                glColor4f(1.0f, 1.0f, 1.0f, 0.3f); 
                glTexCoord2f(0.0, 1.0);
                glVertex3f(0, test_height, -0.11);
                glTexCoord2f(1.0, 1.0);
                glVertex3f(test_width, test_height, -0.11);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(test_width, 0,  -0.11);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(0, 0, -0.11);
                glEnd();
                glDisable(GL_TEXTURE_2D);
                glPopMatrix();*/
                
                //cout<<test_x<<' '<<test_y<<' '<<test_width<<' '<<test_height<<endl;
                
                RenderButton(start_game_btn,lamp,lamp_on,bgtone);
                RenderButton(options_btn,lamp,lamp_on,bgtone);
                RenderButton(exit_btn,red_lamp_off,red_lamp_on,bgtone);
                RenderButton(restart_opt,0,0,0,0,1,1,1,0.3);
                RenderButton(close_opt,0,0,0,0,1,1,1,0.3);
                RenderButton(res_detect_btn,0,0,0,0,1,1,1,0.3);
                RenderButton(change_res_btn,0,0,0,0,1,1,1,0.3);
                RenderButton(full_scr_opt_btn,0,0,0,0,1,1,1,0.3);
                RenderButton(move_f_opt_btn,0,0,0,0,1,1,1,0.3);
                RenderButton(move_b_opt_btn,0,0,0,0,1,1,1,0.3);
                RenderButton(move_l_opt_btn,0,0,0,0,1,1,1,0.3);
                RenderButton(move_r_opt_btn,0,0,0,0,1,1,1,0.3);
                RenderButton(prone_opt_btn,0,0,0,0,1,1,1,0.3);
                RenderButton(crouch_opt_btn,0,0,0,0,1,1,1,0.3);
                RenderButton(jump_opt_btn,0,0,0,0,1,1,1,0.3);
                RenderButton(fire_opt_btn,0,0,0,0,1,1,1,0.3);
                
                WriteText(-0.48,-0.21,0.25,"NOWA",0,0,0,1,NULL,font_c);
                WriteText(-0.48,-0.26,0.25,"IGRA",0,0,0,1,NULL,font_c);
                WriteText(-0.49,-0.37,0.3,"OPCII",0,0,0,1,NULL,font_c);
                WriteText(-0.495,-0.565,0.23,"IZHOD",0,0,0,1,NULL,font_c);
                
                if((!menu_power)&&(phystime.wMinute*60+phystime.wSecond-pow_mess_time>20)){
                   WriteText(-1.0,-0.7,0.2,"NATISNETE",1,1,1,1,NULL,font_c);
                   WriteText(-0.68,-0.7,0.2,"[ENTER], ",1,1,1,1,NULL,font);
                   WriteText(-0.4,-0.7,0.2,"ZA DA WYZSTANOWITE ZAHRANWANETO",1,1,1,1,NULL,font_c);
                   if(key_return){
                      menu_power=true;
                      PlaySound("Sounds/menu.wav", NULL, SND_ASYNC|SND_LOOP);                      
                   }
                }
                
                if(opt_menu){
                       if(restart_opt.detect(menu_x,menu_y,click)==2) conf.load_configs("",1,"");                   
                       if(close_opt.detect(menu_x,menu_y,click)==2){
                           conf.save_configs("config","");
                           close_opt.deselect();
                           opt_menu=false;
                       }
                       if(res_detect_btn.detect(menu_x,menu_y,click)==2){
                           conf.set_setting_fullscreen(1);
                           conf.set_setting_screen_height(ds.bottom);
                           conf.set_setting_screen_lenght(ds.right);
                       } 
                      
                       if(change_res_btn.detect(menu_x,menu_y,click)==2){
                           res_type++;
                           ChangeResolution(conf,ds,res_type);
                           click=false;                   
                       }
                       
                       if(full_scr_opt_btn.detect(menu_x,menu_y,click)==2){
                           full_scr_flag++;
                           conf.set_setting_fullscreen(full_scr_flag%2);
                           click=false;                   
                       }

                       if(move_f_opt_btn.detect(menu_x,menu_y,click)==1){
                           if(key_pressed) conf.set_key_move_forward(num_key_pressed);          
                       }
                       
                       if(move_b_opt_btn.detect(menu_x,menu_y,click)==1){
                           if(key_pressed) conf.set_key_move_back(num_key_pressed);          
                       } 
                       
                       if(move_l_opt_btn.detect(menu_x,menu_y,click)==1){
                           if(key_pressed) conf.set_key_move_left(num_key_pressed);          
                       }       
                       
                       if(move_r_opt_btn.detect(menu_x,menu_y,click)==1){
                           if(key_pressed) conf.set_key_move_right(num_key_pressed);          
                       }                                       
                       
                       if(prone_opt_btn.detect(menu_x,menu_y,click)==1){
                           if(key_pressed) conf.set_key_prone(num_key_pressed);          
                       }
                       
                       if(crouch_opt_btn.detect(menu_x,menu_y,click)==1){
                           if(key_pressed) conf.set_key_crouch(num_key_pressed);          
                       }
                       
                       if(jump_opt_btn.detect(menu_x,menu_y,click)==1){
                           if(key_pressed) conf.set_key_jump(num_key_pressed);          
                       }
                       
                       if(fire_opt_btn.detect(menu_x,menu_y,click)==1){
                           if(key_pressed) conf.set_key_jump(num_key_pressed);          
                       }                                                                                        
                             
                   WriteText(0.23,0.67,0.3,"-=OPCII=-",1,1,1,1,NULL,font_c);
                   WriteText(0.05,0.62,0.2,"GRAFIKA:",1,1,1,1,NULL,font_c);
                   WriteText(0.1,0.55,0.2,"REZOL\\CIA:",1,1,1,1,NULL,font_c);
                   WriteText(0.1,0.49,0.2,"CQL EKRAN:",1,1,1,1,NULL,font_c);
                   WriteText(0.05,0.42,0.2,"KONTROLI:",1,1,1,1,NULL,font_c);
                   WriteText(0.1,0.36,0.2,"NAPRED:",1,1,1,1,NULL,font_c);
                   WriteText(0.1,0.31,0.2,"NAZAD:",1,1,1,1,NULL,font_c);
                   WriteText(0.1,0.26,0.2,"NALQWO:",1,1,1,1,NULL,font_c);
                   WriteText(0.1,0.21,0.2,"NADQSNO:",1,1,1,1,NULL,font_c);
                   WriteText(0.1,0.16,0.2,"PYLZENE:",1,1,1,1,NULL,font_c);
                   WriteText(0.1,0.11,0.2,"KLQKANE:",1,1,1,1,NULL,font_c);
                   WriteText(0.1,0.06,0.2,"SKA`ANE:",1,1,1,1,NULL,font_c);
                   WriteText(0.1,0.01,0.2,"OGYN:",1,1,1,1,NULL,font_c);
                   
                   WriteText(-1.07,-0.8,0.2,"ZABELEVKA: SLED NQKOI PROMENI E NUVEN RESTART NA IGRATA",1,0,0,1,NULL,font_c);
                   
                   conv_str=itoa(conf.get_setting_screen_lenght(),conv_c_str,10);
                   conv_str+=" X ";
                   conv_str+=itoa(conf.get_setting_screen_height(),conv_c_str,10);
                   WriteText(0.45,0.55,0.2,(char*)conv_str.c_str(),1,1,1,1,NULL,font);
                   if(conf.get_setting_fullscreen()) WriteText(0.45,0.49,0.2,"DA",1,1,1,1,NULL,font_c);
                   else WriteText(0.45,0.49,0.2,"NE",1,1,1,1,NULL,font_c);
                   WriteText(0.45,0.36,0.2,SpecKeys(conf.get_key_move_forward()),1,1,1,1,NULL,font);
                   WriteText(0.45,0.31,0.2,SpecKeys(conf.get_key_move_back()),1,1,1,1,NULL,font);
                   WriteText(0.45,0.26,0.2,SpecKeys(conf.get_key_move_left()),1,1,1,1,NULL,font);
                   WriteText(0.45,0.21,0.2,SpecKeys(conf.get_key_move_right()),1,1,1,1,NULL,font);
                   WriteText(0.45,0.16,0.2,SpecKeys(conf.get_key_prone()),1,1,1,1,NULL,font);
                   WriteText(0.45,0.11,0.2,SpecKeys(conf.get_key_crouch()),1,1,1,1,NULL,font);
                   WriteText(0.45,0.06,0.2,SpecKeys(conf.get_key_jump()),1,1,1,1,NULL,font);
                   WriteText(0.45,0.01,0.2,SpecKeys(conf.get_key_fire()),1,1,1,1,NULL,font);
                   
                   //WriteText(0.36,-0.1,0.2,"ZAPAZI",1,1,1,1,NULL,font_c);
                   WriteText(0.16,-0.1,0.2,"WYZSTANOWI NA`ALNI",1,1,1,1,NULL,font_c);
                   WriteText(0.36,-0.15,0.2,"ZATWORI",1,1,1,1,NULL,font_c);
                   WriteText(0.83,0.55,0.2,"ZASE`I",1,1,1,1,NULL,font_c);
                   
                   //WriteText(test_x,test_y,0.2,"ZASE`I",1,1,1,1,NULL,font_c);
                }
                RenderCursor(menu_x+0.062,menu_y-0.08,cursor);
                
           }
           
           if(adv_stop){
           adv_stop = SAD_Engine_Advert(sad,(int)theta,advert);
           }
           
           if((effect)&&(ingame)){ 
            glBlendFunc(GL_SRC_ALPHA,GL_ONE);
            glBegin(GL_QUADS);
            glColor4f(0.0f, 0.05f, 1.0f, 0.5); 
            glTexCoord2f(0.0, 1.0);
            glVertex3f(-1.8f, -1.0f, -0.1);
            glTexCoord2f(1.0, 1.0);
            glVertex3f(1.8f, -1.0f, -0.1);
            glTexCoord2f(1.0, 0.0);
            glVertex3f(1.8f, 1.0f,  -0.1);
            glTexCoord2f(0.0, 0.0);
            glVertex3f(-1.8f, 1.0f, -0.1);
            glEnd();
           }
           else glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
          glPopMatrix();

          if((!advert)&&(ingame)){
             RenderCrosshair(crosshair,main_player.get_accuracy());
          }
         
            SwapBuffers (hDC);

            theta += 20;
            Sleep(10);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL (hWnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow (hWnd);

    return msg.wParam;
}


/********************
 * Window Procedure
 *
 ********************/

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
                          WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        return 0;
    case WM_CLOSE:
        PostQuitMessage (0);
        return 0;

    case WM_DESTROY:
        return 0;

    case WM_LBUTTONDOWN:
         click=true;
         mouse_key_pressed=true;
         num_key_pressed=VK_LBUTTON;
         GetCursorPos( &pt );
         if((FireTimer.get()>500)&&(ingame)){
          mciSendString("play Sounds/barshot.wav", NULL, 0, NULL);
          fire=true;
          FireTimer.reset();
          }
        return 0;
        
    case WM_LBUTTONUP:
         click=false;
         fire=false;
         mouse_key_pressed=false;        
        return 0;

    case WM_KEYDOWN:
        if(wParam!=VK_ESCAPE)key_pressed=true; 
        num_key_pressed=wParam; 
        switch (wParam)
        {
        case 'W': move_forward=true;
                         break;
        case 'A': move_left=true;
                         break;
        case 'S': move_back=true;
                         break;
        case 'D': move_right=true;
                         break;
        case ' ': jump=true; //NEW
                        break;
        case 'C': crouch=true; //NEW
                       break;
        case '1': effect=true;
                       break;
        case '2': effect=false;
                  eff2=false;
                       break;
        case '3': eff2=true;
                       break;
        case VK_CONTROL: prone=true; //NEW
                      break;
        case VK_RETURN: if(advert) advert=false;
                        key_return=true;
                       break;
/*        case VK_UP: if(!prone)test_y+=0.01;
                    else test_height+=0.01;
                    break;
        case VK_DOWN: if(!prone)test_y-=0.01;
                      else test_height-=0.01;
                    break;
        case VK_LEFT: if(!prone) test_x-=0.01;
                      else test_width-=0.01;
                    break;
        case VK_RIGHT: if(!prone) test_x+=0.01;
                       else test_width+=0.01;
                    break;*/
                      
        case VK_ESCAPE:
            ingame=false;
            inmenu=true;
            //PostQuitMessage(0);
            return 0;
        }

        return 0;
        
        case WM_KEYUP:
        key_pressed=false;
        switch (wParam)
        {
        case 'W': move_forward=false;
                         break;
        case 'A': move_left=false;
                         break;
        case 'S': move_back=false;
                         break;
        case 'D': move_right=false;
                         break;
        case 'C': crouch=false; //NEW
                       break;
        case VK_CONTROL: prone=false; //NEW
                      break;
        case VK_RETURN: key_return=false;
                        break;
        }
        return 0;

    default:
        return DefWindowProc (hWnd, message, wParam, lParam);
    }
}

/*******************
 * Enable OpenGL
 *
 *******************/

void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC)
{
     
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC (hWnd);

    /* set the pixel format for the DC */
    ZeroMemory (&pfd, sizeof (pfd));
    pfd.nSize = sizeof (pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | 
      PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat (*hDC, &pfd);
    SetPixelFormat (*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext( *hDC );
    wglMakeCurrent( *hDC, *hRC );
}

/******************
 * Disable OpenGL
 *
 ******************/

void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent (NULL, NULL);
    wglDeleteContext (hRC);
    ReleaseDC (hWnd, hDC);
}

/******************
 * Timing Thread
 *
 ******************/

void Timing_Thread(void* PARAMS){
     Timer.reset();
     PhysTimer.reset();
     FireTimer.reset();
     
/*     unsigned int num_itt=0;
     unsigned int max_itt=1000;
     int old_time=0;
     int step;
     
     while(1){
              
      if((modul((int)(num_itt-1000))<10000)&&(num_itt-1000>1000)) step=1000;
      else if((modul((int)(num_itt-1000))<1000)&&(num_itt-1000>100)) step=100;
      else if((modul((int)(num_itt-1000))<100)&&(num_itt-1000>10)) step=10;
      else if(modul((int)(num_itt-1000))<10) step=1;
              
      if(phystime.wSecond!=old_time){
          if(num_itt>1000) max_itt+=step; 
          else if((num_itt<999) && (max_itt>step)) max_itt-=step/10;
          old_time=phystime.wSecond;
          num_itt=0;
      }
      
      for(int i=0;i<max_itt; i++){
              Sleep(0);
      }                 
              
        Timer.inc();
        PhysTimer.inc();
        FireTimer.inc();
        Sleep(0);
        
        num_itt++; 
        Sleep(1);
     }*/
}

/******************
 * Physics Thread
 *
 ******************/

void Phys_Thread(void* PARAMS){
     int result;
     unsigned int num_itt=0;
     unsigned int max_itt=10;
     int old_time=0;
     int step;
     
     while(1){
      GetSystemTime(&phystime);
      
      Timer.inc();
      PhysTimer.inc();
      FireTimer.inc();
      
      //cout<<phystime.wSecond<<' '<<old_time<<endl;
      if((modul((int)(num_itt-1000))<10000)&&(num_itt-1000>1000)) step=1000;
      else if((modul((int)(num_itt-1000))<1000)&&(num_itt-1000>100)) step=100;
      else if((modul((int)(num_itt-1000))<100)&&(num_itt-1000>10)) step=10;
      else if(modul((int)(num_itt-1000))<10) step=1;
      
      //step=(num_itt-1000)/10;
      
      if(phystime.wSecond!=old_time){
          cout<<num_itt<<' '<<max_itt<<' '<<step<<' '<<modul((int)(num_itt-1000))<<endl;
          if(num_itt>1000) max_itt+=step; 
          else if((num_itt<999) && (max_itt>=step/10)) max_itt-=step/10;
          old_time=phystime.wSecond;
          num_itt=0;
      }
      
      for(int i=0;i<max_itt; i++){
              Sleep(0);
      }
      if(ingame){                             
             if(crouch)
             {
                 main_player.do_crouch(0,4,0.8,1.8);
                 if(move_forward||move_back||move_left||move_right){
                     main_player.set_accuracy(1,2);//dispersion in degrees
                     if(move_forward)
                     {
                        main_player.move_forward(1.1);          
                     }
                     if(move_back)
                     {
                        main_player.move_back(0.9);
                     }
                     if(move_left)
                     { 
                        main_player.move_left(0.7);
                     }
                     if(move_right)
                     { 
                        main_player.move_right(0.7);
                     }
                 }
                 else main_player.set_accuracy(0,0.5);
             }
             else if(prone){
                  main_player.do_crouch(0,3,0.3,1.8);
                 if(move_forward||move_back||move_left||move_right){
                     main_player.set_accuracy(1,1);//dispersion in degrees
                     if(move_forward)
                     {
                        main_player.move_forward(1);          
                     }
                     if(move_back)
                     {
                        main_player.move_back(0.8);
                     }
                     if(move_left)
                     { 
                        main_player.move_left(0.6);
                     }
                     if(move_right)
                     { 
                        main_player.move_right(0.6);
                     }
                 }
                 else main_player.set_accuracy(0,0.3);//dispersion in degrees
             }
             else{ 
                 main_player.do_crouch(1,4,0.8,1.8);
                 main_player.set_accuracy(1,1);//dispersion in degrees
                 if(move_forward||move_back||move_left||move_right){
                     main_player.set_accuracy(1,4);
                     if(move_forward)
                     {
                        main_player.move_forward(4.1);          
                     }
                     if(move_back)
                     {
                        main_player.move_back(3.3);
                     }
                     if(move_left)
                     { 
                        main_player.move_left(2.4);
                     }
                     if(move_right)
                     { 
                        main_player.move_right(2.4);
                     }
                 }
                 else if(jump) main_player.set_accuracy(1,5); 
                 else main_player.set_accuracy(0,1);//dispersion in degrees
             }
             if (main_player.get_x()+0.3>=test_map.get_width()/2){ 
                main_player.set_x(test_map.get_width()/2-0.3);
                }
                
             if (main_player.get_x()-0.3<=-test_map.get_width()/2){ 
                main_player.set_x(-test_map.get_width()/2+0.3);
                }
                
             if (main_player.get_y()+0.3>=test_map.get_lenght()/2){ 
                main_player.set_y(test_map.get_lenght()/2-0.3);
                }
                 
             if (main_player.get_y()-0.3<=-test_map.get_lenght()/2){ 
                main_player.set_y(-test_map.get_lenght()/2+0.3);
                }
                
                //push the boxes
            for(int i=0;i<10;i++){
             if (((main_player.get_x()+0.3>movable[i].get_x()) &&
                 (main_player.get_x()-0.3<movable[i].get_x()+movable[i].get_width()) && 
                 (main_player.get_y()+0.3>movable[i].get_y()) && 
                 (main_player.get_y()-0.3<movable[i].get_y()+movable[i].get_lenght())))
             {
                if(main_player.get_x()<movable[i].get_x()+0.35){
                   if (movable[i].get_x()+movable[i].get_width()<test_map.get_width()/2) movable[i].set_x(movable[i].get_x()+0.0041);
                   else main_player.set_x(movable[i].get_x()-0.3);
                }
                
                if(main_player.get_x()>movable[i].get_x()+movable[i].get_width()-0.35){
                   if (movable[i].get_x()>-test_map.get_width()/2) movable[i].set_x(movable[i].get_x()-0.0041);
                   else main_player.set_x(movable[i].get_x()+movable[i].get_width()+0.3);
                }
                 
                if(main_player.get_y()<movable[i].get_y()+0.35) {
                   if (movable[i].get_y()+movable[i].get_lenght()<test_map.get_lenght()/2) movable[i].set_y(movable[i].get_y()+0.0041);
                   else main_player.set_y(movable[i].get_y()-0.3);               
                }
                
                if(main_player.get_y()>movable[i].get_y()+movable[i].get_lenght()-0.35){
                   if (movable[i].get_y()>-test_map.get_lenght()/2) movable[i].set_y(movable[i].get_y()-0.0041);
                   else main_player.set_y(movable[i].get_y()+movable[i].get_lenght()+0.3);
                }
                
                for(int j=0;j<col_i_f;j+=3){
                     result = PolygonDetect(temp_x,temp_y,temp_z,movable[i].get_x(),movable[i].get_y(),movable[i].get_z(),
                                          col_v[(col_f[i]-1)*3], col_v[(col_f[i]-1)*3+2], col_v[(col_f[i]-1)*3+1],
                                          col_v[(col_f[i+1]-1)*3], col_v[(col_f[i+1]-1)*3+2], col_v[(col_f[i+1]-1)*3+1],
                                          col_v[(col_f[i+2]-1)*3], col_v[(col_f[i+2]-1)*3+2], col_v[(col_f[i+2]-1)*3+1]);
                     if(!result) break;
                }
                
                movable[i].set_z(temp_z);
             }
            }
             
             temp_x=main_player.get_x();
             temp_y=main_player.get_y();
             temp_z=main_player.get_z();         
    
             for(int i=0;i<col_i_f;i+=3){
                 result = PolygonDetect(temp_x,temp_y,temp_z,main_player.get_x(),main_player.get_y(),main_player.get_z(),
                                      col_v[(col_f[i]-1)*3], col_v[(col_f[i]-1)*3+2], col_v[(col_f[i]-1)*3+1],
                                      col_v[(col_f[i+1]-1)*3], col_v[(col_f[i+1]-1)*3+2], col_v[(col_f[i+1]-1)*3+1],
                                      col_v[(col_f[i+2]-1)*3], col_v[(col_f[i+2]-1)*3+2], col_v[(col_f[i+2]-1)*3+1]);
                 if(!result) break;
             }
                                      
    /*             result = PolygonDetect(temp_x,temp_y,temp_z,main_player.get_x(),main_player.get_y(),main_player.get_z(),
                                      col.coords[0][0],col.coords[0][2],col.coords[0][1],
                                      col.coords[1][0],col.coords[1][2],col.coords[1][1],
                                      col.coords[2][0],col.coords[2][2],col.coords[2][1]);*/         
                     
             if((!result) && (!jump)){
                  main_player.set_coords(temp_x,temp_y,temp_z);
                  PhysTimer.reset();
             }
             else{ 
                  if(jump){
                           if(main_player.get_z()>=temp_z){
                                main_player.phys_fall(3,9.81,PhysTimer.get());
                           }
                           else jump=false;
                  }
                  else{
                          if(main_player.get_z()>=temp_z) main_player.phys_fall(0,9.81,PhysTimer.get());
                          else main_player.set_z(temp_z);
                  }
             }
             
             if(Timer.get()>2000){
                 if(fire){
                     proj[count_proj].launch(main_player.get_x(),main_player.get_y(),main_player.get_eyes_lvl()+main_player.get_z()-0.1,main_player.get_angle_z()-(rand()%(int)(main_player.get_accuracy()*1000))/1000,-main_player.get_elevation()+(rand()%(int)(main_player.get_accuracy()*1000))/1000);
                     if(count_proj<10000)count_proj++;
                     else count_proj=0;
                     fire=false;
                 }
                 for(int i=0;i<count_proj;i++){                
                         if(ciganin.detect(proj[i].get_x(),proj[i].get_y(),proj[i].get_z())||ciganin2.detect(proj[i].get_x(),proj[i].get_y(),proj[i].get_z())){ score++; count_proj--;}
                         else proj[i].proj_path();
                 }
             }
        }
        num_itt++;                                   
        Sleep(0);
     }
}

void XY2Polar(RECT ds,int scr_x,int scr_y, float &azimuth,float &elevation,unsigned int res_x, unsigned int res_y, float sens){
           static int fx=0,fy=0;
           int x=scr_x-((ds.right/2)-res_x/2)+fx;
           int y=scr_y-((ds.bottom/2)-res_y/2)+fy;
           
           if(scr_x-((ds.right/2)-(res_x/2))>res_x/2+199){
                fx+=scr_x-ds.right/2;
                SetCursorPos(ds.right/2,scr_y);
           }
           else if(scr_x-((ds.right/2)-(res_x/2))<res_x/2-199){
                fx-=ds.right/2-scr_x;
                SetCursorPos(ds.right/2,scr_y);
           }
              
           if((scr_y-((ds.bottom/2)-(res_y/2))>res_y/2+199)&&(elevation<90)&&(elevation>-90)){
                fy+=scr_y-ds.bottom/2;              
                SetCursorPos(scr_x,ds.bottom/2);
           }
           else if((scr_y-((ds.bottom/2)-(res_y/2))<res_y/2-199)&&(elevation<90)&&(elevation>-90)){
                fy-=ds.bottom/2-scr_y;
                SetCursorPos(scr_x,ds.bottom/2);
           }
           
           azimuth=(x-308)*sens;
           if(((y-218)*sens<90)&&((y-218)*sens>-90))elevation=(y-218)*sens;
           else if((y-218)*sens>90) elevation=90;
           else if((y-218)*sens<-90) elevation=-90;
}

char* SpecKeys(char key){
      string key_str;
      if((key>47)&&(key<94)){
         key_str=key;                    
      }
      else{
         switch(key){
            case VK_LBUTTON: key_str="[LEFT MOUSE BUTTON]";
                             break;
            case VK_RBUTTON: key_str="[RIGHT MOUSE BUTTON]";
                             break; 
            case VK_MBUTTON: key_str="[MIDDLE MOUSE BUTTON]";
                             break;
            case VK_BACK:    key_str="[BACKSPACE]";
                             break;
            case VK_TAB:     key_str="[TAB]";
                             break;
            case VK_RETURN:  key_str="[ENTER]";
                             break;
            case VK_SHIFT:   key_str="[SHIFT]";
                             break;
            case VK_CONTROL: key_str="[CTRL]";
                             break;
            case VK_MENU:     key_str="[ALT]";
                             break;
            case VK_CAPITAL: key_str="[CAPS LOCK]";
                             break;
            case VK_ESCAPE:  key_str="[ESCAPE]";
                             break;
            case VK_SPACE:   key_str="[SPACE]";
                             break;
            case VK_PRIOR:   key_str="[PAGE UP]";
                             break;
            case VK_NEXT:    key_str="[PAGE DOWN]";
                             break;
            case VK_END:     key_str="[END]";
                             break;
            case VK_HOME:    key_str="[HOME]";
                             break;
            case VK_LEFT:    key_str="[HOME]";
                             break;
            case VK_UP:      key_str="[UP]";
                             break; 
            case VK_RIGHT:   key_str="[RIGHT]";
                             break;
            case VK_DOWN:    key_str="[DOWN]";
                             break;                                                                                                                  
         }  
      }
      return (char*)key_str.c_str();
}

void ChangeResolution(config &conf,RECT ds,char res_type){
     int res_x,res_y;
     switch(res_type%7){
        case 1: res_x=640;
                res_y=480;
                break;
        case 2: res_x=800;
                res_y=600;
                break;
        case 3: res_x=1024;
                res_y=600;
                break;
        case 4: res_x=1024;
                res_y=768;
                break;
        case 5: res_x=1280;
                res_y=768;
                break;
        case 6: res_x=1360;
                res_y=768;
                break;
     }
     if(!conf.get_setting_fullscreen()){
         if((res_x<=ds.right)&&(res_y<=ds.bottom)){
            conf.set_setting_screen_lenght(res_x);
            conf.set_setting_screen_height(res_y);                                 
         }
     }
     else{
            conf.set_setting_screen_lenght(res_x);
            conf.set_setting_screen_height(res_y);          
     }
}
