#include "Application.h"

#include "gl/glew.h"
#include "gl/glut.h"

#include "GlobalTimer.h"
#include "Constants.h"
#include "Camera.h"
#include "TextureManager.h"
#include "ProceduralClouds.h"

Application::Application()
{
	// Start us right in the middle of the scene, with some height.
	m_camera = new Camera(vector3(250, 10, -250));

	new GlobalTimer(500);
	GlobalTimer::getSingleton().start();

	new TextureManager;

	m_aspectRatio = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

	m_inWireframe = false;
}

Application::~Application()
{
	if (m_camera)
		delete m_camera;

	GlobalTimer::getSingleton().stop();
	delete GlobalTimer::getSingletonPtr();

	delete TextureManager::getSingletonPtr();
}

bool Application::init()
{
	glClearColor(1, .3, .3, 1.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);

	loadTextures();
	loadShaders();
	generateSky();

	m_procClouds = new ProceduralClouds(m_programID);
	if (!m_procClouds->init(7.0, .82, .01, .003, 0.003))
		return false;

	positionSun_world[0] = 250;
	positionSun_world[1] = 70;
	positionSun_world[2] = -400;

	return true;
}

void Application::loadTextures()
{
	m_terrainTexID = TextureManager::getSingleton().loadTexture("grass01.bmp");
	assert(m_terrainTexID != 0);

	m_primaryFlareID = TextureManager::getSingleton().loadTexture("primaryFlare.bmp");
	assert(m_primaryFlareID != 0);

	m_firstHaloID = TextureManager::getSingleton().loadTexture("firstHalo.bmp");
	assert(m_firstHaloID != 0);

	m_secondHaloID = TextureManager::getSingleton().loadTexture("secondHalo.bmp");
	assert(m_secondHaloID != 0);

	m_thirdHaloID = TextureManager::getSingleton().loadTexture("thirdHalo.bmp");
	assert(m_thirdHaloID != 0);

	m_fourthHaloID = TextureManager::getSingleton().loadTexture("firstHalo.bmp");
	assert(m_thirdHaloID != 0);

	m_fifthHaloID = TextureManager::getSingleton().loadTexture("secondHalo.bmp");
	assert(m_thirdHaloID != 0);
}

void Application::loadShaders()
{
	m_vpID = glCreateShader(GL_VERTEX_SHADER);
	m_fpID = glCreateShader(GL_FRAGMENT_SHADER);

	char *vs, *fs;
	vs = textFileRead("data/shaders/procClouds.vert");
	fs = textFileRead("data/shaders/procClouds.frag");

	const char *vv = vs;
	const char *ff = fs;

	glShaderSource(m_vpID, 1, &vv, NULL);
	glShaderSource(m_fpID, 1, &ff, NULL);

	free(vs);
	free(fs);

	glCompileShader(m_vpID);
	glCompileShader(m_fpID);

	m_programID = glCreateProgram();

	glAttachShader(m_programID, m_vpID);
	glAttachShader(m_programID, m_fpID);

	glLinkProgram(m_programID);
}

char *Application::textFileRead(char *fn)
{
	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fn != NULL) {
		fp = fopen(fn,"rt");

		if (fp != NULL) {

			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = (int)fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

void Application::display()
{
	GlobalTimer::getSingleton().update();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glPushMatrix();

	vector3 pos = m_camera->getPosition();
	vector3 lookAt = m_camera->getLookAt();
	vector3 up = m_camera->getUp();

	gluLookAt(pos.x, pos.y, pos.z,
			  lookAt.x + pos.x, lookAt.y + pos.y, lookAt.z + pos.z,
			  up.x, up.y, up.z);

	displayFPS();

	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	float color[4] = {.5, .5, .5, 1.0};
	glFogfv(GL_FOG_COLOR, color);
	glFogf(GL_FOG_DENSITY, .65);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, 40.0);
	glFogf(GL_FOG_END, 250.0);

	displayTerrain();

	glDisable(GL_FOG);

	if (m_inWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(m_programID);
	m_procClouds->update(GlobalTimer::getSingleton().getTimeElapsed());
	displaySky();
	glUseProgram(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	displayPrimaryFlare();

	// Double buffering.
	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}

void Application::displayFPS()
{
	// Display the FPS to console every SECONDS seconds...
	static float counter = 0.0;
	const float SECONDS = 2.0;
	counter += GlobalTimer::getSingleton().getTimeElapsed();

	if ( counter > SECONDS ) {
		unsigned long fps = GlobalTimer::getSingleton().getFrameRate();
		printf( "FPS = %i\n", fps );
		counter = 0.0;
	}
}

void Application::displayTerrain()
{
	glPushMatrix();
	glPushAttrib( GL_ALL_ATTRIB_BITS );

	glActiveTexture(GL_TEXTURE0);
	glBindTexture( GL_TEXTURE_2D, m_terrainTexID );
	glEnable( GL_TEXTURE_2D );
	glBegin( GL_QUADS );
	for ( int i = 0; i < SCENE_NUM_QUADS; ++i ) {
		for ( int j = 0; j < SCENE_NUM_QUADS; ++j ) {
			glTexCoord2f( 0.0, 0.0 );
			glVertex3f( (float)j * SCENE_QUAD_SIZE, 0.0, -(float)i * SCENE_QUAD_SIZE );
			glTexCoord2f( 1.0, 0.0 );
			glVertex3f( (float)(j + 1) * SCENE_QUAD_SIZE, 0.0, -(float)i * SCENE_QUAD_SIZE );
			glTexCoord2f( 1.0, 1.0 );
			glVertex3f( (float)(j + 1) * SCENE_QUAD_SIZE, 0.0, -(float)(i + 1) * SCENE_QUAD_SIZE );
			glTexCoord2f( 0.0, 1.0 );
			glVertex3f( (float)j * SCENE_QUAD_SIZE, 0.0, -(float)(i + 1) * SCENE_QUAD_SIZE );
		}
	}
	glEnd();
	glDisable( GL_TEXTURE_2D );

	glPopAttrib();
	glPopMatrix();
}

void Application::displaySky()
{
	glPushMatrix();
	glPushAttrib( GL_ALL_ATTRIB_BITS );

	glTranslatef(0.0f, -600.0f, 0.0f);
	glRotatef(270, 1.0f, 0.0f, 0.0f);
	glScalef(4, 4, 1);

	glBegin( GL_TRIANGLE_STRIP );

	for (int i=0; i < NumVerts; i++){
		glMultiTexCoord2f(GL_TEXTURE0, DomeVerts[i].u, DomeVerts[i].v);
		glMultiTexCoord2f(GL_TEXTURE1, DomeVerts[i].u, DomeVerts[i].v);
		glVertex3f(DomeVerts[i].x, DomeVerts[i].y, DomeVerts[i].z);
	}

	glEnd();

//	glTranslatef(0, 50, 0);
//
//	glBegin( GL_QUADS );
//
//	glMultiTexCoord2f(GL_TEXTURE0, 0, 0);
//	glMultiTexCoord2f(GL_TEXTURE1, 0, 0);
//	glVertex3f( 0, 0, 0 );
//	glMultiTexCoord2f(GL_TEXTURE0, 1, 0);
//	glMultiTexCoord2f(GL_TEXTURE1, 1, 0);
//	glVertex3f( 500, 0, 0 );
//	glMultiTexCoord2f(GL_TEXTURE0, 1, 1);
//	glMultiTexCoord2f(GL_TEXTURE1, 1, 1);
//	glVertex3f( 500, 0, -500 );
//	glMultiTexCoord2f(GL_TEXTURE0, 0, 1);
//	glMultiTexCoord2f(GL_TEXTURE1, 0, 1);
//	glVertex3f( 0, 0, -500 );
//
//	glEnd();

	glPopAttrib();
	glPopMatrix();
}

void Application::generateSky()
{
	float radius = SKY_RADIUS; // Set the radius of the skydome.
	float dTheta = SKY_DTHETA; // Change in degrees so we can create a vertex map to create a dome
	float dPhi = SKY_DPHI;   // and then render a dome into the scene based on the vertex list created.
	NumVerts = (int)((360/dTheta)*(90/dPhi)*4); // calculate the number of verticies based on the number of lattices.
	DomeVerts = new DomeVertex[NumVerts]; // create the vertex array.
	vector3 uv;// needed to calculate the uv coords for a vertex so we can add in a texture onto the dome.
	float phiRad,thetaRad, pDelPhiRad, tDelThetaRad;
	phiRad = thetaRad = 0;

	int n=0; // counter for the vertex list

	// We need to go around in circles to create a dome of vertexs
	for(int phi=0; phi <= 90-dPhi; phi += (int)dPhi){
		for(int theta=0; theta <= 360-dTheta; theta += dTheta){
			phiRad       = phi*(PI/180.0f); // convert the angles from degrees to radians
			thetaRad     = theta*(PI/180.0f);
			pDelPhiRad   = (phi+dPhi)*(PI/180.0f);
			tDelThetaRad = (theta+dTheta)*(PI/180.0f);

			// We need to calculate the vertex points and uv coordinates for phi theta and the 
			// 3 points aroud phi theta, phi+dPhi theta, phi theta+dTheta and phi+dPhi theta+dTheta(as long as phi is between -90 and 90 deg

			// Calculate the vertex point at (phi theta)
			DomeVerts[n].x = radius * sinf(phiRad) * cosf(thetaRad);
			DomeVerts[n].y = radius * sinf(phiRad) * sinf(thetaRad);
			DomeVerts[n].z = radius * cosf(phiRad);		
			//Calulate the uv texture coordinate.
			uv.x = DomeVerts[n].x; uv.y = DomeVerts[n].y; uv.z = DomeVerts[n].z;
			uv.normalize();

			// Calculate the spherical texture coordinates
			DomeVerts[n].u = (float)(atan2(uv.x, uv.z)/(PI*2)) + 0.5f;
			DomeVerts[n].v = (float)(asinf(uv.y) / PI) + 0.5f;		
			n++;

			///////////////////////////////////////////////////////////

			// Calculate the vertex point at (phi+dPhi theta)
			DomeVerts[n].x = radius * sinf(pDelPhiRad) * cosf(thetaRad);
			DomeVerts[n].y = radius * sinf(pDelPhiRad) * sinf(thetaRad);
			DomeVerts[n].z = radius * cosf(pDelPhiRad);			
			//Calulate the uv texture coordinate.
			uv.x = DomeVerts[n].x; uv.y = DomeVerts[n].y; uv.z = DomeVerts[n].z;
			uv.normalize();

			// Calculate the spherical texture coordinates
			DomeVerts[n].u = (float)(atan2(uv.x, uv.z)/(PI*2)) + 0.5f;
			DomeVerts[n].v = (float)(asinf(uv.y) / PI) + 0.5f;		
			n++;

			///////////////////////////////////////////////////////////

			// Calculate the vertex point at (phi theta+dTheta)
			DomeVerts[n].x = radius * sinf(phiRad) * cosf(tDelThetaRad);
			DomeVerts[n].y = radius * sinf(phiRad) * sinf(tDelThetaRad);
			DomeVerts[n].z = radius * cosf(phiRad);
			//Calulate the uv texture coordinate.
			uv.x = DomeVerts[n].x; uv.y = DomeVerts[n].y; uv.z = DomeVerts[n].z;
			uv.normalize();

			// Calculate the spherical texture coordinates
			DomeVerts[n].u = (float)(atan2(uv.x, uv.z)/(PI*2)) + 0.5f;
			DomeVerts[n].v = (float)(asinf(uv.y) / PI) + 0.5f;		
			n++;

			///////////////////////////////////////////////////////////

			if (phi > -90 && phi < 90){
				// Calculate the vertex point at (phi+dPhi theta+dTheta)
				DomeVerts[n].x = radius * sinf(pDelPhiRad) * cosf(tDelThetaRad);
				DomeVerts[n].y = radius * sinf(pDelPhiRad) * sinf(tDelThetaRad);
				DomeVerts[n].z = radius * cosf(pDelPhiRad);
				//Calulate the uv texture coordinate.
				uv.x = DomeVerts[n].x; uv.y = DomeVerts[n].y; uv.z = DomeVerts[n].z;
				uv.normalize();

				// Calculate the spherical texture coordinates
				DomeVerts[n].u = (float)(atan2(uv.x, uv.z)/(PI*2)) + 0.5f;
				DomeVerts[n].v = (float)(asinf(uv.y) / PI) + 0.5f;		
				n++;
			}
		}
	}

	// Some UV correction is needed.
	for (int i=0; i < NumVerts-3; i++){
		if (DomeVerts[i].u - DomeVerts[i+1].u > 0.9f)
			DomeVerts[i+1].u += 1.0f;

		if (DomeVerts[i+1].u - DomeVerts[i].u > 0.9f)
			DomeVerts[i].u += 1.0f;

		if (DomeVerts[i].u - DomeVerts[i+2].u > 0.9f)
			DomeVerts[i+2].u += 1.0f;

		if (DomeVerts[i+2].u - DomeVerts[i].u > 0.9f)
			DomeVerts[i].u += 1.0f;

		if (DomeVerts[i+1].u - DomeVerts[i+2].u > 0.9f)
			DomeVerts[i+2].u += 1.0f;

		if (DomeVerts[i+2].u - DomeVerts[i+1].u > 0.9f)
			DomeVerts[i+1].u += 1.0f;

		if (DomeVerts[i].v - DomeVerts[i+1].v > 0.8f)
			DomeVerts[i+1].v += 1.0f;

		if (DomeVerts[i+1].v - DomeVerts[i].v > 0.8f)
			DomeVerts[i].v += 1.0f;

		if (DomeVerts[i].v - DomeVerts[i+2].v > 0.8f)
			DomeVerts[i+2].v += 1.0f;

		if (DomeVerts[i+2].v - DomeVerts[i].v > 0.8f)
			DomeVerts[i].v += 1.0f;

		if (DomeVerts[i+1].v - DomeVerts[i+2].v > 0.8f)
			DomeVerts[i+2].v += 1.0f;

		if (DomeVerts[i+2].v - DomeVerts[i+1].v > 0.8f)
			DomeVerts[i+1].v += 1.0f;
	}
}

void Application::displayPrimaryFlare()
{
	glPushMatrix();
	glPushAttrib( GL_ALL_ATTRIB_BITS );

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA,GL_ONE );

	//----------------------------------------------------------------------//
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, m_primaryFlareID );
	glBegin( GL_QUADS );

	glTexCoord2f( 0.0, 0.0 );
	glVertex3f( positionSun_world[0]-50.0, positionSun_world[1]+50.0, positionSun_world[2]+0.0 );
	glTexCoord2f( 1.0, 0.0 );
	glVertex3f( positionSun_world[0]+50.0, positionSun_world[1]+50.0, positionSun_world[2]+0.0 );
	glTexCoord2f( 1.0, 1.0 );
	glVertex3f( positionSun_world[0]+50.0, positionSun_world[1]-50.0, positionSun_world[2]+0.0 );
	glTexCoord2f( 0.0, 1.0 );
	glVertex3f( positionSun_world[0]-50.0, positionSun_world[1]-50.0, positionSun_world[2]+0.0 );

	glDeleteTextures( 1, &m_primaryFlareID );
	glEnd();
	glDisable ( GL_TEXTURE_2D );
	//----------------------------------------------------------------------//

	glDisable( GL_BLEND );

	glPopAttrib();
	glPopMatrix();

	displayFirstHalo();
}

void Application::displayFirstHalo()
{	
	// arrays to hold matrix information
	GLdouble model_view[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, model_view);

	GLdouble projection[16];
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// get the Sun's screen coordinates based on 3D coordinates
	gluProject(positionSun_world[0], positionSun_world[1], positionSun_world[2],
		model_view, projection, viewport,
		(GLdouble*) &(positionSun_screen[0]), (GLdouble*) &(positionSun_screen[1]), (GLdouble*) &(positionSun_screen[2]));

	// get the dot product of sun vector and lookat vector (to check whether we are facing the sun or not)
	vector3 eye_to_sun;
	vector3 eye_to_lookat;
	float dot_sun_lookat;

	eye_to_sun.x = positionSun_world[0] - m_camera->getPosition().x;
	eye_to_sun.y = positionSun_world[1] - m_camera->getPosition().y;
	eye_to_sun.z = positionSun_world[2] - m_camera->getPosition().z;

	eye_to_lookat.x = m_camera->getLookAt().x;
	eye_to_lookat.y = m_camera->getLookAt().y;
	eye_to_lookat.z = m_camera->getLookAt().z;

	dot_sun_lookat = eye_to_sun.x*eye_to_lookat.x + eye_to_sun.y*eye_to_lookat.y + eye_to_sun.z*eye_to_lookat.z;

	// display halo iff the sun is shown within the screen
	if (positionSun_screen[0] >= 0 && positionSun_screen[0] <= viewport[2] &&
		positionSun_screen[1] >= 0 && positionSun_screen[1] <= viewport[3] &&
		dot_sun_lookat >= 0)
	{
		// get the center (x,y) of screen coordinates
		GLdouble c_x, c_y;
		c_x = viewport[2]*0.5f;
		c_y = viewport[3]*0.5f;

		// get the position halfway between the sun and the center of the screen
		GLdouble first_halo_x, first_halo_y;
		first_halo_x = c_x + (positionSun_screen[0] - c_x)*0.5f;
		first_halo_y = c_y - (positionSun_screen[1] - c_y)*0.5f;

		glPushAttrib( GL_ALL_ATTRIB_BITS );

		glMatrixMode (GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity ();
		glOrtho (0, viewport[2], viewport[3], 0.0f, 0.0f, -2.0f);

		glMatrixMode (GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glDisable(GL_DEPTH_TEST);

		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA,GL_ONE );

		//----------------------------------------------------------------------//
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, m_firstHaloID );
		glBegin( GL_QUADS );

		glVertex3f( first_halo_x-50.0, first_halo_y+50.0, 1.0 );
		glTexCoord2f( 1.0, 0.0 );
		glVertex3f( first_halo_x+50.0, first_halo_y+50.0, 1.0 );
		glTexCoord2f( 1.0, 1.0 );
		glVertex3f( first_halo_x+50.0, first_halo_y-50.0, 1.0 );
		glTexCoord2f( 0.0, 1.0 );
		glVertex3f( first_halo_x-50.0, first_halo_y-50.0, 1.0 );

		glDeleteTextures( 1, &m_firstHaloID );
		glEnd();
		glDisable( GL_TEXTURE_2D );
		//----------------------------------------------------------------------//

		glDisable( GL_BLEND );

		glPopAttrib();
		glMatrixMode (GL_PROJECTION);
		glPopMatrix();
		glMatrixMode (GL_MODELVIEW);
		glPopMatrix();	

		displaySecondHalo();
	}
}

void Application::displaySecondHalo()
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// get the center (x,y) of screen coordinates
	GLdouble c_x, c_y;
	c_x = viewport[2]*0.5f;
	c_y = viewport[3]*0.5f;

	// get the position 1/3 way between the sun and the center of the screen
	GLdouble first_halo_x, first_halo_y;
	first_halo_x = c_x + (positionSun_screen[0] - c_x)*0.33333f;
	first_halo_y = c_y - (positionSun_screen[1] - c_y)*0.33333f;

	glPushAttrib( GL_ALL_ATTRIB_BITS );

	glMatrixMode (GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity ();
	glOrtho (0, viewport[2], viewport[3], 0.0f, 0.0f, -2.0f);

	glMatrixMode (GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA,GL_ONE );

	//----------------------------------------------------------------------//
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, m_secondHaloID );
	glBegin( GL_QUADS );

	glVertex3f( first_halo_x-50.0, first_halo_y+50.0, 1.0 );
	glTexCoord2f( 1.0, 0.0 );
	glVertex3f( first_halo_x+50.0, first_halo_y+50.0, 1.0 );
	glTexCoord2f( 1.0, 1.0 );
	glVertex3f( first_halo_x+50.0, first_halo_y-50.0, 1.0 );
	glTexCoord2f( 0.0, 1.0 );
	glVertex3f( first_halo_x-50.0, first_halo_y-50.0, 1.0 );

	glDeleteTextures( 1, &m_firstHaloID );
	glEnd();
	glDisable( GL_TEXTURE_2D );
	//----------------------------------------------------------------------//

	glDisable( GL_BLEND );

	glPopAttrib();
	glMatrixMode (GL_PROJECTION);
	glPopMatrix();
	glMatrixMode (GL_MODELVIEW);
	glPopMatrix();

	displayThirdHalo();
}

void Application::displayThirdHalo()
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// get the center (x,y) of screen coordinates
	GLdouble c_x, c_y;
	c_x = viewport[2]*0.5f;
	c_y = viewport[3]*0.5f;

	// get the position 1/3 way between the sun and the center of the screen
	GLdouble first_halo_x, first_halo_y;
	first_halo_x = c_x + (positionSun_screen[0] - c_x)*(-0.05f);
	first_halo_y = c_y - (positionSun_screen[1] - c_y)*(-0.05f);

	glPushAttrib( GL_ALL_ATTRIB_BITS );

	glMatrixMode (GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity ();
	glOrtho (0, viewport[2], viewport[3], 0.0f, 0.0f, -2.0f);

	glMatrixMode (GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA,GL_ONE );

	//----------------------------------------------------------------------//
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, m_thirdHaloID );
	glBegin( GL_QUADS );

	glVertex3f( first_halo_x-25.0, first_halo_y+25.0, 1.0 );
	glTexCoord2f( 1.0, 0.0 );
	glVertex3f( first_halo_x+25.0, first_halo_y+25.0, 1.0 );
	glTexCoord2f( 1.0, 1.0 );
	glVertex3f( first_halo_x+25.0, first_halo_y-25.0, 1.0 );
	glTexCoord2f( 0.0, 1.0 );
	glVertex3f( first_halo_x-25.0, first_halo_y-25.0, 1.0 );

	glDeleteTextures( 1, &m_firstHaloID );
	glEnd();
	glDisable( GL_TEXTURE_2D );
	//----------------------------------------------------------------------//

	glDisable( GL_BLEND );

	glPopAttrib();
	glMatrixMode (GL_PROJECTION);
	glPopMatrix();
	glMatrixMode (GL_MODELVIEW);
	glPopMatrix();

	displayFourthHalo();
}

void Application::displayFourthHalo()
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// get the center (x,y) of screen coordinates
	GLdouble c_x, c_y;
	c_x = viewport[2]*0.5f;
	c_y = viewport[3]*0.5f;

	// get the position 1/3 way between the sun and the center of the screen
	GLdouble first_halo_x, first_halo_y;
	first_halo_x = c_x + (positionSun_screen[0] - c_x)*(-0.25f);
	first_halo_y = c_y - (positionSun_screen[1] - c_y)*(-0.25f);

	glPushAttrib( GL_ALL_ATTRIB_BITS );

	glMatrixMode (GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity ();
	glOrtho (0, viewport[2], viewport[3], 0.0f, 0.0f, -2.0f);

	glMatrixMode (GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA,GL_ONE );

	//----------------------------------------------------------------------//
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, m_fourthHaloID );
	glBegin( GL_QUADS );

	glVertex3f( first_halo_x-75.0, first_halo_y+75.0, 1.0 );
	glTexCoord2f( 1.0, 0.0 );
	glVertex3f( first_halo_x+75.0, first_halo_y+75.0, 1.0 );
	glTexCoord2f( 1.0, 1.0 );
	glVertex3f( first_halo_x+75.0, first_halo_y-75.0, 1.0 );
	glTexCoord2f( 0.0, 1.0 );
	glVertex3f( first_halo_x-75.0, first_halo_y-75.0, 1.0 );

	glDeleteTextures( 1, &m_firstHaloID );
	glEnd();
	glDisable( GL_TEXTURE_2D );
	//----------------------------------------------------------------------//

	glDisable( GL_BLEND );

	glPopAttrib();
	glMatrixMode (GL_PROJECTION);
	glPopMatrix();
	glMatrixMode (GL_MODELVIEW);
	glPopMatrix();

	displayFifthHalo();
}

void Application::displayFifthHalo()
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// get the center (x,y) of screen coordinates
	GLdouble c_x, c_y;
	c_x = viewport[2]*0.5f;
	c_y = viewport[3]*0.5f;

	// get the position 1/3 way between the sun and the center of the screen
	GLdouble first_halo_x, first_halo_y;
	first_halo_x = c_x + (positionSun_screen[0] - c_x)*(-0.75f);
	first_halo_y = c_y - (positionSun_screen[1] - c_y)*(-0.75f);

	glPushAttrib( GL_ALL_ATTRIB_BITS );

	glMatrixMode (GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity ();
	glOrtho (0, viewport[2], viewport[3], 0.0f, 0.0f, -2.0f);

	glMatrixMode (GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA,GL_ONE );

	//----------------------------------------------------------------------//
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, m_fifthHaloID );
	glBegin( GL_QUADS );

	glVertex3f( first_halo_x-25.0, first_halo_y+25.0, 1.0 );
	glTexCoord2f( 1.0, 0.0 );
	glVertex3f( first_halo_x+25.0, first_halo_y+25.0, 1.0 );
	glTexCoord2f( 1.0, 1.0 );
	glVertex3f( first_halo_x+25.0, first_halo_y-25.0, 1.0 );
	glTexCoord2f( 0.0, 1.0 );
	glVertex3f( first_halo_x-25.0, first_halo_y-25.0, 1.0 );

	glDeleteTextures( 1, &m_firstHaloID );
	glEnd();
	glDisable( GL_TEXTURE_2D );
	//----------------------------------------------------------------------//

	glDisable( GL_BLEND );

	glPopAttrib();
	glMatrixMode (GL_PROJECTION);
	glPopMatrix();
	glMatrixMode (GL_MODELVIEW);
	glPopMatrix();
}

void Application::idle()
{
	glutPostRedisplay();
}

void Application::reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0, (float)w / (float)h, .01, 5000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutPostRedisplay();
/*
	float r = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

	// Find the largest undistorted viewports we can use w/ these application window dimensions.
	if ( r > ((float)w / (float)h) )    glViewport( 0, 0, w, (float)w / r );
	else                                glViewport( 0, 0, (float)h * r, h );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0, r, .01, 2000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutPostRedisplay();*/
}

void Application::processNormalKeys(unsigned char key, int x, int y)
{
	// Which key was pressed?
	switch ( key ) {
			 // Quit (ESC).
		 case 27:
			 exit(-1);
			 break;

			 // Wireframe toggle.
		 case 'w':
			 m_inWireframe = !m_inWireframe;
			 break;

		 case '1':
			 m_currentParam = CLOUD_FORMATION;
			 printf("Cloud Formation\n");
			 break;

		 case '2':
			 m_currentParam = CLOUD_COVER;
			 printf("Cloud Cover\n");
			 break;

		 case '3':
			 m_currentParam = CLOUD_SHARPNESS;
			 printf("Cloud Sharpness\n");
			 break;

		 case '4':
			 m_currentParam = CLOUD_SCROLL_U;
			 printf("Cloud Scroll U\n");
			 break;

		 case '5':
			 m_currentParam = CLOUD_SCROLL_V;
			 printf("Cloud Scroll V\n");
			 break;

		 case '[':
			 switch (m_currentParam)
			 {
			 case CLOUD_FORMATION:
				 m_procClouds->setCloudFormationUpdateInterval(-.5);
				 printf("Update Interval: %f\n", m_procClouds->getCloudFormationUpdateInterval());
				 break;

			 case CLOUD_COVER:
				 m_procClouds->setCloudCover(-.01);
				 printf("Cloud Cover: %f\n", m_procClouds->getCloudCover());
				 break;

			 case CLOUD_SHARPNESS:
				 m_procClouds->setCloudSharpness(-.001);
				 printf("Cloud Sharpness: %f\n", m_procClouds->getCloudSharpness());
				 break;

			 case CLOUD_SCROLL_U:
				 m_procClouds->setCloudScrollU(-.001);
				 printf("Scroll U: %f\n", m_procClouds->getCloudScrollU());
				 break;

			 case CLOUD_SCROLL_V:
				 m_procClouds->setCloudScrollV(-.001);
				 printf("Scroll V: %f\n", m_procClouds->getCloudScrollV());
				 break;
			 }
			 break;

		 case ']':
			 switch (m_currentParam)
			 {
				 case CLOUD_FORMATION:
					 m_procClouds->setCloudFormationUpdateInterval(.5);
					 printf("Update Interval: %f\n", m_procClouds->getCloudFormationUpdateInterval());
					 break;

				 case CLOUD_COVER:
					 m_procClouds->setCloudCover(.01);
					 printf("Cloud Cover: %f\n", m_procClouds->getCloudCover());
					 break;

				 case CLOUD_SHARPNESS:
					 m_procClouds->setCloudSharpness(.001);
					 printf("Cloud Sharpness: %f\n", m_procClouds->getCloudSharpness());
					 break;

				 case CLOUD_SCROLL_U:
					 m_procClouds->setCloudScrollU(.001);
					 printf("Scroll U: %f\n", m_procClouds->getCloudScrollU());
					 break;

				 case CLOUD_SCROLL_V:
					 m_procClouds->setCloudScrollV(.001);
					 printf("Scroll V: %f\n", m_procClouds->getCloudScrollV());
					 break;
			 }
			 break;
	}

	glutPostRedisplay();
}

void Application::processSpecialKeys(int key, int x, int y)
{
	static const int xMove = 150;
	static const int yMove = 150;

	// Which key was pressed?
	switch ( key ) {
		 case GLUT_KEY_UP:
			 m_camera->pitch(-yMove);
			 break;

		 case GLUT_KEY_DOWN:
			 m_camera->pitch(yMove);
			 break;

		 case GLUT_KEY_RIGHT:
			 m_camera->yaw(xMove);
			 break;

		 case GLUT_KEY_LEFT:
			 m_camera->yaw(-xMove);
			 break;
	}
}