#include <windows.h>
#include <gl/gl.h>
#include "stb-master/stb_easy_font.h"

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

void show_button(char *name, float x, float y, float width, float height, float texscale){

    float buffer[1000];
    int num_quads;
    float vert[8];

        vert[0] = vert[6] = x;
        vert[2] = vert[4] = x + width;
        vert[1] = vert[3] = y;
        vert[5] = vert[7] = y + height;
        num_quads = stb_easy_font_print(0, 0, name, 0, buffer, sizeof(buffer));
        float textcordx = x + (width - stb_easy_font_width(name) * texscale)/2;
        float textcordy = y + (height - stb_easy_font_height(name) * texscale)/2;

        glVertexPointer(2, GL_FLOAT, 0, vert);
        glEnableClientState(GL_VERTEX_ARRAY);
        glColor3f(0.9, 0.1, 0.1);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);

        glPushMatrix();
        glTranslatef(textcordx, textcordy, 0);
        glScalef(texscale, texscale, 1);

        glVertexPointer(2, GL_FLOAT, 16, buffer);
        glEnableClientState(GL_VERTEX_ARRAY);
        glColor3f(0, 0, 0);
        glDrawArrays(GL_QUADS, 0, num_quads * 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        glPushMatrix();
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          1000,
                          600,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    RECT rct; //создание переменной с координатами прямоуголника
    GetClientRect(hwnd, &rct); //получение текущих координат окна
    glOrtho(0, rct.right, 0, rct.bottom, 1, -1); //выставляем их как координаты окна

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glPushMatrix();

            glLoadIdentity();
            glOrtho(0, rct.right, rct.bottom, 0, 1, -1);
            show_button("animation 1", 20, 20, 200, 60, 3);

            glLoadIdentity();
            glOrtho(0, rct.right, rct.bottom, 0, 1, -1);
            show_button("animation 2", 20, 100, 200, 60, 3);

            glLoadIdentity();
            glOrtho(0, rct.right, rct.bottom, 0, 1, -1);
            show_button("animation 3", 20, 180, 200, 60, 3);

            glLoadIdentity();
            glOrtho(0, rct.right, rct.bottom, 0, 1, -1);
            show_button("exit", 20, 260, 200, 60, 3);

            glPushMatrix();
            /*glTranslatef(516, 516, 0);
            glRotatef(theta, 0.0f, 0.0f, 1.0f);

            glBegin(GL_TRIANGLES);

                glColor3f(1.0f, 0.0f, 0.0f);   glVertex2f(-250, 250);
                glColor3f(0.0f, 1.0f, 0.0f);   glVertex2f(-250, -250);
                glColor3f(0.0f, 0.0f, 1.0f);   glVertex2f(250, -250);

            glEnd();

            glPopMatrix();*/

            SwapBuffers(hDC);

            theta += 1.0f;
            Sleep (1);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}
