#pragma once

#include <string>
#include <iostream>
using namespace std;

#include <Windows.h>
#include <gl\GL.h>

#include "IL\il.h"

class TextField
{
public:
	//TextField(unsigned char *pixelData, int pixelWidth, int pixelHeight);
	TextField();
	TextField(ILubyte fontId);
	~TextField(void);

	inline void setFontId(ILubyte fontId);
	inline ILubyte getFontId() { return m_font_id; }

	inline GLuint getTextureId() { if(m_dirty) updateTexture(); return m_texture_id; }
	inline int getTextureWidth() { if(m_dirty) updateTexture(); return m_texture_width; }
	inline int getTextureHeight() { if(m_dirty) updateTexture(); return m_texture_height; }

	inline void setText(string text) { m_text = text; m_dirty = true; }
	inline void appendText(string text) { m_text.append(text); m_dirty = true; }
	inline void clearText() { m_text.clear(); m_dirty = true; }

	inline void setColour(GLuint colour) { m_colour = colour; };
	inline GLuint getColour() { return m_colour; }

	void render(int x, int y);

protected:
	void init();
	void updateTexture();
	void blit(char letter, int x, int y);

	//unsigned char *m_pixel_data;

	string m_text;
	int m_dirty;

	ILubyte m_image_id;
	ILubyte m_font_id;

	GLuint m_colour;

	//int m_pixel_width;
	//int m_pixel_height;

	ILubyte *m_temp;

	float m_char_width;
	float m_char_height;

	int m_texture_width;
	int m_texture_height;

	GLuint m_texture_id;
};

