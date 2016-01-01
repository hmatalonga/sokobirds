#include "sokobirds.h"

void renderGame(Game game, Scene scene, GLuint *textureContent) {
    //Clear The Screen And The Depth Buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!game.MenuOpened) {
        glPushMatrix();
        glLoadIdentity(); //Reset The View
        drawSkyBox(textureContent);
        glPopMatrix();
        drawScene(game, scene, textureContent);
    } else
        drawMainMenu(textureContent);
}

void drawMainMenu(GLuint *textureContent) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureContent[TEXTURE_MAINMENU]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(-1.0f, 1.0f);
        glTexCoord2f(0, 1); glVertex2f(-1.0f, -1.0f);
        glTexCoord2f(1, 1); glVertex2f(1.0f, -1.0f);
        glTexCoord2f(1, 0); glVertex2f(1.0f, 1.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glTranslatef(0, 0, 1.0f);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void drawSkyBox(GLuint *textureContent) {
    GLfloat position = 1.0f; //Posição relativa do SkyBox

    // Store the current matrix
    glPushMatrix();

    // Reset and transform the matrix.
    glLoadIdentity();

    //Posiciona a Camera
    gluLookAt(
            1, 1, 0,
            0, 0, 0,
            0, 1, 0);

    // Enable/Disable features
    glPushAttrib(GL_ENABLE_BIT); //push and pop the server attribute stack
    glEnable(GL_TEXTURE_2D);  //Activa Texture Mapping

    // If enabled, do depth comparisons and update the depth buffer.
    // the depth buffer is not updated if the depth test is disabled.
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    //If enabled, blend the computed fragment color values with the values in the color buffers.
    glDisable(GL_BLEND);

    // Just in case we set all vertices to white.
    glColor4f(1, 1, 1, 1);

    // Render the front quad
    glBindTexture(GL_TEXTURE_2D, textureContent[TEXTURE_SKYBOX_SIDE]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(position, -position, -position);
    glTexCoord2f(1, 0);
    glVertex3f(-position, -position, -position);
    glTexCoord2f(1, 1);
    glVertex3f(-position, position, -position);
    glTexCoord2f(0, 1);
    glVertex3f(position, position, -position);
    glEnd();

    // Render the left quad
    glBindTexture(GL_TEXTURE_2D, textureContent[TEXTURE_SKYBOX_SIDE]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(position, -position, position);
    glTexCoord2f(1, 0);
    glVertex3f(position, -position, -position);
    glTexCoord2f(1, 1);
    glVertex3f(position, position, -position);
    glTexCoord2f(0, 1);
    glVertex3f(position, position, position);
    glEnd();

    // Render the back quad
    glBindTexture(GL_TEXTURE_2D, textureContent[TEXTURE_SKYBOX_SIDE]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(-position, -position, position);
    glTexCoord2f(1, 0);
    glVertex3f(position, -position, position);
    glTexCoord2f(1, 1);
    glVertex3f(position, position, position);
    glTexCoord2f(0, 1);
    glVertex3f(-position, position, position);

    glEnd();

    // Render the right quad
    glBindTexture(GL_TEXTURE_2D, textureContent[TEXTURE_SKYBOX_SIDE]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(-position, -position, -position);
    glTexCoord2f(1, 0);
    glVertex3f(-position, -position, position);
    glTexCoord2f(1, 1);
    glVertex3f(-position, position, position);
    glTexCoord2f(0, 1);
    glVertex3f(-position, position, -position);
    glEnd();

    // Render the top quad
    glBindTexture(GL_TEXTURE_2D, textureContent[TEXTURE_SKYBOX_TOP]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1);
    glVertex3f(-position, position, -position);
    glTexCoord2f(0, 0);
    glVertex3f(-position, position, position);
    glTexCoord2f(1, 0);
    glVertex3f(position, position, position);
    glTexCoord2f(1, 1);
    glVertex3f(position, position, -position);
    glEnd();

    // Render the bottom quad
    glBindTexture(GL_TEXTURE_2D, textureContent[TEXTURE_SKYBOX_BOTTOM]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(-position, -position, -position);
    glTexCoord2f(0, 1);
    glVertex3f(-position, -position, position);
    glTexCoord2f(1, 1);
    glVertex3f(position, -position, position);
    glTexCoord2f(1, 0);
    glVertex3f(position, -position, -position);
    glEnd();

    // Restore enable bits and matrix
    glPopAttrib();
    glPopMatrix();
}

void drawScene(Game game, Scene scene, GLuint *textureContent) {
    //Desenhar Cena do Jogo
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(scene.x, scene.y, scene.z); // move z units out from the screen. e coordenadas(0, 0)

    //Muda camera da Cena
    glTranslatef(-8.0f, 8.0f, -2.0f); //Centra plataforma do Jogo
    glRotatef(15.0f, 1.0f, 0.0f, 0.0f); // Rotate On The X Axis

    int i = 0, j = 0, valid = 0, ct = 0, lt = 0;

        for (i = 0; i < WORLD_MAX_WIDTH; i++) {
            for (j = 0; j < WORLD_MAX_HEIGHT; j++) {
                if (game.Map[i][j] == CWALL) {
                    drawObject(textureContent, "wall");
                    valid = 1;
                } else if (game.Map[i][j] == CFLOOR) {
                    glTranslatef(0.0f, 0.0f, -2.0f);
                    drawObject(textureContent, "floor");
                    glTranslatef(0.0f, 0.0f, 2.0f);
                    valid = 1;
                } else if (game.Map[i][j] == CBOX) {
                    drawObject(textureContent, "box");
                    valid = 1;
                } else if (game.Map[i][j] == CPLAYER) {
                    drawObject(textureContent, "player");
                    valid = 1;
                } else if (game.Map[i][j] == CHOLE) {
                    glTranslatef(0.0f, 0.0f, -2.0f);
                    drawObject(textureContent, "hole");
                    glTranslatef(0.0f, 0.0f, 2.0f);
                    valid = 1;
                } else if (game.Map[i][j] == CDONE) {
                    drawObject(textureContent, "box");
                    valid = 1;
                } else {
                    valid = 0;
                    break;
                }
                if (valid) {
                    glTranslatef(2.0f, 0.0f, 0.0f);
                    ct++;
                }
            }
            glTranslatef(-ct * 2.0f, 0.0f, 0.0f);
            glTranslatef(0.0f, -2.0f, 0.0f);
            ct = 0;
            lt++;
        }
        glTranslatef(0.0f, lt * 2.0f, 0.0f);

    glPopMatrix();
}

void drawObject(GLuint *textureContent, char *objName) {
    if (strcmp(objName, "player") == 0) {
        glEnable(GL_TEXTURE_2D); // Enable texture mapping.
        glBindTexture(GL_TEXTURE_2D, textureContent[TEXTURE_PLAYER_FRONT]); // choose the texture to use.
        glBegin(GL_QUADS); // begin drawing a cube

        // Front Face (note that the texture's corners have to match the quad's corners)
        glNormal3f(0.0f, 0.0f, 1.0f); // front face points out of the screen on z.
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad

        glEnd();
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureContent[TEXTURE_PLAYER_OTHER]); // choose the texture to use.
        glBegin(GL_QUADS);

        // Back Face
        glNormal3f(0.0f, 0.0f, -1.0f); // back face points into the screen on z.
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad

        // Top Face
        glNormal3f(0.0f, 1.0f, 0.0f); // top face points up on y.
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0f, 1.0f, 1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad

        // Bottom Face
        glNormal3f(0.0f, -1.0f, 0.0f); // bottom face points down on y.
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad

        // Right face
        glNormal3f(1.0f, 0.0f, 0.0f); // right face points right on x.
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad

        // Left Face
        glNormal3f(-1.0f, 0.0f, 0.0f); // left face points left on x.
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad

        glEnd(); // done with the polygon.
        glDisable(GL_TEXTURE_2D);
    }
    else if(strcmp(objName, "box") == 0) {
        glEnable(GL_TEXTURE_2D); // Enable texture mapping.
        glBindTexture(GL_TEXTURE_2D, textureContent[TEXTURE_BOX]); // choose the texture to use.
        glBegin(GL_QUADS); // begin drawing a cube

        // Front Face (note that the texture's corners have to match the quad's corners)
        glNormal3f(0.0f, 0.0f, 1.0f); // front face points out of the screen on z.
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad

        // Back Face
        glNormal3f(0.0f, 0.0f, -1.0f); // back face points into the screen on z.
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad

        // Top Face
        glNormal3f(0.0f, 1.0f, 0.0f); // top face points up on y.
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0f, 1.0f, 1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad

        // Bottom Face
        glNormal3f(0.0f, -1.0f, 0.0f); // bottom face points down on y.
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad

        // Right face
        glNormal3f(1.0f, 0.0f, 0.0f); // right face points right on x.
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad

        // Left Face
        glNormal3f(-1.0f, 0.0f, 0.0f); // left face points left on x.
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad

        glEnd(); // done with the polygon.
        glDisable(GL_TEXTURE_2D);
    }
    else if(strcmp(objName, "floor") == 0) {
        glEnable(GL_TEXTURE_2D); // Enable texture mapping.
        glBindTexture(GL_TEXTURE_2D, textureContent[TEXTURE_FLOOR]); // choose the texture to use.
        glBegin(GL_QUADS); // begin drawing a cube

        // Front Face (note that the texture's corners have to match the quad's corners)
        glNormal3f(0.0f, 0.0f, 1.0f); // front face points out of the screen on z.
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad

        glEnd(); // done with the polygon.
        glDisable(GL_TEXTURE_2D);
    }
    else if (strcmp(objName, "wall") == 0) {
        glEnable(GL_TEXTURE_2D); // Enable texture mapping.
        glBindTexture(GL_TEXTURE_2D, textureContent[TEXTURE_WALL]); // choose the texture to use.
        glBegin(GL_QUADS); // begin drawing a cube

        // Front Face (note that the texture's corners have to match the quad's corners)
        glNormal3f(0.0f, 0.0f, 1.0f); // front face points out of the screen on z.
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad

        // Back Face
        glNormal3f(0.0f, 0.0f, -1.0f); // back face points into the screen on z.
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad

        // Top Face
        glNormal3f(0.0f, 1.0f, 0.0f); // top face points up on y.
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0f, 1.0f, 1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad

        // Bottom Face
        glNormal3f(0.0f, -1.0f, 0.0f); // bottom face points down on y.
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad

        // Right face
        glNormal3f(1.0f, 0.0f, 0.0f); // right face points right on x.
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, -1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad

        // Left Face
        glNormal3f(-1.0f, 0.0f, 0.0f); // left face points left on x.
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f); // Top Left Of The Texture and Quad

        glEnd(); // done with the polygon.
        glDisable(GL_TEXTURE_2D);
    }
    else if (strcmp(objName, "hole") == 0) {
        glEnable(GL_TEXTURE_2D); // Enable texture mapping.
        glBindTexture(GL_TEXTURE_2D, textureContent[TEXTURE_HOLE]); // choose the texture to use.
        glBegin(GL_QUADS); // begin drawing a cube

        // Front Face (note that the texture's corners have to match the quad's corners)
        glNormal3f(0.0f, 0.0f, 1.0f); // front face points out of the screen on z.
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 1.0f); // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f); // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f); // Top Left Of The Texture and Quad

        glEnd(); // done with the polygon.
        glDisable(GL_TEXTURE_2D);
    }
}