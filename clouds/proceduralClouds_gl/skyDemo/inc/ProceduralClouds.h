#ifndef _ProceduralClouds_h_
#define _ProceduralClouds_h_

class ProceduralClouds
{
public:
	ProceduralClouds(unsigned int programID);
	virtual ~ProceduralClouds();
	
	/* Initialize all textures and materials. Create starting octaves.
	*/
	bool init(float updateInterval, float cloudCover, float cloudSharpness, float scrollU, float scrollV);

	/* Animate the material (cloud motion and formation).
	*/
	void update(float timeSinceLastFrame);

	/* Affects formation speed. # of seconds (> 0).
	*/
	void setCloudFormationUpdateInterval(float delta);
	float getCloudFormationUpdateInterval() { return m_updateInterval; }

	/* Cloud scrolling speed in U direction. (> 0).
	*/
	void setCloudScrollU(float delta);
	float getCloudScrollU() { return m_scrollU; }

	/* Cloud scrolling speed in V direction. (> 0).
	*/
	void setCloudScrollV(float delta);
	float getCloudScrollV() { return m_scrollV; }

	/* Cloud cover. 0 (no clouds) to 1 (max clouds).
	*/
	void setCloudCover(float delta);
	float getCloudCover() { return m_cloudCover; }

	/* Cloud 'sharpness'. 0 (fluffy) to 1 (hard edges).
	*/
	void setCloudSharpness(float delta);
	float getCloudSharpness() { return m_cloudSharpness; }

private:
	/* Generate noise for the given texture, overwriting existing data.
	* If prevTexture is supplied, save out texture to prevTexture before
	* generating new values.
	*/
	void generateNoise(unsigned char *textureData, unsigned char *prevTextureData);

	// TODO: put smoothNoise back in this.. test it out here?

	/* Assign the sky some color.
	* TODO: Let's make this better. Right now, we're just turning the sky a constant blue.
	*/
	void generateSkyColor(unsigned int skyTexture);

	/* LERPs between tex1 and tex2 by t. Write results to dest.
	*/
	void interpolateNoise(float t, unsigned char *tex1, unsigned char *tex2, unsigned char *dest);

	/* Given a row/column, return the correct index into our 1D array.
	*/
	inline unsigned int indexInto(unsigned int row, unsigned int col, unsigned int width)
	{
		return (row * width + col) * 4;
	}

	unsigned int m_programID;

	// The textures.
	unsigned char *m_skyColorData;
	unsigned int m_skyColor;

	unsigned char *m_tex_Octave1_data, *m_tex_Octave1_cur_data, *m_tex_Octave1_pre_data;
	unsigned int m_tex_Octave1;

	unsigned char *m_tex_Octave2_data, *m_tex_Octave2_cur_data, *m_tex_Octave2_pre_data;
	unsigned int m_tex_Octave2;

	unsigned char *m_tex_Octave3_data, *m_tex_Octave3_cur_data, *m_tex_Octave3_pre_data;
	unsigned int m_tex_Octave3;

	unsigned char *m_tex_Octave4_data, *m_tex_Octave4_cur_data, *m_tex_Octave4_pre_data;
	unsigned int m_tex_Octave4;

	// Shader variables.
	float m_updateInterval;		// Seconds between octave updates.
	float m_scrollU, m_scrollV;	// Scroll the cloud texture along u/v.
	float m_cloudCover;			// Cloud cover (density).
	float m_cloudSharpness;		// "Sharpness" of cloud outline.
};

#endif // _ProceduralClouds_h_
