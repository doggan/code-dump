#include "ProceduralClouds.h"

#include "TextureManager.h"
#include "gl/glew.h"

#include <cassert>

ProceduralClouds::ProceduralClouds(unsigned int programID)
: m_programID(programID)
{
}

ProceduralClouds::~ProceduralClouds()
{
	// Clean up.
	delete [] m_skyColorData;
	delete [] m_tex_Octave1_data;
	delete [] m_tex_Octave1_cur_data;
	delete [] m_tex_Octave1_pre_data;
	delete [] m_tex_Octave2_data;
	delete [] m_tex_Octave2_cur_data;
	delete [] m_tex_Octave2_pre_data;
	delete [] m_tex_Octave3_data;
	delete [] m_tex_Octave3_cur_data;
	delete [] m_tex_Octave3_pre_data;
	delete [] m_tex_Octave4_data;
	delete [] m_tex_Octave4_cur_data;
	delete [] m_tex_Octave4_pre_data;
}

bool ProceduralClouds::init(float updateInterval, float cloudCover, float cloudSharpness, float scrollU, float scrollV)
{
	glPushMatrix();
	glPushAttrib( GL_ALL_ATTRIB_BITS );

	m_updateInterval = updateInterval;
	m_scrollU = scrollU;
	m_scrollV = scrollV;
	m_cloudCover = cloudCover;
	m_cloudSharpness = cloudSharpness;

	srand(0);	// TODO: remove once better PRNG is implemented

	// Sky color texture.
	m_skyColorData = new unsigned char [128 * 128 * 4];
	glActiveTexture(GL_TEXTURE0);
	m_skyColor = TextureManager::getSingleton().loadTexture(m_skyColorData, 128, 128, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, m_skyColor);
	glUseProgram(m_programID);
	glUniform1i(glGetUniformLocation(m_programID, "tex_skyColor"), 0);
	glUseProgram(0);

	generateSkyColor(m_skyColor);

	// Create each octave texture.
	// Octave 1.
	m_tex_Octave1_data = new unsigned char [32 * 32 * 4];
	m_tex_Octave1_cur_data = new unsigned char [32 * 32 * 4];
	m_tex_Octave1_pre_data = new unsigned char [32 * 32 * 4];
	glActiveTexture(GL_TEXTURE1);
	m_tex_Octave1 = TextureManager::getSingleton().loadTexture(m_tex_Octave1_data, 32, 32, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, m_tex_Octave1);
	glUseProgram(m_programID);
	glUniform1i(glGetUniformLocation(m_programID, "tex_octave1"), 1);
	glUseProgram(0);
	
	generateNoise(m_tex_Octave1_pre_data, NULL);
	generateNoise(m_tex_Octave1_cur_data, NULL);

	// Octave 2.
	m_tex_Octave2_data = new unsigned char [32 * 32 * 4];
	m_tex_Octave2_cur_data = new unsigned char [32 * 32 * 4];
	m_tex_Octave2_pre_data = new unsigned char [32 * 32 * 4];
	glActiveTexture(GL_TEXTURE2);
	m_tex_Octave2 = TextureManager::getSingleton().loadTexture(m_tex_Octave2_data, 32, 32, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, m_tex_Octave2);
	glUseProgram(m_programID);
	glUniform1i(glGetUniformLocation(m_programID, "tex_octave2"), 2);
	glUseProgram(0);

	generateNoise(m_tex_Octave2_pre_data, NULL);
	generateNoise(m_tex_Octave2_cur_data, NULL);

	// Octave 3.
	m_tex_Octave3_data = new unsigned char [32 * 32 * 4];
	m_tex_Octave3_cur_data = new unsigned char [32 * 32 * 4];
	m_tex_Octave3_pre_data = new unsigned char [32 * 32 * 4];
	glActiveTexture(GL_TEXTURE3);
	m_tex_Octave2 = TextureManager::getSingleton().loadTexture(m_tex_Octave3_data, 32, 32, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, m_tex_Octave3);
	glUseProgram(m_programID);
	glUniform1i(glGetUniformLocation(m_programID, "tex_octave3"), 3);
	glUseProgram(0);

	generateNoise(m_tex_Octave3_pre_data, NULL);
	generateNoise(m_tex_Octave3_cur_data, NULL);

	// Octave 4.
	m_tex_Octave4_data = new unsigned char [32 * 32 * 4];
	m_tex_Octave4_cur_data = new unsigned char [32 * 32 * 4];
	m_tex_Octave4_pre_data = new unsigned char [32 * 32 * 4];
	glActiveTexture(GL_TEXTURE4);
	m_tex_Octave2 = TextureManager::getSingleton().loadTexture(m_tex_Octave4_data, 32, 32, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, m_tex_Octave4);
	glUseProgram(m_programID);
	glUniform1i(glGetUniformLocation(m_programID, "tex_octave4"), 4);
	glUseProgram(0);

	generateNoise(m_tex_Octave4_pre_data, NULL);
	generateNoise(m_tex_Octave4_cur_data, NULL);

	glUseProgram(m_programID);
	glUniform1f(glGetUniformLocation(m_programID, "cloudCover"), m_cloudCover);
	glUniform1f(glGetUniformLocation(m_programID, "cloudSharpness"), m_cloudSharpness);
	glUniform1f(glGetUniformLocation(m_programID, "scrollSpeedU"), m_scrollU);
	glUniform1f(glGetUniformLocation(m_programID, "scrollSpeedV"), m_scrollV);
	glUseProgram(0);

	glPopAttrib();
	glPopMatrix();

	return true;
}

void ProceduralClouds::update(float timeSinceLastFrame)
{
	// TODO: not sure why this breaks other stuff.
//	glPushMatrix();
//	glPushAttrib( GL_ALL_ATTRIB_BITS );

	static float tScroll = 0.0f;

	// Used for texture scrolling animation.
	glUniform1f(glGetUniformLocation(m_programID, "theTime"), tScroll);

	tScroll += timeSinceLastFrame;

	// This time is used for cloud formation animation.
	static float runningTime = 0.0f;

	if (runningTime > m_updateInterval)
	{
		// Generate some new noise, saving out old noise.
		generateNoise(m_tex_Octave1_cur_data, m_tex_Octave1_pre_data);
		generateNoise(m_tex_Octave2_cur_data, m_tex_Octave2_pre_data);
		generateNoise(m_tex_Octave3_cur_data, m_tex_Octave3_pre_data);
		generateNoise(m_tex_Octave4_cur_data, m_tex_Octave4_pre_data);

		runningTime -= m_updateInterval;
	}

	// Noise interpolation (cloud formation animation).
	float t = runningTime / m_updateInterval;
	if (t < 0.0f)	t = 0.0f;
	if (t > 1.0f)	t = 1.0f;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_skyColor);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_tex_Octave1);
	interpolateNoise(t, m_tex_Octave1_pre_data, m_tex_Octave1_cur_data, m_tex_Octave1_data);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_tex_Octave2);
	interpolateNoise(t, m_tex_Octave2_pre_data, m_tex_Octave2_cur_data, m_tex_Octave2_data);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_tex_Octave3);
	interpolateNoise(t, m_tex_Octave3_pre_data, m_tex_Octave3_cur_data, m_tex_Octave3_data);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_tex_Octave4);
	interpolateNoise(t, m_tex_Octave4_pre_data, m_tex_Octave4_cur_data, m_tex_Octave4_data);

	runningTime += timeSinceLastFrame;

	glActiveTexture(GL_TEXTURE0);

//	glPopAttrib();
//	glPopMatrix();
}

void ProceduralClouds::generateNoise(unsigned char *textureData, unsigned char *prevTextureData)
{
	// Fill in pixel data.
	for (unsigned int row = 0; row < 32; ++row)
	{
		for(unsigned int col = 0; col < 32; ++col)
		{
			unsigned int base = indexInto(row, col, 32);

			// Save out current into prev.
			if (prevTextureData != NULL)
			{
				prevTextureData[base] = textureData[base];
				prevTextureData[base + 1] = textureData[base + 1];
				prevTextureData[base + 2] = textureData[base + 2];
				prevTextureData[base + 3] = textureData[base + 3];
			}

			// TODO: implement better noise.
			// Noise generation.
			float r = (float)rand() / ((float)RAND_MAX+1.0f);
			unsigned char rInt = (unsigned char)(r * 255.0f);
			assert(rInt >= 0 && rInt <= 255);

			textureData[base] = rInt;		// R
			textureData[base + 1] = rInt;	// G
			textureData[base + 2] = rInt;	// B
			textureData[base + 3] = 255;	// A
		}
	}
}

void ProceduralClouds::generateSkyColor(unsigned int skyTexture)
{
	TextureManager::textureInfo ti = TextureManager::getSingleton().findTex(skyTexture);

	// Fill in pixel data.
	for (unsigned int row = 0; row < ti.height; ++row)
	{
		for(unsigned int col = 0; col < ti.width; ++col)
		{
			unsigned int base = indexInto(row, col, ti.width);
			m_skyColorData[base] = 0;		// R
			m_skyColorData[base + 1] = 0;	// G
			m_skyColorData[base + 2] = 255;	// B
			m_skyColorData[base + 3] = 255;	// A
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, ti.width, ti.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_skyColorData);
}

void ProceduralClouds::interpolateNoise(float t, unsigned char *tex1, unsigned char *tex2, unsigned char *dest)
{
	// Fill in pixel data.
	for (unsigned int row = 0; row < 32; ++row)
	{
		for(unsigned int col = 0; col < 32; ++col)
		{
			unsigned int base = indexInto(row, col, 32);

			// Interpolate.
			dest[base] = unsigned char((float)tex1[base] * (1.0f - t) + (float)tex2[base] * t);
			dest[base + 1] = unsigned char((float)tex1[base + 1] * (1.0f - t) + (float)tex2[base + 1] * t);
			dest[base + 2] = unsigned char((float)tex1[base + 2] * (1.0f - t) + (float)tex2[base + 2] * t);
			dest[base + 3] = unsigned char((float)tex1[base + 3] * (1.0f - t) + (float)tex2[base + 3] * t);
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, dest);
}

void ProceduralClouds::setCloudFormationUpdateInterval(float delta)
{
	m_updateInterval += delta;
	if (m_updateInterval < 0.0f)
		m_updateInterval = 0.0f;
}

void ProceduralClouds::setCloudScrollU(float delta)
{
	m_scrollU += delta;
	if (m_scrollU < 0.0f)
		m_scrollU = 0.0f;

	glUseProgram(m_programID);
	glUniform1f(glGetUniformLocation(m_programID, "scrollSpeedU"), m_scrollU);
	glUseProgram(0);
}

void ProceduralClouds::setCloudScrollV(float delta)
{
	m_scrollV += delta;
	if (m_scrollV < 0.0f)
		m_scrollV = 0.0f;

	glUseProgram(m_programID);
	glUniform1f(glGetUniformLocation(m_programID, "scrollSpeedV"), m_scrollV);
	glUseProgram(0);
}

void ProceduralClouds::setCloudCover(float delta)
{
	m_cloudCover += delta;
	if (m_cloudCover < 0.0f)
		m_cloudCover = 0.0f;
	else if (m_cloudCover > 1.0f)
		m_cloudCover = 1.0f;

	glUseProgram(m_programID);
	glUniform1f(glGetUniformLocation(m_programID, "cloudCover"), m_cloudCover);
	glUseProgram(0);
}

void ProceduralClouds::setCloudSharpness(float delta)
{
	m_cloudSharpness += delta;
	if (m_cloudSharpness < 0.0f)
		m_cloudSharpness = 0.0f;
	else if (m_cloudSharpness > 1.0f)
		m_cloudSharpness = 1.0f;

	glUseProgram(m_programID);
	glUniform1f(glGetUniformLocation(m_programID, "cloudSharpness"), m_cloudSharpness);
	glUseProgram(0);
}