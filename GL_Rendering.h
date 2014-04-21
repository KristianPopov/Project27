#include <gl/gl.h>
#include "GLINIT.h"
#include "dds.h"
#include <gl/glu.h>

#define GL_TEXTURE_MAX_ANISOTROPY_EXT     0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

bool SAD_Engine_Advert(GLuint powered_tex,int timer){
  if(timer<8000){
    glBegin(GL_QUADS);
    glColor4f(0.0f, 0.0f, 0.0f, (3000-timer)*0.0005); 
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-1.3f, -1.0f, -1);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(1.3f, -1.0f, -1);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(1.3f, 1.0f,  -1);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-1.3f, 1.0f, -1);
    glEnd();
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, powered_tex);
    glBegin(GL_QUADS);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0); 
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-1.3f, -1.0f, -1);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(1.3f, -1.0f, -1);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(1.3f, 1.0f,  -1);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-1.3f, 1.0f, -1);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    return true;
  }
  else{
    PlaySound("Sounds/night_hunter.wav", NULL, SND_ASYNC|SND_LOOP);
    return false;
  }
}

void GL_init(){
     GL2Init();
     glViewport(0, 0, 640, 480);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     gluPerspective(40.0f,(GLfloat)640/(GLfloat)480,0.0001f,10000000.0f);
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
     glEnable(GL_DEPTH_TEST);
     glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);
     glEnable(GL_COLOR_MATERIAL);
     glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
     glShadeModel(GL_SMOOTH);
     glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
     glEnable(GL_POLYGON_SMOOTH);
     glEnable(GL_BLEND);
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
     glDepthFunc(GL_LEQUAL);
     glClearDepth(1.0f);
     glEnableClientState (GL_VERTEX_ARRAY);
     glEnableClientState (GL_NORMAL_ARRAY);
     glEnableClientState (GL_COLOR_ARRAY);
     glEnableClientState (GL_TEXTURE_COORD_ARRAY);
}

void RenderCrosshair(GLuint crosshair_tex,float accuracy){
    glPushMatrix();
    glScalef(accuracy,accuracy,1);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, crosshair_tex);
    glBegin(GL_QUADS);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0); 
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.001, -0.001, -0.1);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.001f, -0.001f, -0.1);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.001f, 0.001f,  -0.1);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.001f, 0.001f, -0.1);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glPopMatrix();    
}
     
void RenderColison(colison col){     
     glPushMatrix();
     glLineWidth(30);
     glBegin(GL_TRIANGLES);
     glVertex3f(col.coords[0][0],col.coords[0][1],col.coords[0][2]);
     glVertex3f(col.coords[1][0],col.coords[1][1],col.coords[1][2]);
     glVertex3f(col.coords[2][0],col.coords[2][1],col.coords[2][2]);
     glEnd();
     glPopMatrix();
     
}

void RenderProjectile(projectile* proj,camera cam,GLuint proj_tex,int count_proj){
    for(int i=0;i<count_proj;i++){
        glPushMatrix();
        glTranslatef(proj[i].get_x(), proj[i].get_z(), -proj[i].get_y());
        glRotatef(-cam.get_angle_z(),0,1,0);
        glRotatef(cam.get_angle_x(),1,0,0);
        glRotatef(-cam.get_angle_y(),0,0,1);
        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, proj_tex);
        glBegin(GL_QUADS);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0); 
        glTexCoord2f(0.0, 1.0);
        glVertex3f(-0.1, -0.1, 0);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(0.1f, -0.1f, 0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(0.1f, 0.1f,  0);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(-0.1f, 0.1f, 0);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
        glPopMatrix();
    }              
}

void RenderModel(int num_p, float* arr_v,float* arr_vt, float* arr_vn,int* arr_f){
     glPushMatrix();
     glLineWidth(1);
     glBegin(GL_TRIANGLES);
     int i=0;
     while(i<num_p){
         glNormal3d(arr_vn[(arr_f[i+2]-1)*3], arr_vn[(arr_f[i+2]-1)*3+1], arr_vn[(arr_f[i+2]-1)*3+2]);
         glVertex3f(arr_v[(arr_f[i]-1)*3], arr_v[(arr_f[i]-1)*3+1], arr_v[(arr_f[i]-1)*3+2]);
         i+=3;
     }
     glEnd();
     
 /*    glDisableClientState (GL_COLOR_ARRAY);
     
     glPointSize(10);
     
     glVertexPointer (3, GL_FLOAT, 0, arr_v);    
     glNormalPointer(GL_FLOAT, 0, arr_vn);
     glTexCoordPointer(2, GL_FLOAT, 0, arr_vt);
     glColor4f(0,0.5,1,1);
     //glDrawArrays(GL_POINTS,1,num_p/3);
     glColor4f(1,1,1,1);
     //glDrawArrays(GL_LINE_STRIP,1,num_v/3);
     //glDrawRangeElements(GL_LINE_LOOP, 0, 3, 4, GL_UNSIGNED_BYTE, arr_f);
     //glDrawElements(GL_TRIANGLE_FAN, num_v/3, GL_UNSIGNED_BYTE, arr_f);*/
     glPopMatrix();          
}
     
void RenderMovable(movable_objects movable[10],GLuint texture){
     
     glEnable(GL_MULTISAMPLE);
     glEnable(GL_BLEND);
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
     glEnable(GL_LINE_SMOOTH);
     glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
     glEnable(GL_POINT_SMOOTH);
     glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
     glEnable(GL_POLYGON_SMOOTH);
     glHint(GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);

     
       static GLfloat v[8][3];
       static GLfloat c[8][4] = {
      {1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0},
      {1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0},
      {1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0},
      {1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}
      };
      
       static GLfloat tex_coords[8][2] = {
      {0.0, 1.0},
      {1.0, 1.0},
      {1.0, 0.0},
      {0.0, 0.0},
      {1.0, 1.0},
      {0.0, 1.0},
      {0.0, 0.0},
      {1.0, 0.0}
      };
      
      GLfloat normals1[8][3]  = { 
                              {-1.0, 0.0, 0.0},   
                              {1, 0, 0},   
                              {1, 0, 0},   
                              {-1, 0, 0}, 
                              {-1, 0, 0},   
                              {1, 0, 0},
                              {1, 0, 0},   
                              {-1, 0, 0}
      };

      /*  indices of front, top, left, bottom, right, back faces  */
      static GLubyte indices[6][4] = {
      {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3},
      {0, 1, 5, 4}, {1, 5, 6, 2}, {0, 3, 2, 1}
      };

      for(int i=0; i<10; i++){
      v[0][0] = v[3][0] = v[4][0] = v[7][0] = 0;
      v[1][0] = v[2][0] = v[5][0] = v[6][0] = movable[i].get_width();
      v[0][1] = v[1][1] = v[4][1] = v[5][1] = 0;
      v[2][1] = v[3][1] = v[6][1] = v[7][1] = movable[i].get_height();
      v[0][2] = v[1][2] = v[2][2] = v[3][2] = 0;
      v[4][2] = v[5][2] = v[6][2] = v[7][2] = -movable[i].get_lenght();
      
     glPushMatrix();
     
     //glEnable(GL_LIGHTING);
     
     glEnable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D, texture);

     glTranslatef(movable[i].get_x(),-movable[i].get_z(),-movable[i].get_y());
     
     glVertexPointer (3, GL_FLOAT, 0, v);    
     glColorPointer (4, GL_FLOAT, 0, c);
     glNormalPointer(GL_FLOAT, 0, normals1);
     glTexCoordPointer(2, GL_FLOAT, 0, tex_coords);
     glDrawElements(GL_QUADS, 8*4, GL_UNSIGNED_BYTE, indices);
//     glDrawElements(GL_LINES, 8*4, GL_UNSIGNED_BYTE, indices);

     
    glBegin(GL_QUADS);              
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.0f, movable[i].get_height(), -movable[i].get_lenght());
    glTexCoord2f(0.0, 1.0);
    glVertex3f(movable[i].get_width(),  movable[i].get_height(), -movable[i].get_lenght());
    glTexCoord2f(0.0, 0.0);
    glVertex3f(movable[i].get_width(), movable[i].get_height(),  0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.0f, movable[i].get_height(), 0);
    glEnd();
       
     glDisable(GL_TEXTURE_2D);
     glPopMatrix();
     }
}     

/*     glEnable(GL_FOG);
   {
     GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0};

      glFogi (GL_FOG_MODE, GL_EXP);
      glFogfv (GL_FOG_COLOR, fogColor);
      glFogf (GL_FOG_DENSITY, 0.35);
      glHint (GL_FOG_HINT, GL_DONT_CARE);
      glFogf (GL_FOG_START, 1.0);
      glFogf (GL_FOG_END, 5.0);
   }
   glClearColor(0.0, 0.0, 0.0, 1.0);*/

void CameraUse(camera cam){
     glRotatef (cam.get_angle_z(), 0.0f, 1.0f, 0.0f);
     glRotatef (cam.get_angle_y(), 0.0f, 0.0f, 1.0f);
     glRotatef (cam.get_angle_x(), 1.0f, 0.0f, 0.0f);
     glTranslatef(-cam.get_x(),-cam.get_z(),cam.get_y());
}

void CameraUse(camera cam, float azimuth,float elevation,float new_x,float new_y, float new_z){
     cam.set_angles(azimuth,elevation);
     cam.set_coords(new_x,new_y,new_z);
     glRotatef (cam.get_angle_z(), 0.0f, 1.0f, 0.0f);
     glRotatef (cam.get_angle_y(), 0.0f, 0.0f, 1.0f);
     glRotatef (cam.get_angle_x(), 1.0f, 0.0f, 0.0f);
     glTranslatef(-cam.get_x(),-cam.get_z(),cam.get_y());
      
}

void RenderMap(GLuint fence, GLuint ground){
     
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ground);
     
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
                  
    glNormal3d(0, 1, 0);
    glTexCoord2f(0.0, 5.0);
    glVertex3f(-25.0, 0.0, 25.0);
    glTexCoord2f(5.0, 5.0);
    glVertex3f(25.0, 0.0, 25.0);
    glTexCoord2f(5.0, 0.0);
    glVertex3f(25.0, 0.0,  -25.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-25.0, 0.0, -25.0);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, fence);
    
    glEnable (GL_MULTISAMPLE); 
    glEnable (GL_POLYGON_SMOOTH);
    glEnable (GL_LINE_SMOOTH);
    glEnable (GL_BLEND); 
    
    glBegin(GL_QUADS);
    glNormal3d(0, 0, 1);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-25.0f, 0.0f, -25.0f);
    glTexCoord2f(4.0, 1.0);
    glVertex3f(25.0f,  0.0f, -25.0f);
    glTexCoord2f(4.0, 0.0);
    glVertex3f(25.0f, 5.0f,  -25.0f);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-25.0f, 5.0f, -25.0f);
                  
    glNormal3d(0, 0, -1);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-25.0f, 0.0f, 25.0f);
    glTexCoord2f(4.0, 1.0);
    glVertex3f(25.0f,  0.0f, 25.0f);
    glTexCoord2f(4.0, 0.0);
    glVertex3f(25.0f, 5.0f,  25.0f);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-25.0f, 5.0f, 25.0f);
                  
    glNormal3d(-1, 0, 0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(25.0f, 0.0f, -25.0f);
    glTexCoord2f(4.0, 1.0);
    glVertex3f(25.0f, 0.0f,  25.0f);
    glTexCoord2f(4.0, 0.0);
    glVertex3f(25.0f, 5.0f, 25.0f);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(25.0f, 5.0f,-25.0f);
                  
    glNormal3d(1, 0, 0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-25.0f, 0.0f, -25.0f);
    glTexCoord2f(4.0, 1.0);
    glVertex3f(-25.0f, 0.0f,  25.0f);
    glTexCoord2f(4.0, 0.0);
    glVertex3f(-25.0f, 5.0f, 25.0f);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-25.0f, 5.0f,-25.0f);     
    glEnd();    
}

void RenderSkybox(GLuint skybox[5]){
     
    glPushMatrix();
    glTranslatef(0,-500,0);
    glDisable(GL_LIGHTING);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, skybox[0]);
    
    glColor3f(1.0f, 1.0f, 1.0f);
         
    glBegin(GL_QUADS);
                  
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-1000.0, 995.0, 1000.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(1000.0, 995.0, 1000.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(1000.0, 995.0,  -1000.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-1000.0, 995.0, -1000.0);
    glEnd();
    

    glBindTexture(GL_TEXTURE_2D, skybox[4]);
    glBegin(GL_QUADS);              
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-1000.0f, 0.0f, -1000.0f);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(1000.0f,  0.0f, -1000.0f);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(1000.0f, 1000.0f,  -1000.0f);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-1000.0f, 1000.0f, -1000.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, skybox[2]);
    glBegin(GL_QUADS);             
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-1000.0f, 0.0f, 1000.0f);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(1000.0f,  0.0f, 1000.0f);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(1000.0f, 1000.0f,  1000.0f);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-1000.0f, 1000.0f, 1000.0f);
    glEnd();
  
    glBindTexture(GL_TEXTURE_2D, skybox[3]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(1000.0f, 0.0f, -1000.0f);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(1000.0f, 0.0f,  1000.0f);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(1000.0f, 1000.0f, 1000.0f);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(1000.0f, 1000.0f,-1000.0f);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, skybox[1]);
    glBegin(GL_QUADS);               
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-1000.0f, 0.0f, -1000.0f);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-1000.0f, 0.0f,  1000.0f);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-1000.0f, 1000.0f, 1000.0f);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-1000.0f, 1000.0f,-1000.0f);     
    glEnd(); 
    
    glEnable(GL_LIGHTING);
    glPopMatrix();
}     
