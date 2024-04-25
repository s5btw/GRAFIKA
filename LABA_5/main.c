#include <windows.h>
#include <gl/gl.h>
#include <math.h>

float vert[] = {1,1,0, 1,-1,0, -1,-1,0, -1,1,0};
float xAlfa = 40;
float zAlfa = 0;
POINTFLOAT pos = {0, 0};
int n = 10;
void DrawArrowHead(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat ax, GLfloat ay, GLfloat az) {
    glPushMatrix();
    glColor3f(r, g, b);
    glTranslatef(x, y, z);

    // Определяем вектор вращения для стрелки на основе направления оси
    GLfloat angle = acos(az / sqrt(ax * ax + ay * ay + az * az)) * 180.0f / M_PI;
    GLfloat rx = -ay;
    GLfloat ry = ax;
    GLfloat rz = 0;

    // Если направление не только по Z (т.е. мы уже не в направлении оси Z),
    // выполняем вращение
    if (ax != 0.0f || ay != 0.0f) {
        glRotatef(angle, rx, ry, rz);
    }

    // Рисуем стрелку
    glBegin(GL_TRIANGLES);
        glVertex3f( 0.0f, -0.05f, 0.0f);
        glVertex3f( 0.1f, 0.0f, 0.0f);
        glVertex3f(-0.1f, 0.0f, 0.0f);
    glEnd();
    glPopMatrix();
}

void ShowWord() {
    // Включаем массив вершин
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &vert);

    // Отрисовываем шахматную доску
    for (int i = -n; i < n; i++) {
        for (int j = -n; j < n; j++) {
            glPushMatrix();
            glTranslatef(i*2, j*2, 0);
            glBegin(GL_QUADS);  // Начинаем рисовать квадраты

            // В зависимости от четности суммы индексов выбираем цвет
            if ((i+j) % 2 == 0) {
                // Темные плитки одним цветом
                glColor3f(0.02f, 0.02f, 0.02f);  // Темный цвет
                glVertex3f(-1.0f, -1.0f, 0);
                glVertex3f(-1.0f,  1.0f, 0);
                glVertex3f( 1.0f,  1.0f, 0);
                glVertex3f( 1.0f, -1.0f, 0);
            } else {
                // Градиент для светлых плиток
                // Нижняя левая вершина
                glColor3f(0.9f, 0.9f, 0.9f);  // Белый цвет
                glVertex3f(-1.0f, -1.0f, 0);
                // Нижняя правая вершина
                glColor3f(0.8f, 0.8f, 0.8f);  // Светло-серый цвет
                glVertex3f(-1.0f,  1.0f, 0);
                // Верхняя правая вершина
                glColor3f(0.7f, 0.7f, 0.7f);  // Еще более светло-серый
                glVertex3f( 1.0f,  1.0f, 0);
                // Верхняя левая вершина
                glColor3f(0.4f, 0.4f, 0.4f);  // Почти серый
                glVertex3f( 1.0f, -1.0f, 0);
            }

            glEnd();  // Заканчиваем рисовать квадрат
            glPopMatrix();
        }
    }

        //Оси
        glLineWidth(5);
        glBegin(GL_LINES);
            //Х
            glColor3f(1,0,0);
            glVertex3f(0, 0, 1);
            glVertex3f(0, 1, 1);

            //Y
            glColor3f(0,1,0);
            glVertex3f(0, 0, 1);
            glVertex3f(1, 0, 1);

            //Z
            glColor3f(0,0,1);
            glVertex3f(0, 0, 1);
            glVertex3f(0, 0, 2);

        glEnd();
         // Добавление стрелочек
    DrawArrowHead(0, 1, 1, 1, 0, 0, 0, 2, -5); // Стрелочка для оси X
    DrawArrowHead(1, 0, 1, 0, 1, 0, 214, 2, 2455); // Стрелочка для оси Y
    DrawArrowHead(0, 0, 2, 0, 0, 1, 0, 1, 0); // Стрелочка для оси Z



    glDisableClientState(GL_VERTEX_ARRAY);
}

void MoveCamera()
{
    if (GetKeyState(VK_UP) < 0) xAlfa = ++xAlfa > 180 ? 180 : xAlfa;
    if (GetKeyState(VK_DOWN) < 0) xAlfa = --xAlfa < 0 ? 0 : xAlfa;
    if (GetKeyState(VK_LEFT) < 0) zAlfa++;
    if (GetKeyState(VK_RIGHT) < 0) zAlfa--;

    float ungol = -zAlfa / 180 * M_PI;
    float speed = 0;
    if (GetKeyState('W') < 0) speed = 0.1;
    if (GetKeyState('S') < 0) speed = -0.1;
    if (GetKeyState('A') < 0) { speed = 0.1; ungol -= M_PI*0.5; }
    if (GetKeyState('D') < 0) { speed = 0.1; ungol += M_PI*0.5; }
    if (speed != 0)
    {
        pos.x += sin(ungol) * speed;
        pos.y += cos(ungol) * speed;
    }

    glRotatef(-xAlfa, 1, 0, 0);
    glRotatef(-zAlfa, 0, 0, 1);
    glTranslatef(-pos.x, -pos.y, -3);
}

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);


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
                          720,
                          720,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    glFrustum(-1,1, -1,1, 2,80);
    glEnable(GL_DEPTH_TEST); /*тест глубины*/

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
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);/*тест глубины*/

            glPushMatrix();
                MoveCamera();
                ShowWord();
            glPopMatrix();

            SwapBuffers(hDC);

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

