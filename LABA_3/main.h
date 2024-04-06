#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <malloc.h>
#include "stb-master/stb_easy_font.h"
#include "mEnu.h"
#include "tExtur.h"

using namespace std;

GLuint texture;
GLuint sprite;
GLuint background;

int width = 1080;
int height = 720;
float gravity = 0.45;
float speed = 5;
float currentframe=0;
float chSiX;
float chSiY;
bool directional;
bool gameState=0;
int n=0;
int m=0;

typedef struct
{
    float left, right, top, buttom, textX, textY;
} sritCoord;

typedef struct
{
    float x, y, dx, dy;
} Hero;

Hero pers;
sritCoord vrtcoord;
sritCoord BackGroundCoord;

void Hero_Init(Hero *obj, float x1, float y1, float dx1, float dy1)
{
    obj->x=x1;
    obj->y=y1;
    obj->dx=dx1;
    obj->dy=dy1;
}

void Reflect (float *da, float *a, BOOL cond, float wall)
{
    if (!cond) return;
    *da*=-0;
    *a=wall;
}

void Hero_Move(Hero *obj)
{
    obj->y+=obj->dy;
    obj->x+=obj->dx;
    Reflect(&obj->dy, &obj->y, obj->y<0,0);
    Reflect(&obj->dx,&obj->x, obj->x<0, 0);
    Reflect(&obj->dx,&obj->x, obj->x>width-chSiX-0, width-chSiX-0);
    obj->dy-=gravity;
    if (GetKeyState(VK_LEFT)<0 && gameState==1)
    {
        currentframe+=0.15;
        obj->dx-=speed;
        obj->x+=obj->dx;
        obj->dx=0;
        if (currentframe>8) currentframe-=8;
        directional=1;

    }

    if (GetKeyState(VK_RIGHT)<0 && gameState==1)
    {
        currentframe+=0.15;
        obj->dx+=speed;
        obj->x+=obj->dx;
        obj->dx=0;
        if (currentframe>8) currentframe-=8;
        directional=0;


    }
    if (GetKeyState(VK_UP)<0 && (obj->y<410) && gameState==1)
    {
        obj->dy =speed*1.2;
        obj->y+=obj->dy;
        if (currentframe>8) currentframe-=8;
    }
}



void Sprite_animation(GLuint texture, int n, Hero *obj)
{

    static float svertix[]= {0,0, 80,0, 80,80, 0,80};
    static float TexCord[]= {0,0, 0.12,0, 0.12,1, 0,1};
    static float line[]= {0,0, 80,0, 80,80, 0,80};

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.7);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2,GL_FLOAT,0,svertix);
    glTexCoordPointer(2,GL_FLOAT,0,TexCord);

    static float spriteXsize=800;
    static float spriteYsize=100;
    static float charsizey=34;
    static float charsizex=100;
    vrtcoord.left=(charsizex*n)/spriteXsize;
    vrtcoord.right=vrtcoord.left+(charsizex/spriteXsize);
    vrtcoord.top=(charsizey*0)/spriteYsize;
    vrtcoord.buttom=vrtcoord.top+(charsizey/spriteYsize);

    TexCord[1] = TexCord[3] = vrtcoord.buttom;
    TexCord[5] = TexCord[7] = vrtcoord.top;

    if (GetKeyState(VK_LEFT)<0 && gameState==1)
    {
        TexCord[2] = TexCord[4] = vrtcoord.left;
        TexCord[0] = TexCord[6] = vrtcoord.right;
    }
    if (GetKeyState(VK_RIGHT)<0 && gameState==1)
    {
        TexCord[0] = TexCord[6] = vrtcoord.left;
        TexCord[2] = TexCord[4] = vrtcoord.right;
    }

    if (GetKeyState(VK_UP)<0 && gameState==1)
    {
        for(int i {1}; i < 8; i++)
    {
        m=i;
        vrtcoord.left=(charsizex*m)/spriteXsize;
        vrtcoord.right=vrtcoord.left+(charsizex/spriteXsize);
        vrtcoord.top=(charsizey*1)/spriteYsize;
        vrtcoord.buttom=vrtcoord.top+(charsizey/spriteYsize);

        TexCord[1] = TexCord[3] = vrtcoord.buttom;
        TexCord[5] = TexCord[7] = vrtcoord.top;
        Sleep(0.1);
        }
    }


    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDisable(GL_ALPHA_TEST);

    //glLineWidth(0);
    //glVertexPointer(2,GL_FLOAT,0,line);
    //glDrawArrays(GL_LINE_LOOP, 0,4);



    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glPopMatrix();

    chSiX=charsizex;
    chSiY=charsizey;

}



void Main_Init()
{
    Hero_Init(&pers, width/2.0,height/2.0,0,0);
    Load_Texture("Anim.png", &sprite, GL_CLAMP, GL_CLAMP, GL_NEAREST);
    Load_Texture("backgraund.png", &background, GL_REPEAT, GL_CLAMP, GL_NEAREST);
}

void Menu_Init(){
    Menu_AddButton("Play", 10.0,10.0,200.0,60.0,4);
    Menu_AddButton("Pause", 10.0,80.0,200.0,60.0,4);
    Menu_AddButton("Quit", 10.0,150.0,200.0,60.0,4);
}




void Show_Background(GLuint texture)
{
    static float svertix[]= {-1080,0, -1080,700, 1080*2,700, 1080*2,0};
    static float TexCord[]= {-1,1, -1,0, 2,0, 2,1 };

    glClearColor(0,0,0,0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2,GL_FLOAT,0,svertix);
    glTexCoordPointer(2,GL_FLOAT,0,TexCord);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


void MouseDown()
{
    int buttonId = Menu_MouseDown();
    if(buttonId<0) return;
    char *name = Menu_GetButtonName(buttonId);
    printf("%s\n",name, &buttonId);
    switch (buttonId)
    {
    case 0:
        if (gameState==0) gameState=1;
        break;
    case 1:
        if (gameState==1) gameState=0;
        break;
    case 2:
        PostQuitMessage(0);
        break;

    }
}


void Hero_Show(Hero *obj)
{
    glPushMatrix();
    glTranslatef(obj->x,obj->y,0);
    Sprite_animation(sprite, currentframe, &pers);
    glPopMatrix();

}

#endif // MAIN_H_INCLUDED
