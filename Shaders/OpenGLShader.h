#ifndef OPENGLSHADER_H
#define OPENGLSHADER_H

#include <QtOpenGL>
#include "Shader.h"

class OpenGLShader : public Shader {

public:
    OpenGLShader();
    virtual ~OpenGLShader(void);

    /* shader interface */
    virtual bool initializeShader()=0;  // things thats need to be done once
    virtual void render()=0;
    virtual bool postRendertidyUp()=0;  // restore everything to pre-init state
    virtual QLayout *getParameterLayout()=0;  // get layout containing parameter controls

    /* non pure virtual interface functions */
    virtual void setRenderSize(unsigned int width, unsigned int height);



    std::string &functionName(void);


protected:

    std::string _functionName;
    GLuint _textureWidth;
    GLuint _textureHeight;
    GLuint _texture;

    QGLWidget *_glw;

void checkFBOStatus();
void printShaderInfoLog(GLuint obj);
void printProgramInfoLog(GLuint obj);

void createRenderTexture (GLuint texture, int width, int height, unsigned char *buffer);
void createRenderTexture (GLuint texture, int width, int height, void *buffer);
void createRenderTexture (GLuint texture, int width, int height);

private:
    QGLContext *_ctx;

};

#endif // OPENGLSHADER_H
