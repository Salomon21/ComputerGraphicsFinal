
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <windows.h>
#include "glut.h"

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#define PI_OVER_180  0.0174532925f
#define PI_OVER_360  0.0174532925f

//0.00872664625f

typedef struct
{
	GLubyte	*imageData;
	GLuint	bpp;
	GLuint	width;
	GLuint	height;
	GLuint	texID;
} TextureImage;

typedef struct _VERTEX {
	float x, y, z;
	float u, v;
} VERTEX;

typedef struct _POLYGON {
	VERTEX vertex[4];
} POLYGON;

typedef struct _SECTOR {
	int numpolygons;
	int* texture;
	POLYGON* polygon;
} SECTOR;

bool    g_gamemode;					
bool    g_fullscreen;				
bool    g_blend			= true;		
bool    g_key[255];					
bool	DOOR_IS_OPEN	= false;	
bool	ENTER_WAS_PRESSED = false;	
GLfloat g_xpos			= -0.5410f;		
GLfloat g_zpos			= -6.2173f;		
GLfloat g_ypos			= 0.45f;
GLfloat lasty = 0.45f;
GLfloat lastz = 0.0f;
GLfloat lastx = 0.0f;
GLfloat lasty_rot;
GLfloat g_xspeed		= 0.0f;	    
GLfloat g_yspeed		= 0.0f;	    
GLfloat	g_z				= 0.0f;	    
GLfloat g_yrot			= 0.0f;		
GLfloat speed_XYZ		= 0.075f;	
GLfloat speed_UDLR		= 0.875f;	
GLfloat g_lookupdown	= 0.0f;		
GLfloat light_position[] = {0.0,1.5,-2.0,1.0};
GLuint	g_filter;
GLuint	base;
SECTOR  g_sector1;
TextureImage textures[50];
GLUquadricObj *my_shape[10];
int window_width, window_height, screen_height, screen_width;


bool setup_textures(char *name);
GLvoid build_font(GLvoid);										
void game_function();											
GLvoid gl_print(GLint x, GLint y, const char *string, ...);		
bool init();													
void keyboard(unsigned char key, int x, int y);					
void readstr(FILE* f, char* string);							
void render(void);												
void reshape(int w, int h);										
void setup_world();												
bool upressed = false;
bool ipressed = false;
bool ppressed = false;
void special_keys(int a_keys, int x, int y);					
void special_keys_up(int key, int x, int y);					
bool no_collision();											
bool Load_TGA(TextureImage *texture, char *filename);			
void object(float width,float height,GLuint texid);				
void texture_object(int polygonNum);							
