#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

#include "math.h"
#include "newton.h"
#include "myview.h"
#include "mycomplex.h"

#include "pngwriter-0.5.4/src/pngwriter.h"

//const unsigned int width = 1024, height = 768;
//const unsigned int width = 400, height = 300;
//const unsigned int width = 100, height = 100;
//const unsigned int width = 1280, height = 720; //1920, height = 1080;
const unsigned int width = 1920, height = 1080;
//const unsigned int repeat_factor = 1;
//const double zoom_factor = 1;
static int screencounter = 1;

static myview* view;
static equation* lasteq;
static equation* eq;
static newton* fractal;
static float* newton_pixels;
static float* current_time;
static std::vector<std::vector<int> > zero_index;
static std::vector<std::vector<int> > last_zero_index;
static std::vector<std::vector<float> > strength;

// store the colors associated with the fractal's zeros
std::vector<std::vector<float> > colors;

using std::cout;
using std::endl;
using std::ofstream;

void imagefill()
{
  int dimi = strength.size();
  int dimj = strength[0].size();
  std::cout<<"time: " << *current_time<<std::endl;


  if (lasteq->zero_size() > 0)
  {
    std::cout<<"sizes are " <<eq->zero_size()<<" and " << lasteq->zero_size()<<std::endl;
    std::vector<std::vector<int> > matrix(eq->zero_size(),std::vector<int>(lasteq->zero_size(),0));
    //check last ones
    //std::map<std::pair<int,int> > curlastmap;
    for (int j = 0; j < dimj; ++j)
    {
      for (int i = 0; i < dimi; ++i)
      { 
        int zind = zero_index[i][j];
        int lastzind = last_zero_index[i][j];
        matrix[zind][lastzind]++;
      }
    }

    for (int i = 0; i < eq->zero_size(); ++i)
    {
      for (int j = 0; j < lasteq->zero_size(); ++j)
      {
        std::cout<<matrix[i][j]<<" ";
      }
      std::cout<<std::endl;
    }
    // now we can reorder them
    
    std::vector<int> reorders(eq->zero_size(),-1);
    for (int i = 0; i < reorders.size(); ++i)
    {
      int maxind = 0;
      int maxval = matrix[i][0];
      for (int j = 1; j < lasteq->zero_size(); ++j)
      {
        int curval = matrix[i][j];
        if (curval > maxval)
        {
          maxind = j;
          maxval = curval;
        }
      }
      reorders[i] = maxind;
    }
    
//    std::cout<<"reorders are ";
    for (int i = 0; i < reorders.size(); ++i)
      std::cout<<" ri is " << reorders[i] << std::endl;
    
    for (int i = 0; i < dimi; ++i)
    {
      for (int j = 0; j < dimj; ++j)
      {   
        zero_index[i][j] = reorders[zero_index[i][j]];
      }
    }
  }

  //std::cout<<"imagefill running"<<std::endl;

  for (int i = 0; i < dimi; ++i)
  {
    for (int j = 0; j < dimj; ++j)
    {   
      int& zind = zero_index[i][j];
      float zfactor = strength[i][j];
      float target1 = colors[zind%colors.size()][0]*zfactor;
      float target2 = colors[zind%colors.size()][1]*zfactor;
      float target3 = colors[zind%colors.size()][2]*zfactor;
      
      newton_pixels[(i*(dimj)+j)*3  ] = target1;
      newton_pixels[(i*(dimj)+j)*3+1] = target2;
      newton_pixels[(i*(dimj)+j)*3+2] = target3;
    }
  }

}


void writePPM(const char * name, GLubyte* image) 
{
  ofstream out (name);
  out<<"P3 "<<width<<" "<<height<<" "<<255<<endl;
  int i=0;
  for(int y=height-1;y>=0;y--) {
    for(int x=0;x<width; x++) {
      if(i>=15){i=0; out<<endl;} //keep line size around 60
      out<<(unsigned int)image[(y*width+x)*3+0]<<" ";
      out<<(unsigned int)image[(y*width+x)*3+1]<<" ";
      out<<(unsigned int)image[(y*width+x)*3+2]<<" ";
      i+=3;
    }
  }
  out.close();
}

void display()
{

  
/*
  for (int jjj = 0; jjj < repeat_factor; ++jjj)
    for (int iii = 0; iii < repeat_factor; ++iii)
      for (int j = 0; j < height; ++j)  
        for (int i = 0; i < width; ++i)
        {
          bool do_left_swap = (iii%2==0);
          bool do_right_swap = (jjj%2==0);
          if (do_right_swap)
          {             
            if (do_left_swap) 
            {
              newpixels[((i+iii*width)*width*repeat_factor+(j+jjj*height))*3+0] = 
                newton_pixels[((height-1-j)*width+(width-1-i))*3+0];
              newpixels[((i+iii*width)*width*repeat_factor+(j+jjj*height))*3+1] = 
                newton_pixels[((height-1-j)*width+(width-1-i))*3+1];
              newpixels[((i+iii*width)*width*repeat_factor+(j+jjj*height))*3+2] = 
                newton_pixels[((height-1-j)*width+(width-1-i))*3+2];
            } 
            else {
              newpixels[((i+iii*width)*width*repeat_factor+(j+jjj*height))*3+0] = 
                newton_pixels[((height-1-j)*width+(i))*3+0];
              newpixels[((i+iii*width)*width*repeat_factor+(j+jjj*height))*3+1] = 
                newton_pixels[((height-1-j)*width+(i))*3+1];
              newpixels[((i+iii*width)*width*repeat_factor+(j+jjj*height))*3+2] = 
                newton_pixels[((height-1-j)*width+(i))*3+2];
            }
          }
          else 
          {
            if (do_left_swap) 
            {
              newpixels[((i+iii*width)*width*repeat_factor+(j+jjj*height))*3+0] = 
                newton_pixels[(j*width+(width-1-i))*3+0];
              newpixels[((i+iii*width)*width*repeat_factor+(j+jjj*height))*3+1] = 
                newton_pixels[(j*width+(width-1-i))*3+1];
              newpixels[((i+iii*width)*width*repeat_factor+(j+jjj*height))*3+2] = 
                newton_pixels[(j*width+(width-1-i))*3+2];
            } 
            else {
              newpixels[((i+iii*width)*width*repeat_factor+(j+jjj*height))*3+0] = 
                newton_pixels[(j*width+(i))*3+0];
              newpixels[((i+iii*width)*width*repeat_factor+(j+jjj*height))*3+1] = 
                newton_pixels[(j*width+(i))*3+1];
              newpixels[((i+iii*width)*width*repeat_factor+(j+jjj*height))*3+2] = 
                newton_pixels[(j*width+(i))*3+2]; 
            }
          }
        }
  
  int newwidth = width*repeat_factor;
  int newheight= height*repeat_factor;
  glDrawPixels(newwidth,newheight,GL_RGB,GL_FLOAT,newpixels);
  */

  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//glClear(GL_COLOR_BUFFER_BIT);
  


  int dimi = strength.size();
  int dimj = strength[0].size();

  float XM = 1;
  float XM2 = 200;

  for (int i = 0; i < (dimi-1); ++i)
  {
    for (int j = 0; j < (dimj-1); ++j)
    {   
      //float target1 = newton_pixels[(i*(dimj)+j)*3  ];
      //float target2 = newton_pixels[(i*(dimj)+j)*3+1];
      //float target3 = newton_pixels[(i*(dimj)+j)*3+2];
glBegin(GL_TRIANGLES);
      glColor3f(newton_pixels[(i*(dimj)+j)*3+0],newton_pixels[(i*(dimj)+j)*3+1],newton_pixels[(i*(dimj)+j)*3+2]);
      glVertex3f(i*XM,j*XM,strength[i][j]*XM2);

      glColor3f(newton_pixels[((i+1)*(dimj)+j)*3+0],newton_pixels[((i+1)*(dimj)+j)*3+1],newton_pixels[((i+1)*(dimj)+j)*3+2]);
      glVertex3f((i+1)*XM,j*XM,strength[i+1][j]*XM2);

      glColor3f(newton_pixels[(i*(dimj)+j+1)*3+0],newton_pixels[(i*(dimj)+j+1)*3+1],newton_pixels[(i*(dimj)+j+1)*3+2]);
      glVertex3f(i*XM,(j+1)*XM,strength[i][j+1]*XM2);
  glEnd();

glBegin(GL_TRIANGLES);
      glColor3f(newton_pixels[((i+1)*(dimj)+j)*3+0],newton_pixels[((i+1)*(dimj)+j)*3+1],newton_pixels[((i+1)*(dimj)+j)*3+2]);
      glVertex3f((i+1)*XM,j*XM,strength[i+1][j]*XM2);

      glColor3f(newton_pixels[(i*(dimj)+j+1)*3+0],newton_pixels[(i*(dimj)+j+1)*3+1],newton_pixels[(i*(dimj)+j+1)*3+2]);
      glVertex3f(i*XM,(j+1)*XM,strength[i][j+1]*XM2);

      glColor3f(newton_pixels[((i+1)*(dimj)+j+1)*3+0],newton_pixels[((i+1)*(dimj)+j+1)*3+1],newton_pixels[((i+1)*(dimj)+j+1)*3+2]);
      glVertex3f((i+1)*XM,(j+1)*XM,strength[i+1][j+1]*XM2);

  glEnd();

    }
  }



  glColor3f(0.0, 0.0, 1.0); 
  glVertex3f(0,0,0);
  glColor3f(0.0, 1.0, 0.0); 
  glVertex3f(0,100,0);
  glColor3f(1.0, 0.0, 0.0); 
  glVertex3f(100,0,0);


  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDrawPixels(width,height,GL_RGB,GL_FLOAT,newton_pixels);
  glutSwapBuffers();
}

void
reshape(int w, int h)
{

  glViewport(0, 0, w, h);       /* Establish viewing area to cover entire window. */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();             /* Reset project matrix. */
  gluPerspective( /* field of view in degree */ 60.0,
                  /* aspect ratio */ 1.0,
                  /* Z near */ 0.1, 
                  /* Z far */ 10000.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();             /* Reset project matrix. */
  gluLookAt(w/2, h/2, 500,  /* eye is at (0,0,5) */
            w/2, h/2, 0.0, /* center is at (0,0,0) */
            0.0, 1.0, 0.); /* up is in positive Y direction */


}


void TimerFunction(int value)
{
  *current_time = *current_time + .02;

  //if (*current_time == 4)
  //  exit(1);
  
  eq->localize_zeros(*current_time);
  fractal->newtonfill(*eq,zero_index,strength);  
  imagefill(); //(zero_index,strength,colors,newton_pixels);
  
  // remember last zero index matrix as well as equation solution
  last_zero_index = zero_index;
  *lasteq = *eq;
  
  //std::cout<<"2sizes are " <<eq->zero_size()<<" and " << lasteq->zero_size()<<std::endl;
  display();

  if (true)
  {
    GLubyte* image = (GLubyte*)new GLubyte[sizeof(GLubyte)*width*height*3]; 
    glReadPixels(0,0,width,height, GL_RGB, GL_UNSIGNED_BYTE, image);
    
    std::stringstream ss,ss2,ss3;
    ss<<"imageloop/screenshot"<<std::setw(5)<<std::setfill('0')<<screencounter<<".png";
    const char* c = ss.str().c_str(); 

    //write the ppm
    //writePPM(c,image);

    pngwriter png(width,height,0,c);  //Here we create a PNGwriter instance called png, 
    // and whose associated file is c

    for (int i = 0; i < width; ++i) {
      for (int j = 0; j < height; ++j) {
//        png.plot(i,j,(int)(newton_pixels[i][j][0]*255),(int)(newton_pixels[i][j][1]*255),(int)(newton_pixels[i][j][2]*255));
        
//        png.plot(i,j,(int)image[i*height+j+0],(int)image[i*height+j+1],(int)image[i*height+j+2]);
        int a = (int)(65535*newton_pixels[(j*width+i)*3+0]);
        //std::cout<<" a is " << a << std::endl;
        png.plot(i,j,a,(int)(65535*newton_pixels[(j*width+i)*3+1]),(int)(65535*newton_pixels[(j*width+i)*3+2]));
        
      }
    }

      
    png.setcompressionlevel(5); // To speed things up. Set to 0 for max speed when close()ing the image.
//   png.setgamma(0.7);
   png.close();
    
   //ss2<<"convert "<<ss.str()<<" imageloop/screenshot"<<std::setw(5)<<std::setfill('0')<<screencounter<<".png";
   //ss3<<"rm "<<ss.str();
   //system(ss2.str().c_str());
   //system(ss3.str().c_str());
  }

  
  screencounter++; 
  glutTimerFunc(10,TimerFunction,0);

}

void passivemotion(int x, int y)
{
  std::cout<<"passive func at x = " << x << " and y = " << y << std::endl;
}

int main(int argc, char** argv) {
  
  // double ccc;
  // for (int i = 0; i < 100000; ++i)
  // {
  //   for (int a = 0; a < 1000; ++a) {
  //     if (a % 100 == 0)
  //       std::cout<<".";
  //     for (int b = 0; b < 100000; ++b)
  //     {
  //       double c = double(a) + double(b);
  //       ccc = c;
  //     }
  //   }
  //   std::cout<<"i = " << i << endl;
  //   sleep(1);
  // }


  //newpixels = new float[width*height*repeat_factor*repeat_factor*3];
  fractal = new newton; //(width,height); 

  newton_pixels = new float[width*height*3];
  //eq = new radial_zeros_equation(4);

  eq = new arbitrary_equation;
  lasteq = new arbitrary_equation;
  current_time = new float;
  view = new myview;

  *current_time = 0;

  // initialize the view
  view->cx = 0;
  view->cy = 0; 
  view->offx = 4; 
  view->offy = 3;

  view->width = width;
  view->height = height;


  //generate colors
  std::vector<float> col1(3),col2(3),col3(3),
    col4(3),col5(3),col6(3),
    col7(3),col8(3),col9(3);

  col1[0] = 1; col1[1] = 0; col1[2] = 0;
  col2[0] = 0; col2[1] = 1; col2[2] = 0;
  col3[0] = 0; col3[1] = 0; col3[2] = 1;
  col4[0] = 1; col4[1] = 1; col4[2] = 1;
  col5[0] = .3; col5[1] = .7; col5[2] = .5;


  //oillamp
/*
  col1[0] = 242; col1[1] = 231; col1[2] = 75;
  col2[0] = 242; col2[1] = 135; col2[2] = 5;
  col3[0] = 242; col3[1] = 89; col3[2] = 3;
  col4[0] = 242; col4[1] = 25; col4[2] = 5;
  col5[0] = 242; col5[1] = 2; col5[2] = 2;
*/

  // brown-green leaf theme
  //col1[0] = 148/255.0; col1[1] = 95/255.0; col1[2] = 21/255.0;
  //col2[0] = 209/255.0; col2[1] = 148/255.0; col2[2] = 30/255.0;
  //col3[0] = 219/255.0; col3[1] = 212/255.0; col3[2] = 158/255.0;
  //col4[0] = 86/255.0; col4[1] = 135/255.0; col4[2] = 55/255.0;
  //col5[0] = 48/255.0; col5[1] = 74/255.0; col5[2] = 17/255.0;

  
  col6[0] = .1; col6[1] = 1; col6[2] = .3;
  col7[0] = .7; col7[1] = .6; col7[2] = .1;
  col8[0] = .3; col8[1] = 0; col8[2] = .8;
  col9[0] = .7; col9[1] = .1; col9[2] = .3;
  
  colors.resize(9);

  colors[0] = col1;
  colors[1] = col2;
  colors[2] = col3;
  colors[3] = col4;
  colors[4] = col5;
  colors[5] = col6;
  colors[6] = col7;
  colors[7] = col8;
  colors[8] = col9;

  //for (int i = 0; i < colors.size(); ++i)
  //  for (int j = 0; j < colors[i].size(); ++j)
  //    colors[i][j]/=255.0;
    
  std::cout<<"about to localize zeros"<<std::endl;
  eq->localize_zeros(*current_time);
  fractal->fill_view(*view);

  fractal->newtonfill(*eq,zero_index,strength);  
  imagefill();//zero_index,strength,colors,newton_pixels);
  //std::cout<<"imagefill done"<<std::endl;
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(width, height);
  glutCreateWindow("tombone's fractals");

  glutDisplayFunc(display);
  
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0, 0.0, 0.0, 1.0);

/*
  glDisable(GL_ALPHA_TEST);
  glDisable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_DITHER);
  glDisable(GL_FOG);
  glDisable(GL_LIGHTING);
  glDisable(GL_LOGIC_OP);
  glDisable(GL_STENCIL_TEST);
  glDisable(GL_TEXTURE_1D);
  glDisable(GL_TEXTURE_2D);
  glPixelTransferi(GL_MAP_COLOR, GL_FALSE);
  glPixelTransferi(GL_RED_SCALE, 1);
  glPixelTransferi(GL_RED_BIAS, 0);
  glPixelTransferi(GL_GREEN_SCALE, 1);
  glPixelTransferi(GL_GREEN_BIAS, 0);
  glPixelTransferi(GL_BLUE_SCALE, 1);
  glPixelTransferi(GL_BLUE_BIAS, 0);
  glPixelTransferi(GL_ALPHA_SCALE, 1);
  glPixelTransferi(GL_ALPHA_BIAS, 0);
*/
  glutPassiveMotionFunc(passivemotion);
  glutReshapeFunc(reshape);
  glutTimerFunc(10,TimerFunction,0);
  glutMainLoop();
  delete fractal;

}
