#include "StdAfx.h"

/*
 * This source file is part of libRocket, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://www.librocket.com
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "LibRocketRenderInterface.h"
#include <Rocket/Core.h>
#include "cinder/gl/Vbo.h"
#include "cinder/app/App.h"

#define GL_CLAMP_TO_EDGE 0x812F

using namespace ci;
using namespace ci::app;
using namespace std;

// Local helper class to store texture handler
struct CustomTextureHanlder
{
	gl::TextureRef texture;
	GLuint textureId;
};


LibRocketRenderInterface::LibRocketRenderInterface()
{
    m_width = 1024;
    m_height = 768;
}

void LibRocketRenderInterface::SetViewport(int width, int height)
{
    m_width = width;
    m_height = height;
}


// Called by Rocket when it wants to render geometry that it does not wish to optimise.
void LibRocketRenderInterface::RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, const Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation)
{
    vector<uint32_t>    vbo_indices;
	vector<Vec3f>       vbo_positions;
	vector<Vec2f>       vbo_texCoords;
	vector<ColorA>      vbo_colors;
    
    //    gl::enableDepthRead();
    //    gl::enableDepthWrite();
    
    gl::VboMesh::Layout layout;
	layout.setStaticIndices();
	layout.setStaticPositions();
	layout.setStaticTexCoords2d();
    layout.setStaticColorsRGBA();
    
    ci::gl::VboMesh mesh = gl::VboMesh( num_vertices, num_indices, layout, GL_TRIANGLES );
    
    Vec3f pos;
    
	for( int i=0; i < num_vertices; i++ )
    {
        vbo_positions.push_back( Vec3f(vertices[i].position.x, vertices[i].position.y, 0) );
		vbo_colors.push_back( ColorA( vertices[i].colour.red/255.f, vertices[i].colour.green/255.f, vertices[i].colour.blue/255.f, vertices[i].colour.alpha/255.f ) );
		vbo_texCoords.push_back( Vec2f( vertices[i].tex_coord.x, vertices[i].tex_coord.y ) );
	}
    
	for( int i=0; i<num_indices; i++ )
		vbo_indices.push_back( indices[i] );
    
	mesh.bufferIndices( vbo_indices );
	mesh.bufferPositions( vbo_positions );
	mesh.bufferTexCoords2d( 0, vbo_texCoords );
    mesh.bufferColorsRGBA( vbo_colors );
    
    glEnable(GL_TEXTURE_2D);
	CustomTextureHanlder* tex = (CustomTextureHanlder*) texture;
	int textureId = tex ? tex->textureId : 0;

	glBindTexture(GL_TEXTURE_2D, textureId);
    gl::enableAlphaBlending();
    gl::pushMatrices();
    gl::translate(Vec2f(translation.x, translation.y));
    gl::draw( mesh );
    
    gl::popMatrices();
    glDisable(GL_TEXTURE_2D);
    gl::disableAlphaBlending();
    
    vbo_indices.clear();
    vbo_positions.clear();
    vbo_colors.clear();
    vbo_texCoords.clear();
}


// Called by Rocket when it wants to compile geometry it believes will be static for the forseeable future.
Rocket::Core::CompiledGeometryHandle LibRocketRenderInterface::CompileGeometry(Rocket::Core::Vertex* UNUSED_vertices, int num_vertices, int* indices, int num_indices, const Rocket::Core::TextureHandle texture)
{
	return (Rocket::Core::CompiledGeometryHandle) NULL;
}

// Called by Rocket when it wants to render application-compiled geometry.
void LibRocketRenderInterface::RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry, const Rocket::Core::Vector2f& translation)
{
}

// Called by Rocket when it wants to release application-compiled geometry.
void LibRocketRenderInterface::ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry)
{
}

// Called by Rocket when it wants to enable or disable scissoring to clip content.
void LibRocketRenderInterface::EnableScissorRegion(bool enable)
{
	if (enable)
		glEnable(GL_SCISSOR_TEST);
	else
		glDisable(GL_SCISSOR_TEST);
}

// Called by Rocket when it wants to change the scissor region.
void LibRocketRenderInterface::SetScissorRegion(int x, int y, int width, int height)
{
	glScissor(x, m_height - (y + height), width, height);
}

// Set to byte packing, or the compiler will expand our struct, which means it won't read correctly from file
#pragma pack(1)
struct TGAHeader
{
	char  idLength;
	char  colourMapType;
	char  dataType;
	short int colourMapOrigin;
	short int colourMapLength;
	char  colourMapDepth;
	short int xOrigin;
	short int yOrigin;
	short int width;
	short int height;
	char  bitsPerPixel;
	char  imageDescriptor;
};
// Restore packing
#pragma pack()

// Called by Rocket when a texture is required by the library.
bool LibRocketRenderInterface::LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source)
{
	gl::TextureRef texture = gl::TextureRef(new gl::Texture(loadImage(source.CString())));


	if (texture) 
	{
		CustomTextureHanlder* customHandler = new CustomTextureHanlder();
		customHandler->textureId = texture->getId();
		customHandler->texture = texture;

		texture_handle = (Rocket::Core::TextureHandle) customHandler;
		texture_dimensions = Rocket::Core::Vector2i(texture->getWidth(), texture->getHeight());

		return true;
	}

	return true;
}

// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
bool LibRocketRenderInterface::GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const Rocket::Core::byte* source, const Rocket::Core::Vector2i& source_dimensions)
{
	GLuint texture_id = 0;
	glGenTextures(1, &texture_id);
	if (texture_id == 0)
	{
		printf("Failed to generate textures\n");
		return false;
	}
    
	glBindTexture(GL_TEXTURE_2D, texture_id);
    
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, source_dimensions.x, source_dimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, source);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
	CustomTextureHanlder* customHandler = new CustomTextureHanlder();
	customHandler->textureId = texture_id;

	texture_handle = (Rocket::Core::TextureHandle) customHandler;
    
	return true;
}

// Called by Rocket when a loaded texture is no longer required.
void LibRocketRenderInterface::ReleaseTexture(Rocket::Core::TextureHandle texture_handle)
{
	CustomTextureHanlder* customHandler = (CustomTextureHanlder*)texture_handle;

	glDeleteTextures(1, &customHandler->textureId);
	delete customHandler;
}

