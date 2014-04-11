/**************************
 * Includes
 *
 **************************/

#include <windows.h>
#include <process.h>
#include "engine.h" //Game engine
#include "GL_Rendering.h"


/**************************
 * Function Declarations
 *
 **************************/
void XY2Polar(RECT ds,int scr_x,int scr_y, float &azimuth,float &elevation,int res_x, int res_y, float sens);
void Phys_Thread(void* PARAMS); 
void Timing_Thread(void* PARAMS);
void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC);
void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC);
LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
WPARAM wParam, LPARAM lParam);
float Check(bool SorG, float a, float b, float c);
int Check_ID(bool SorG, float a, float b, float c);
int Check_sign(float a, float b);
float modul(float a);
/**************************
 * Global Objects Declarations
 *
 **************************/

colison col;
player main_player;
map test_map(50,50);
movable_objects movable[10] = {movable_objects(-10, -5, 0, 1, 1, 1), 
                               movable_objects(-10, 20, 0, 1, 1, 2),
                               movable_objects(0, 7, 0, 2, 2, 2),
                               movable_objects(1, 4, 0, 2, 1, 1),
                               movable_objects(12, 18, 0, 0.5, 0.5, 0.5),
                               movable_objects(10, -15, 0, 1, 1, 1),
                               movable_objects(4, -23, 0, 2, 2, 2),
                               movable_objects(-8, -14, 0, 2, 1, 1),
                               movable_objects(14, -18, 0, 0.5, 0.5, 0.5),
                               movable_objects(21, 0, 0, 1, 1, 1),};
timer Timer;
timer PhysTimer;

bool move_forward=0, move_back=0, move_right=0, move_left=0, crounch=0;

bool boom;

float temp;

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
    
    bool advert=true;
    
    camera cam;
    
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

    /* create main window */
    RECT ds;
    GetWindowRect(GetDesktopWindow(), &ds);
    hWnd = CreateWindow (
      "GLSample", "OpenGL Sample", 
      WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
      ((ds.right/2)-320), ((ds.bottom/2)-240), 640, 480,
      NULL, NULL, hInstance, NULL);
    LONG lStyle = GetWindowLong(hWnd, GWL_STYLE);
    lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
    SetWindowLong(hWnd, GWL_STYLE, lStyle);
    SetWindowPos(hWnd, NULL, 0,0,0,0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);    

    /* enable OpenGL for the window */
    EnableOpenGL (hWnd, &hDC, &hRC);
    
    PlaySound("Sounds/Powered.wav", NULL, SND_ASYNC);
    
    //GLuint *frame = (GLuint *)malloc(sizeof(GLuint)*10);
    
    /*initialize OpenGL */
    GL_init();

    /* Load some textures */
    GLuint sad = loadTexture(true, "Textures/powered_tex.dds" );
    GLuint crate = loadTexture(true, "Textures/crate001.dds" );
    GLuint fence = loadTexture(true, "Textures/fence.dds" );
    GLuint ground = loadTexture(true, "Textures/ground.dds" );
    GLuint skybox[5] = {
           loadTexture(true, "Textures/skybox_top.dds" ),
           loadTexture(true, "Textures/skybox_left.dds" ),
           loadTexture(true, "Textures/skybox_front.dds" ),
           loadTexture(true, "Textures/skybox_right.dds" ),
           loadTexture(true, "Textures/skybox_back.dds" )
    };
    
    /*starts timing thread*/
    HANDLE h = (HANDLE)_beginthread((void(*)(void*))Timing_Thread, 0, NULL);
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
         
           GetCursorPos( &pt );
             
         XY2Polar(ds,pt.x,pt.y,azth,elev,640,480,0.1);
         
         glPushMatrix ();
         
         glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
          glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);
         
         main_player.set_angles(azth,elev);
         
         CameraUse(cam,azth,elev,main_player.get_x(),main_player.get_y(),main_player.get_eyes_lvl()+main_player.get_z());
         
         cout<<main_player.get_x()<<' '<<main_player.get_y()<<' ' << main_player.get_z()<<endl;
         
             glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
            	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
            	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight0);
             glLightfv(GL_LIGHT0, GL_POSITION, position0);
             
          RenderSkybox(skybox);
          RenderMap(fence,ground);
          if(boom) glColor3f(1,0,0);
          else glColor3f(1,1,1);     
          RenderMovable(movable,crate);
          RenderColison(col);    
          glPopMatrix();

          glPushMatrix();
           glScalef(0.4,0.4,1);
           if(advert){
           advert = SAD_Engine_Advert(sad,Timer.get());
           }
          glPopMatrix();
         
            SwapBuffers (hDC);

            theta += 0.1f;
            Sleep(1);
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

    case WM_KEYDOWN:
        switch (wParam)
        {
        case 'D': move_right=true;
                         break;
        case 'A': move_left=true;
                         break;
        case 'W': move_forward=true;
                         break;
        case 'S': move_back=true;
                         break;
        case VK_CONTROL: crounch=true;
                         break;
        case VK_ESCAPE:
            PostQuitMessage(0);
            return 0;
        }
        return 0;
        
    case WM_KEYUP:
         switch (wParam)
         {
          case 'D':   move_right=false;
                      break;
          case 'A':   move_left=false;
                      break;
	      case 'W':   move_forward=false;
                      break;
	      case 'S':   move_back=false;
                      break;
        case VK_CONTROL: crounch=false;
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
     while(1){
        Timer.inc();
        PhysTimer.inc();
        Sleep(1);
     }
}

/******************
 * Physics Thread
 *
 ******************/

void Phys_Thread(void* PARAMS){
     float A_x,A_y,A_z;
     float B_x,B_y,B_z;
     float C_x,C_y,C_z;
     while(1){
         if(move_forward)
         {
            main_player.move_forward(4.1);          
         }
         if(move_back)
         {
            main_player.move_back(4.1);
         }
         if(move_left)
         { 
            main_player.move_left(4.1);
         }
         if(move_right)
         { 
            main_player.move_right(4.1);
         }
         if(crounch)
         {
            main_player.set_eyes_lvl(0.7);
         }
         else main_player.set_eyes_lvl(1.7);
         
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
         }
         

         cout<<PolygonDetect(temp,main_player.get_x(),main_player.get_y(),main_player.get_z(),
                              col.coords[0][0],col.coords[0][2],col.coords[0][1],
                              col.coords[1][0],col.coords[1][2],col.coords[1][1],
                              col.coords[2][0],col.coords[2][2],col.coords[2][1])<<endl;
                              
         if(0==PolygonDetect(temp,main_player.get_x(),main_player.get_y(),main_player.get_z(),
                              col.coords[0][0],col.coords[0][2],col.coords[0][1],
                              col.coords[1][0],col.coords[1][2],col.coords[1][1],
                              col.coords[2][0],col.coords[2][2],col.coords[2][1])){
                                                                                   
              main_player.set_z(temp);
              PhysTimer.reset();
         }
         else{
              if(main_player.get_z()>0) main_player.phys_fall(0,9.81,PhysTimer.get());
              else main_player.set_z(0);
         }
         
        }                                   
        Sleep(1);
     }
}

void XY2Polar(RECT ds,int scr_x,int scr_y, float &azimuth,float &elevation,int res_x, int res_y, float sens){
           static int fx=0,fy=0;
           int x=scr_x-((ds.right/2)-320)+320*fx;
           int y=scr_y-((ds.bottom/2)-240)+240*fy;

           if(scr_x-((ds.right/2)-320)>640){ 
                SetCursorPos(ds.right/2,scr_y);
                fx++;
           }
           else if(scr_x-((ds.right/2)-320)<1){ 
                SetCursorPos(ds.right/2,scr_y);
                fx--;
           }
           
           azimuth=(x-308)*sens;
           elevation=(y-218)*sens;
           
           if(elevation>=90){
              fy=3;
              SetCursorPos(scr_x,ds.bottom/2+160);
           }
           else if (elevation<=-90){
                fy=-3;
                SetCursorPos(scr_x,ds.bottom/2-200);
           }
           
           if(scr_y-((ds.bottom/2)-240)>480){ 
                SetCursorPos(scr_x,ds.bottom/2);
                fy++;
           }
           else if(scr_y-((ds.bottom/2)-240)<1){ 
                SetCursorPos(scr_x,ds.bottom/2);
                fy--;
           }    
}


