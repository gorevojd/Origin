#ifndef GDA_GL_H
#define GDA_GL_H

#ifndef GDA_IMPLEMENTATION
#include "gda.h"
#endif

#ifndef STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#ifndef GDA_GL_DEF 
#define GDA_GL_DEF extern
#endif

#ifdef __cplusplus
extern "C"{
#endif

	typedef enum gdaTextureType{
		gdaTexture_2D,
		gdaTexture_Cubemap,
		gdaTexture_Count
	}gdaTextureType;

	typedef struct gdaTexture{
		s32 width, height, channel_count;
		//
		gdaTextureType type;
		u32 texture_id;
	};

	GDA_GL_DEF b32 gda_load_texture2d_from_file(gdaTexture* texture, b32 flip_vertically, char const *filename);
	GDA_GL_DEF b32 gda_load_texture2d_from_memory(gdaTexture* texture, void* data, s32 width, s32 height, s32 channel_count);
	GDA_GL_DEF void gda_destroy_texture(gdaTexture* texture);
#ifdef __cplusplus
}
#endif

#endif

#ifdef GDA_GL_IMPLEMENTATION

b32 gda_load_texture2d_from_file(gdaTexture* texture, b32 flip_vertically, char const *filename){
	b32 r = false;
	u8* data;
	int width, height, comp;
	char* path;

	stbi_set_flip_vertically_on_load(flip_vertically);
	data = stbi_load(filename, &width, &height, &comp, 0);
	if (data != NULL){
		r = gda_load_texture2d_from_memory(texture, data, width, height, comp);
		stbi_image_free(data);
	}
	return(r);
}

b32 gda_load_texture2D_from_memory(gdaTexture* texture, void* data, s32 width, s32 height, s32 channel_count){
	b32 result = true;
	texture->width = width;
	texture->height = height;
	texture->channel_count = channel_count;
	texture->type = gdaTexture_2D;

	glGenTextures(1, &texture->texture_id);
	glBindTexture(GL_TEXTURE_2D, texture->texture_id);

	glBindTexture(GL_TEXTURE_2D, 0);
	glGenerateMipmap(GL_TEXTURE_2D);
	glFinish();
	return(result);
}

void gda_destroy_texture(gdaTexture* texture){
	if (texture->texture_id){
		glDeleteTextures(1, &texture->texture_id);
	}
}

#endif