
#include "Museum.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
float angle = 90.0f;
GLint direction = 1;
float popChicle = 0.0f;

bool load_TGA(TextureImage *texture, char *filename)
{
	GLubyte		TGAheader[12] = { 0,0,2,0,0,0,0,0,0,0,0,0 };
	GLubyte		TGAcompare[12];
	GLubyte		header[6];
	GLuint		bytesPerPixel;
	GLuint		imageSize;
	GLuint		temp;
	GLuint		type = GL_RGBA;

	FILE *file = fopen(filename, "rb");

	if (file == NULL ||
		fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||
		fread(header, 1, sizeof(header), file) != sizeof(header))
	{
		if (file == NULL)
			return FALSE;
		else
		{
			fclose(file);
			return FALSE;
		}
	}

	texture->width = header[1] * 256 + header[0];
	texture->height = header[3] * 256 + header[2];

	if (texture->width <= 0 ||
		texture->height <= 0 ||
		(header[4] != 24 && header[4] != 32))
	{
		fclose(file);
		return FALSE;
	}

	texture->bpp = header[4];
	bytesPerPixel = texture->bpp / 8;
	imageSize = texture->width*texture->height*bytesPerPixel;

	texture->imageData = (GLubyte *)malloc(imageSize);

	if (texture->imageData == NULL ||
		fread(texture->imageData, 1, imageSize, file) != imageSize)
	{
		if (texture->imageData != NULL)
			free(texture->imageData);

		fclose(file);
		return FALSE;
	}

	for (GLuint i = 0; i<int(imageSize); i += bytesPerPixel)
	{
		temp = texture->imageData[i];
		texture->imageData[i] = texture->imageData[i + 2];
		texture->imageData[i + 2] = temp;
	}

	fclose(file);

	glGenTextures(1, &texture[0].texID);
	glBindTexture(GL_TEXTURE_2D, texture[0].texID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (texture[0].bpp == 24)
	{
		type = GL_RGB;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);

	return true;
}

void readstr(FILE* f, char* string)
{
	do
	{
		fgets(string, 255, f);
	} while ((string[0] == '/') || (string[0] == '\n'));
}

void setup_world()
{
	float x, y, z, u, v;
	int numpolygons;
	int texid;
	FILE *filein;
	char oneline[255];
	filein = fopen("world.txt", "rt");
	readstr(filein, oneline);
	sscanf(oneline, "NUMPOLLIES %d\n", &numpolygons);
	g_sector1.polygon = new POLYGON[numpolygons];
	g_sector1.texture = new int[numpolygons];
	g_sector1.numpolygons = numpolygons;

	for (int loop = 0; loop < numpolygons; loop++)
	{
		readstr(filein, oneline);
		sscanf(oneline, "TEXTURE %d\n", &texid);
		for (int vert = 0; vert < 4; vert++)
		{
			g_sector1.texture[loop] = texid;
			readstr(filein, oneline);
			sscanf(oneline, "%f %f %f %f %f", &x, &y, &z, &u, &v);
			g_sector1.polygon[loop].vertex[vert].x = x;
			g_sector1.polygon[loop].vertex[vert].y = y;
			g_sector1.polygon[loop].vertex[vert].z = z;
			g_sector1.polygon[loop].vertex[vert].u = u;
			g_sector1.polygon[loop].vertex[vert].v = v;
		}
	}


	fclose(filein);
	return;
}

bool setup_textures()
{
	if (!load_TGA(&textures[0], "Data/tiled.tga")
		|| !load_TGA(&textures[1], "Data/ceiling.tga")
		|| !load_TGA(&textures[2], "Data/painting1.tga")
		|| !load_TGA(&textures[3], "Data/painting2.tga")
		|| !load_TGA(&textures[4], "Data/painting3.tga")
		|| !load_TGA(&textures[5], "Data/painting4.tga")
		|| !load_TGA(&textures[7], "Data/painting6.tga")
		|| !load_TGA(&textures[6], "Data/painting5.tga")
		|| !load_TGA(&textures[8], "Data/painting7.tga")
		|| !load_TGA(&textures[9], "Data/painting8.tga")
		|| !load_TGA(&textures[10], "Data/painting9.tga")
		|| !load_TGA(&textures[11], "Data/graywall.tga")
		|| !load_TGA(&textures[12], "Data/door.tga")
		|| !load_TGA(&textures[13], "Data/crosshairs.tga")
		|| !load_TGA(&textures[14], "Data/hardwood.tga")
		|| !load_TGA(&textures[15], "Data/stairsbottom.tga")
		|| !load_TGA(&textures[16], "Data/atticceiling.tga")
		|| !load_TGA(&textures[17], "Data/bluewall.tga")
		|| !load_TGA(&textures[18], "Data/railing.tga")
		|| !load_TGA(&textures[19], "Data/font.tga")
		|| !load_TGA(&textures[20], "Data/opendoor.tga")
		|| !load_TGA(&textures[21], "Data/simpledoor.tga")
		|| !load_TGA(&textures[22], "Data/column.tga")
		|| !load_TGA(&textures[23], "Data/stairstop.tga")
		|| !load_TGA(&textures[24], "Data/stairs.tga")
		|| !load_TGA(&textures[25], "Data/sun.tga")
		|| !load_TGA(&textures[26], "Data/mercury.tga")
		|| !load_TGA(&textures[27], "Data/venus.tga")
		|| !load_TGA(&textures[28], "Data/earth.tga")
		|| !load_TGA(&textures[29], "Data/mars.tga")
		|| !load_TGA(&textures[30], "Data/jupiter.tga")
		|| !load_TGA(&textures[31], "Data/saturn.tga")
		|| !load_TGA(&textures[32], "Data/uranus.tga")
		|| !load_TGA(&textures[33], "Data/neptune.tga")
		|| !load_TGA(&textures[34], "Data/pascua.tga")
		|| !load_TGA(&textures[35], "Data/roca.tga")
		|| !load_TGA(&textures[36], "Data/labioinferior.tga")
		|| !load_TGA(&textures[37], "Data/chicle.tga")
		|| !load_TGA(&textures[38], "Data/camera.tga")

		)
		return false;
	return true;
}

GLvoid build_front(GLvoid)
{

	base = glGenLists(95);
	glBindTexture(GL_TEXTURE_2D, textures[19].texID);
	for (int loop = 0; loop<95; loop++)
	{
		float cx = float(loop % 16) / 16.0f;
		float cy = float(loop / 16) / 8.0f;

		glNewList(base + loop, GL_COMPILE);
		glBegin(GL_QUADS);
		glTexCoord2f(cx, 1.0f - cy - 0.120f); glVertex2i(0, 0);
		glTexCoord2f(cx + 0.0625f, 1.0f - cy - 0.120f); glVertex2i(16, 0);
		glTexCoord2f(cx + 0.0625f, 1.0f - cy);		  glVertex2i(16, 16);
		glTexCoord2f(cx, 1.0f - cy);		  glVertex2i(0, 16);
		glEnd();
		glTranslated(15, 0, 0);
		glEndList();
	}
}


bool init()
{
	glAlphaFunc(GL_GREATER, 0.1f);
	glEnable(GL_ALPHA_TEST);
	for (int i = 0; i < 10; i++) {
		my_shape[i] = gluNewQuadric();
	}
	setup_textures();
	build_front();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glEnable(GL_TEXTURE_2D);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glShadeModel(GL_SMOOTH);
	setup_world();
	memset(g_key, 0, sizeof(g_key));
	return true;
}

void reshape(int w, int h)
{
	window_width = w;
	window_height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (h == 0) h = 1;
	gluPerspective(45.0f, (float)w / (float)h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

GLvoid gl_print(GLint x, GLint y, const char *string, ...)
{
	char		text[256];
	va_list		ap;

	if (string == NULL)
		return;

	va_start(ap, string);
	vsprintf(text, string, ap);
	va_end(ap);
	glBindTexture(GL_TEXTURE_2D, textures[19].texID);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(x, y, 0);
	glListBase(base - 32);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopMatrix();
}

void object(float width, float height, GLuint texid)
{
	glBindTexture(GL_TEXTURE_2D, textures[texid].texID);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, -height, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -height, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, height, 0.0f);
	glEnd();
}

void texture_object(int polygonNum)
{
	GLfloat x_m, y_m, z_m, u_m, v_m;

	glBindTexture(GL_TEXTURE_2D, textures[g_sector1.texture[polygonNum]].texID);
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);
	x_m = g_sector1.polygon[polygonNum].vertex[0].x;
	y_m = g_sector1.polygon[polygonNum].vertex[0].y;
	z_m = g_sector1.polygon[polygonNum].vertex[0].z;
	u_m = g_sector1.polygon[polygonNum].vertex[0].u;
	v_m = g_sector1.polygon[polygonNum].vertex[0].v;
	glTexCoord2f(u_m, v_m); glVertex3f(x_m, y_m, z_m);

	x_m = g_sector1.polygon[polygonNum].vertex[1].x;
	y_m = g_sector1.polygon[polygonNum].vertex[1].y;
	z_m = g_sector1.polygon[polygonNum].vertex[1].z;
	u_m = g_sector1.polygon[polygonNum].vertex[1].u;
	v_m = g_sector1.polygon[polygonNum].vertex[1].v;
	glTexCoord2f(u_m, v_m); glVertex3f(x_m, y_m, z_m);

	x_m = g_sector1.polygon[polygonNum].vertex[2].x;
	y_m = g_sector1.polygon[polygonNum].vertex[2].y;
	z_m = g_sector1.polygon[polygonNum].vertex[2].z;
	u_m = g_sector1.polygon[polygonNum].vertex[2].u;
	v_m = g_sector1.polygon[polygonNum].vertex[2].v;
	glTexCoord2f(u_m, v_m); glVertex3f(x_m, y_m, z_m);

	x_m = g_sector1.polygon[polygonNum].vertex[3].x;
	y_m = g_sector1.polygon[polygonNum].vertex[3].y;
	z_m = g_sector1.polygon[polygonNum].vertex[3].z;
	u_m = g_sector1.polygon[polygonNum].vertex[3].u;
	v_m = g_sector1.polygon[polygonNum].vertex[3].v;
	glTexCoord2f(u_m, v_m); glVertex3f(x_m, y_m, z_m);
	glEnd();
}


void detect() {
	boolean firstRoom;
	if (g_zpos > -1.8) {
		firstRoom = true;
	}
	else {
		firstRoom = false;
	}
	if (firstRoom && !ppressed) {
		if (g_xpos < -1.0 && g_zpos > 0.5 && g_yrot > 136.00 && g_yrot < 210.00)
			gl_print((window_width / 2) - 150, (window_height / 2), "Mona Lisa");

		else if (g_zpos < -0.65 && g_zpos > -3.0 && g_xpos < 0.5 && g_xpos > -0.5 && g_ypos < 0.5 && ((g_yrot < 30.0 || g_yrot > 340.0) || (g_yrot < 210.0  && g_yrot > 150)))
			gl_print((window_width / 2) - 150, (window_height / 2), "Press X to open door");

		else if (g_xpos > 1.00 && g_zpos < -1.10 && g_yrot < 300.00 && g_yrot > 230.00)
			gl_print((window_width / 2) - 150, (window_height / 2), "Black");

		else if (g_xpos > 1.00 && g_zpos < -0.50 && ((g_yrot > 300.00 && g_yrot < 360.00) || (g_yrot >= 0.00 && g_yrot < 30.0)))
			gl_print((window_width / 2) - 150, (window_height / 2), "Watchmen");

		else if (g_xpos < 0.5 && g_zpos > 0.83 && g_yrot > 70 && g_yrot < 120)
			gl_print((window_width / 2) - 150, (window_height / 2), "Van Gogh");

		else if (g_xpos < 1.65 && g_zpos > 0.50 && g_yrot > 136 && g_yrot < 200)
			gl_print((window_width / 2) - 150, (window_height / 2), "Noche estrellada");

		else if (g_xpos > 0.42 && g_zpos > 0.54 && g_zpos < 1.73 && g_yrot	> 230 && g_yrot < 292)
			gl_print((window_width / 2) - 150, (window_height / 2), "300");

		else if (g_zpos < 0.6 && g_xpos < -0.7 && ((g_yrot < 47.0) || (g_yrot > 340)))
			gl_print((window_width / 2) - 150, (window_height / 2), "Liberte");

		else if (g_zpos < 0.6 && g_xpos < -0.7 && g_yrot < 120.0  && g_yrot > 60)
			gl_print((window_width / 2) - 150, (window_height / 2), "Ripped face");
	}
}

void render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	GLfloat xtrans = -g_xpos;
	GLfloat ztrans = -g_zpos;
	GLfloat ytrans = -g_ypos;
	if (g_yrot > 360)
		g_yrot -= 360;
	else if (g_yrot < 0)
		g_yrot += 360;
	GLfloat sceneroty = (360.0f - g_yrot);


	int numpolygons;

	glRotatef(g_lookupdown, 1.0f, 0, 0);
	glRotatef(sceneroty, 0, 1.0f, 0);

	glTranslatef(xtrans, ytrans, ztrans);

	numpolygons = g_sector1.numpolygons;

	for (int loop_m = 0; loop_m < numpolygons; loop_m++) {
		texture_object(loop_m);
	}
	float newAng = 0;
	int i = 0;
	while (i < 90) {
		for (int p = 31; p < 34; p++) {
			glRotatef(newAng += i, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.0f, 0.0f, 0.0f);
			glRotatef(-45.0f, 1.0f, 0.0f, 0.0f);
			for (int v = 0; v < 4; v++) {
				g_sector1.polygon[p].vertex[v].x;
				g_sector1.polygon[p].vertex[v].y;
				g_sector1.polygon[p].vertex[v].z;
			}
		}
		i++;
	}


	gluQuadricDrawStyle(my_shape[0], GLU_FILL);


	glScalef(.05, .05, .05);
	glPushMatrix();
	glTranslatef(0.0, 0.5, -5.0);
	glPopMatrix();
	angle += 0.1f;
	popChicle += 0.009f;
	glPushMatrix();
	GLfloat factor = 0.0002;
	if (g_sector1.polygon[28].vertex[0].y >= 0.16 || g_sector1.polygon[28].vertex[0].y <= 0.095) {
		direction *= -1;
	}
	g_sector1.polygon[28].vertex[0].y += factor * direction;
	g_sector1.polygon[28].vertex[1].y += factor * direction;
	g_sector1.polygon[28].vertex[2].y += factor * direction;
	g_sector1.polygon[28].vertex[3].y += factor * direction;


	glPopMatrix();

	//Sun
	glPushMatrix();
	glTranslatef(-90.0, 108.0, -45.0);
	gluQuadricNormals(my_shape[0], GLU_SMOOTH);
	gluQuadricTexture(my_shape[0], GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[25].texID);
	glRotatef(angle, 1.0, 1.0, 1.0);
	gluSphere(my_shape[0], 10.0, 50, 50);
	glPopMatrix();

	//Mercury
	glPushMatrix();
	glTranslatef(-75.0, 100.0, -50.0);
	gluQuadricNormals(my_shape[1], GLU_SMOOTH);
	gluQuadricTexture(my_shape[1], GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[26].texID);
	glRotatef(angle, 1.0, 1.0, 1.0);
	gluSphere(my_shape[1], 1.0, 50, 50);
	glPopMatrix();

	//Venus
	glPushMatrix();
	glTranslatef(-75.0, 90.0, -50.0);
	gluQuadricNormals(my_shape[2], GLU_SMOOTH);
	gluQuadricTexture(my_shape[2], GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[27].texID);
	glRotatef(angle, 1.0, 1.0, 1.0);
	gluSphere(my_shape[2], 2.0, 50, 50);
	glPopMatrix();

	//Earth
	glPushMatrix();
	glTranslatef(-70.0, 90.0, -55.0);
	gluQuadricNormals(my_shape[3], GLU_SMOOTH);
	gluQuadricTexture(my_shape[3], GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[28].texID);
	glRotatef(angle, 1.0, 1.0, 1.0);
	gluSphere(my_shape[3], 2.0, 50, 50);
	glPopMatrix();

	//Mars
	glPushMatrix();
	glTranslatef(-68.0, 85.0, -60.0);
	gluQuadricNormals(my_shape[4], GLU_SMOOTH);
	gluQuadricTexture(my_shape[4], GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[29].texID);
	glRotatef(angle, 1.0, 1.0, 1.0);
	gluSphere(my_shape[4], 2.0, 50, 50);
	glPopMatrix();

	//Jupiter
	glPushMatrix();
	glTranslatef(-70.0, 85.0, -70.0);
	gluQuadricNormals(my_shape[5], GLU_SMOOTH);
	gluQuadricTexture(my_shape[5], GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[30].texID);
	glRotatef(angle, 1.0, -0.5, -1.0);
	gluSphere(my_shape[5], 4.0, 50, 50);
	glPopMatrix();

	//Saturn
	glPushMatrix();
	glTranslatef(-75.0, 80.0, -78.0);
	gluQuadricNormals(my_shape[6], GLU_SMOOTH);
	gluQuadricTexture(my_shape[6], GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[31].texID);
	glRotatef(angle, 0.5, -1.0, 1.0);
	gluSphere(my_shape[6], 3.0, 50, 50);
	glPopMatrix();

	//Uranus
	glPushMatrix();
	glTranslatef(-70.0, 78.0, -83.0);
	gluQuadricNormals(my_shape[7], GLU_SMOOTH);
	gluQuadricTexture(my_shape[7], GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[32].texID);
	glRotatef(angle, 1.0, -0.5, -1.0);
	gluSphere(my_shape[7], 1.7, 50, 50);
	glPopMatrix();

	//Neptune
	glPushMatrix();
	glTranslatef(-73.0, 78.0, -93.0);
	gluQuadricNormals(my_shape[8], GLU_SMOOTH);
	gluQuadricTexture(my_shape[8], GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[33].texID);
	glRotatef(angle, 0.0, -1.0, 0.0);
	gluSphere(my_shape[8], 3.5, 50, 50);
	glPopMatrix();

	//Gum
	glPushMatrix();
	glTranslatef(-67.0, 77.0, -19.0);
	gluQuadricNormals(my_shape[9], GLU_SMOOTH);
	gluQuadricTexture(my_shape[9], GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[37].texID);
	glRotatef(90.0, 0.0, -1.0, 0.0);
	gluSphere(my_shape[9], popChicle, 50, 50);
	glPopMatrix();

	if (popChicle >= 3.0f) {
		popChicle = 0.0f;
	}

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-10, window_width, 0, window_height, -10, 10);
	glMatrixMode(GL_MODELVIEW);

	gl_print(50, 10, "X: %f", g_xpos);
	gl_print(300, 10, "Y: %f", g_ypos);
	gl_print(500, 10, "Z: %f", g_zpos);
	if (ENTER_WAS_PRESSED) {
		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);
		gl_print((window_width / 2) - 200, (window_height / 2) + 80, "Up Arrow: Move Forward");
		gl_print((window_width / 2) - 200, (window_height / 2) + 60, "Down Arrow: Move Backwards");
		gl_print((window_width / 2) - 200, (window_height / 2) + 40, "Left Arrow: Turn Left");
		gl_print((window_width / 2) - 200, (window_height / 2) + 20, "Right Arrow: Turn Right");
		gl_print((window_width / 2) - 200, (window_height / 2), "U: Change to Camera #1");
		gl_print((window_width / 2) - 200, (window_height / 2) - 20, "I: Change to Camera #2");
		gl_print((window_width / 2) - 200, (window_height / 2) - 40, "P: Change to Aerial View");
		glEnable(GL_BLEND);
		glEnable(GL_ALPHA_TEST);
	}
	if (ipressed == true) {
		g_ypos = 0.85;
		g_xpos = 1.40;
		g_zpos = -2.59;
	}
	else {
		g_ypos = 0.45;
		g_xpos = lastx;
		g_zpos = lastz;
	}
	if (upressed == true) {
		g_ypos = 0.85;
		g_zpos = 1.79;
		g_xpos = 1.40;

	}
	if (ppressed == true) {
		g_ypos = 2.99;
		g_sector1.polygon[1].vertex[0].y = 3.0;
		g_sector1.polygon[1].vertex[1].y = 3.0;
		g_sector1.polygon[1].vertex[2].y = 3.0;
		g_sector1.polygon[1].vertex[3].y = 3.0;
	}
	detect();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glutSwapBuffers();
}

void openDoor()
{
	if (!DOOR_IS_OPEN)
	{
		DOOR_IS_OPEN = true;
		for (int i = 0; i < 110; i++)
		{
			g_sector1.polygon[21].vertex[0].x = -0.18 + (0.373 - (0.373 * cos(i*PI_OVER_180)));
			g_sector1.polygon[21].vertex[1].x = -0.18 + (0.373 - (0.373 * cos(i*PI_OVER_180)));
			g_sector1.polygon[21].vertex[0].z = -1.999 - (0.373 - (0.373 * cos(i*PI_OVER_180)));
			g_sector1.polygon[21].vertex[1].z = -1.999 - (0.373 - (0.373 * cos(i*PI_OVER_180)));
			Sleep(7);
			render();
		}
	}
	else
	{
		DOOR_IS_OPEN = false;
		for (int i = 0; i < 110; i++)
		{
			g_sector1.polygon[21].vertex[0].x = 0.314 - (0.373 - (0.373 * cos(i*PI_OVER_180)));
			g_sector1.polygon[21].vertex[1].x = 0.314 - (0.373 - (0.373 * cos(i*PI_OVER_180)));
			g_sector1.polygon[21].vertex[0].z = -2.49 + (0.373 - (0.373 * cos(i*PI_OVER_180)));
			g_sector1.polygon[21].vertex[1].z = -2.49 + (0.373 - (0.373 * cos(i*PI_OVER_180)));
			Sleep(7);
			render();
		}
	}

	glPushMatrix();
	glPopMatrix();
}

void keyboard(unsigned char key, int x, int y)
{
	GLfloat jumpTop = g_ypos + 0.25;
	GLfloat jumpBottom = g_ypos + (speed_XYZ / 2);
	switch (key)
	{
	case 'x':
	case 'X':
		openDoor();

		break;
	case 13:
		if (!ENTER_WAS_PRESSED)
			ENTER_WAS_PRESSED = true;
		else
			ENTER_WAS_PRESSED = false;
		break;
	case 'u':
	case 'U':
		ipressed = false;
		if (!ppressed) {
			if (upressed) {
				upressed = false;
				g_yrot = lasty_rot;
			}
			else {
				g_yrot = 0.0;
				g_lookupdown = 0.0;
				upressed = true;
			}
		}
		break;
	case 'i':
	case 'I':
		upressed = false;
		if (!ppressed) {
			if (ipressed) {
				ipressed = false;
				g_yrot = lasty_rot;
			}
			else
			{
				g_yrot = 0.0;
				g_lookupdown = 0.0;
				ipressed = true;
			}
		}
		break;
	case 'p':
	case 'P':
		upressed = false;
		ipressed = false;
		if (ppressed) {
			ppressed = false;
			g_lookupdown = 0;
			g_sector1.polygon[1].vertex[0].y = 1.0;
			g_sector1.polygon[1].vertex[1].y = 1.0;
			g_sector1.polygon[1].vertex[2].y = 1.0;
			g_sector1.polygon[1].vertex[3].y = 1.0;
			g_yrot = lasty_rot;
		}
		else
		{
			g_lookupdown = 60;
			ppressed = true;
		}
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
	render();
}


void special_keys(int a_keys, int x, int y)
{
	switch (a_keys)
	{
	case GLUT_KEY_F1:
		if (!g_gamemode)
		{
			g_fullscreen = !g_fullscreen;
			if (g_fullscreen) glutFullScreen();
			else glutReshapeWindow(window_width, window_height);
		}
		break;
	default:
		g_key[a_keys] = true;
		break;
	}
}

void special_keys_up(int key, int x, int y)
{
	g_key[key] = false;
}

void game_function()
{

	if (g_key[GLUT_KEY_UP])
	{
		{
			g_xpos -= (float)sin(g_yrot*PI_OVER_180) * speed_XYZ;
			lastx = g_xpos;
			g_zpos -= (float)cos(g_yrot*PI_OVER_180) * speed_XYZ;
			lastz = g_zpos;
		}
		if (!no_collision())
		{
			g_xpos += (float)sin(g_yrot*PI_OVER_180) * speed_XYZ;
			lastx = g_xpos;
			g_zpos += (float)cos(g_yrot*PI_OVER_180) * speed_XYZ;
			lastz = g_zpos;
		}
	}
	if (g_key[GLUT_KEY_DOWN])
	{
		g_xpos += (float)sin(g_yrot*PI_OVER_180) * speed_XYZ;
		lastx = g_xpos;
		g_zpos += (float)cos(g_yrot*PI_OVER_180) * speed_XYZ;
		lastz = g_zpos;


		if (!no_collision())
		{
			g_xpos -= (float)sin(g_yrot*PI_OVER_180) * speed_XYZ;
			lastx = g_xpos;
			g_zpos -= (float)cos(g_yrot*PI_OVER_180) * speed_XYZ;
			lastz = g_zpos;
		}

	}
	if (g_key[GLUT_KEY_RIGHT]) {
		g_yrot -= speed_UDLR;
		if (!upressed && !ipressed)
			lasty_rot = g_yrot;
	}
	if (g_key[GLUT_KEY_LEFT]) {
		g_yrot += speed_UDLR;
		if (!upressed && !ipressed)
			lasty_rot = g_yrot;
	}
	render();
}


bool no_collision()
{
	if (g_ypos > 1.00) {
		if (g_xpos <= -1.70) {
			return false;
		}
		else if (g_xpos >= 1.70) {
			return false;
		}
		else if (g_zpos <= -7.70) {
			return false;
		}
		else if (g_zpos >= 1.70) {
			return false;
		}
	}
	else {
		if (g_xpos > 0.30 && g_xpos < 1.60 && g_zpos < -1.65 && g_zpos > -2.60) {
			return false;
		}
		else if (g_xpos < -0.30 && g_xpos > -1.65 && g_zpos < -1.65 && g_zpos > -2.60) {
			return false;
		}
		else if (g_xpos <= -1.80) {
			return false;
		}
		else if (g_xpos >= 1.80) {
			return false;
		}
		else if (g_zpos >= 1.80) {
			return false;
		}
		else if (g_zpos <= -7.80) {
			return false;
		}
		else if (DOOR_IS_OPEN == false) {
			if (g_zpos <= -1.80) {
				return false;
			}
		}
	}
	return true;
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);
	if (g_gamemode)
	{
		glutGameModeString("640x480:16");
		if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
			glutEnterGameMode();
		else g_gamemode = false;
	}
	screen_width = glutGet(GLUT_SCREEN_WIDTH);
	screen_height = glutGet(GLUT_SCREEN_HEIGHT);
	window_width = screen_width / 1.4;
	window_height = screen_height / 1.4;

	if (!g_gamemode)
	{
		glutInitWindowSize(window_width, window_height);
		glutInitWindowPosition((screen_width - window_width) / 2, (screen_height - window_height) / 2);
		glutCreateWindow("Digital Museum");
	}

	init();
	glutIgnoreKeyRepeat(true);
	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special_keys);
	glutSpecialUpFunc(special_keys_up);
	glutIdleFunc(game_function);
	glutMainLoop();

	return 0;
}
