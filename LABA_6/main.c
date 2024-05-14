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
        glVertex3f(0.0f, -0.05f, 0.0f);
        glVertex3f(0.1f, 0.0f, 0.0f);
        glVertex3f(-0.1f, 0.0f, 0.0f);
    glEnd();
    glPopMatrix();
}

void MoveCamera() {
    if (GetKeyState(VK_UP) < 0) xAlfa = ++xAlfa > 180 ? 180 : xAlfa;
    if (GetKeyState(VK_DOWN) < 0) xAlfa = --xAlfa < 0 ? 0 : xAlfa;
    if (GetKeyState(VK_LEFT) < 0) zAlfa++;
    if (GetKeyState(VK_RIGHT) < 0) zAlfa--;

    float ungol = -zAlfa / 180 * M_PI;
    float speed = 0;
    if (GetKeyState('W') < 0) speed = 0.1;
    if (GetKeyState('S') < 0) speed = -0.1;
    if (GetKeyState('A') < 0) { speed = 0.1; ungol -= M_PI * 0.5; }
    if (GetKeyState('D') < 0) { speed = 0.1; ungol += M_PI * 0.5; }
    if (speed != 0) {
        pos.x += sin(ungol) * speed;
        pos.y += cos(ungol) * speed;
    }

    glRotatef(-xAlfa, 1, 0, 0);
    glRotatef(-zAlfa, 0, 0, 1);
    glTranslatef(-pos.x, -pos.y, -3);
}

void Init_Material()
{
    glEnable(GL_COLOR_MATERIAL);


    glShadeModel(GL_SMOOTH);
    GLfloat material_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat material_diffuse[] = { 0.0f, 1.0f, 1.0f, 1.0f };
    GLfloat material_specular[] = { 1.0f, 1.0f, 1.0f, 32.0f };
    GLfloat material_shininess[] = { 50.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void SetupLighting(float x, float y, float z) {
    GLfloat light_position[] = {x, y, z, 1.0f};
    GLfloat light_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat spot_direction[] = {-x, -y, -z}; // Свет направлен к началу координат

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0f); // Угол освещения
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0f); // Концентрация света
}


void updateLightPosition(float theta, float *x, float *y, float *z) {
    float radius = 5.0f;  // Радиус вращения сферы
    float height = 8.0f;  // Высота над шахматной доской

    *x = radius * cos(theta * M_PI / 180.0f);
    *y = radius * sin(theta * M_PI / 180.0f);
    *z = height;
}

void drawSphere(float radius, int slices, int stacks) {
    for (int i = 0; i <= stacks; ++i) {
        float V = i / (float) stacks;
        float phi = V * M_PI;

        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= slices; ++j) {
            float U = j / (float) slices;
            float theta = U * (M_PI * 2);

            float x = cos(theta) * sin(phi);
            float y = sin(theta) * sin(phi);
            float z = cos(phi);

            glNormal3f(x, y, z);
            glVertex3f(x * radius, y * radius, z * radius);

            x = cos(theta) * sin(phi + M_PI / stacks);
            y = sin(theta) * sin(phi + M_PI / stacks);
            z = cos(phi + M_PI / stacks);

            glNormal3f(x, y, z);
            glVertex3f(x * radius, y * radius, z * radius);
        }
        glEnd();
    }
}

void Draw_Cube(float X, float Y, float Z) {
    GLfloat vertices[] = {
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f
    };

    GLuint indices[] = {
        0, 1, 2, 2, 3, 0,
        1, 5, 6, 6, 2, 1,
        7, 6, 5, 5, 4, 7,
        4, 0, 3, 3, 7, 4,
        4, 5, 1, 1, 0, 4,
        3, 2, 6, 6, 7, 3
    };

    GLfloat normals[] = {
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f,  1.0f,
        0.0f, 0.0f,  1.0f,
        0.0f, 0.0f,  1.0f,
        0.0f, 0.0f,  1.0f,
       -1.0f, 0.0f,  0.0f,
       -1.0f, 0.0f,  0.0f,
       -1.0f, 0.0f,  0.0f,
       -1.0f, 0.0f,  0.0f,
        1.0f, 0.0f,  0.0f,
        1.0f, 0.0f,  0.0f,
        1.0f, 0.0f,  0.0f,
        1.0f, 0.0f,  0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, 1.0f,  0.0f,
        0.0f, 1.0f,  0.0f,
        0.0f, 1.0f,  0.0f,
        0.0f, 1.0f,  0.0f
    };

    GLfloat material_ambient[] = {0.1f, 0.5f, 0.5f, 0.7f};
    GLfloat material_diffuse[] = {0.2f, 0.7f, 0.7f, 0.7f};
    GLfloat material_specular[] = {0.8f, 1.0f, 1.0f, 0.7f};
    GLfloat material_shininess[] = {50.0f};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_shininess);

    glPushMatrix();
    glColor3f(1.0f,0.26565f,0.0f);
        glTranslatef(X, Y, Z);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, normals);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
    glPopMatrix();
}

void ShowWord() {
    // Обновляем позицию и направление света
    static float theta = 0.0f;  // Угол для вращения сферы
    float lightX, lightY, lightZ;
    updateLightPosition(theta, &lightX, &lightY, &lightZ);
    theta += 1.0f;  // Скорость вращения сферы

    // Настройка освещения
    SetupLighting(lightX, lightY, lightZ);

    // Включаем массив вершин
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vert);

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
                glNormal3f(0.0f, 0.0f, 1.0f);  // Нормали для освещения
                glVertex3f(-1.0f, -1.0f, 0);
                glVertex3f(-1.0f,  1.0f, 0);
                glVertex3f( 1.0f,  1.0f, 0);
                glVertex3f( 1.0f, -1.0f, 0);
            } else {
                // Градиент для светлых плиток
                glNormal3f(0.0f, 0.0f, 1.0f);  // Нормали для освещения
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

    // Рисуем фигуру (сферу) из которой исходит свет
    glPushMatrix();
        glColor3f(1.0, 1.0, 0.0); // Желтый цвет для сферы
        glTranslatef(lightX, lightY, lightZ); // Перемещаем сферу в позицию света
        drawSphere(1.0f, 20, 20); // Радиус и сегменты сферы
    glPopMatrix();

    // Рисуем куб
    glPushMatrix();
        glTranslatef(0, 0, 4.0f); // Положение куба (поднято на 4 единицы по Z)
        Draw_Cube(0,0,0); // Вызов функции для рисования куба
    glPopMatrix();

    // Оси
    glLineWidth(5);
    glBegin(GL_LINES);
        // X
        glColor3f(1, 0, 0);
        glVertex3f(0, 0, 1);
        glVertex3f(0, 1, 1);

        // Y
        glColor3f(0, 1, 0);
        glVertex3f(0, 0, 1);
        glVertex3f(1, 0, 1);

        // Z
        glColor3f(0, 0, 1);
        glVertex3f(0, 0, 1);
        glVertex3f(0, 0, 2);
    glEnd();

    // Добавление стрелочек
    DrawArrowHead(0, 1, 1, 1, 0, 0, 0, 2, -5); // Стрелочка для оси X
    DrawArrowHead(1, 0, 1, 0, 1, 0, 214, 2, 2455); // Стрелочка для оси Y
    DrawArrowHead(0, 0, 2, 0, 0, 1, 0, 1, 0); // Стрелочка для оси Z

    glDisableClientState(GL_VERTEX_ARRAY);
}

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
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
    hwnd = CreateWindowEx(0, "GLSample", "OpenGL Sample", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 720, 720, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    glFrustum(-1, 1, -1, 1, 2, 80);
    glEnable(GL_DEPTH_TEST); /*тест глубины*/

    glMatrixMode(GL_PROJECTION); /*матрица проекций*/
    glLoadIdentity();
    glFrustum(-0.1, 0.1, -0.1, 0.1, 0.2, 1000);
    glMatrixMode(GL_MODELVIEW); /*матрица моделей*/
    glLoadIdentity();


    glEnable(GL_LIGHTING); /*разрешение работы с светом*/
    glEnable(GL_LIGHT0); /*включение нулевого источника света*/
    glEnable(GL_COLOR_MATERIAL); /*включает освещение*/
    glEnable(GL_NORMALIZE); /*включает нормализацию вектора*/
    /* program main loop */
    while (!bQuit) {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT) {
                bQuit = TRUE;
            } else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        } else {
            /* OpenGL animation code goes here */

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);/*тест глубины*/

            glPushMatrix();
                MoveCamera();
                ShowWord();
            glPopMatrix();

            SwapBuffers(hDC);

            Sleep(1);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN: {
            switch (wParam) {
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

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC) {
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
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

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC) {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}
