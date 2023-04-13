#pragma once

#include "p6/p6.h"

class WaterFrameBuffers {
public:
    WaterFrameBuffers();

    ~WaterFrameBuffers();

    void bindReflectionFrameBuffer();
    void bindRefractionFrameBuffer();

    void unbindCurrentFrameBuffer();

    GLuint getReflectionTexture() const;
    GLuint getRefractionTexture() const;
    GLuint getRefractionDepthTexture() const;

    void check() const;

private:

    // int REFLECTION_WIDTH = 320;
	// int REFLECTION_HEIGHT = 180;
    int REFLECTION_WIDTH = 1280;
	int REFLECTION_HEIGHT = 720;
	
	int REFRACTION_WIDTH = 1280;
	int REFRACTION_HEIGHT = 720;

    GLuint reflectionFrameBuffer = 0;
    GLuint reflectionTexture = 0;
    GLuint reflectionDepthBuffer = 0;

    GLuint refractionFrameBuffer = 0;
    GLuint refractionTexture = 0;
    GLuint refractionDepthTexture = 0;

    
    void initialiseReflectionFrameBuffer();
    void initialiseRefractionFrameBuffer();

    void bindFrameBuffer(GLuint frameBuffer, int width, int height);

    GLuint createFrameBuffer();
    GLuint createTextureAttachment(int width, int height);
    GLuint createDepthTextureAttachment(int width, int height);
    GLuint createDepthBufferAttachment(int width, int height);

};

