#include "sokobirds.h"

void render() {
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );

    glRotatef(0.4f,0.0f,1.0f,0.0f);    // Rotate The cube around the Y axis
    glRotatef(0.2f,1.0f,1.0f,1.0f);
    glColor3f(0.0f,1.0f,0.0f);

    glBegin( GL_QUADS );
        glVertex2f( -0.5f, -0.5f );
        glVertex2f( 0.5f, -0.5f );
        glVertex2f( 0.5f, 0.5f );
        glVertex2f( -0.5f, 0.5f );
    glEnd();
}
