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

    // ���������� ������ �������� ��� ������� �� ������ ����������� ���
    GLfloat angle = acos(az / sqrt(ax * ax + ay * ay + az * az)) * 180.0f / M_PI;
    GLfloat rx = -ay;
    GLfloat ry = ax;
    GLfloat rz = 0;

    // ���� ����������� �� ������ �� Z (�.�. �� ��� �� � ����������� ��� Z),
    // ��������� ��������
    if (ax != 0.0f || ay != 0.0f) {
        glRotatef(angle, rx, ry, rz);
    }

    // ������ �������
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
    GLfloat spot_direction[] = {-x, -y, -z}; // ���� ��������� � ������ ���������

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0f); // ���� ���������
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0f); // ������������ �����
}


void updateLightPosition(float theta, float *x, float *y, float *z) {
    float radius = 5.0f;  // ������ �������� �����
    float height = 8.0f;  // ������ ��� ��������� ������

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

void Draw_Cube() {
    GLfloat vertices[] = {
        // Front face
        -1.0f, -1.0f,  1.0f, // 0
         1.0f, -1.0f,  1.0f, // 1
         1.0f,  1.0f,  1.0f, // 2
        -1.0f,  1.0f,  1.0f, // 3
        // Back face
        -1.0f, -1.0f, -1.0f, // 4
        -1.0f,  1.0f, -1.0f, // 5
         1.0f,  1.0f, -1.0f, // 6
         1.0f, -1.0f, -1.0f, // 7
        // Left face
        -1.0f, -1.0f, -1.0f, // 8
        -1.0f, -1.0f,  1.0f, // 9
        -1.0f,  1.0f,  1.0f, // 10
        -1.0f,  1.0f, -1.0f, // 11
        // Right face
         1.0f, -1.0f, -1.0f, // 12
         1.0f, -1.0f,  1.0f, // 13
         1.0f,  1.0f,  1.0f, // 14
         1.0f,  1.0f, -1.0f, // 15
        // Top face
        -1.0f,  1.0f, -1.0f, // 16
        -1.0f,  1.0f,  1.0f, // 17
         1.0f,  1.0f,  1.0f, // 18
         1.0f,  1.0f, -1.0f, // 19
        // Bottom face
        -1.0f, -1.0f, -1.0f, // 20
         1.0f, -1.0f, -1.0f, // 21
         1.0f, -1.0f,  1.0f, // 22
        -1.0f, -1.0f,  1.0f  // 23
    };

    GLuint indices[] = {
        0,  1,  2,  2,  3,  0,   // Front face
        4,  5,  6,  6,  7,  4,   // Back face
        8,  9,  10, 10, 11, 8,   // Left face
        12, 13, 14, 14, 15, 12,  // Right face
        16, 17, 18, 18, 19, 16,  // Top face
        20, 21, 22, 22, 23, 20   // Bottom face
    };

    GLfloat normals[] = {
        // Front face
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        // Back face
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        // Left face
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        // Right face
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        // Top face
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        // Bottom face
        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, normals);

    glColor3f(0.98f, 0.250f, 0.30f); // ��������� ����
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void drawTruncatedPyramid(float baseRadius, float topRadius, float height, int sides) {
    float angleStep = 2.0f * M_PI / sides;

    // ������ ���������
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    for (int i = 0; i <= sides; ++i) {
        float angle = i * angleStep;
        glVertex3f(baseRadius * cos(angle), baseRadius * sin(angle), 0.0f);
    }
    glEnd();

    // ������ ������� �����
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, height);
    for (int i = 0; i <= sides; ++i) {
        float angle = i * angleStep;
        glVertex3f(topRadius * cos(angle), topRadius * sin(angle), height);
    }
    glEnd();

    // ������ ������� �����
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= sides; ++i) {
        float angle = i * angleStep;
        float x = cos(angle);
        float y = sin(angle);
        glNormal3f(x, y, 0.0f);
        glVertex3f(baseRadius * x, baseRadius * y, 0.0f);
        glVertex3f(topRadius * x, topRadius * y, height);
    }
    glEnd();
}




void ShowWord() {
//    updateSpotlightDirection(); // ��������� ����������� �����

    // �������� ������ ������
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vert);

    // ������������ ��������� �����
    for (int i = -n; i < n; i++) {
        for (int j = -n; j < n; j++) {
            glPushMatrix();
            glTranslatef(i * 2, j * 2, 0);
            glBegin(GL_QUADS);  // �������� �������� ��������

            // � ����������� �� �������� ����� �������� �������� ����
            if ((i + j) % 2 == 0) {
                // ������ ������ ����� ������
                glColor3f(0.02f, 0.02f, 0.02f);  // ������ ����
                glVertex3f(-1.0f, -1.0f, 0);
                glVertex3f(-1.0f, 1.0f, 0);
                glVertex3f(1.0f, 1.0f, 0);
                glVertex3f(1.0f, -1.0f, 0);
            } else {
                // �������� ��� ������� ������
                // ������ ����� �������
                glColor3f(0.9f, 0.9f, 0.9f);  // ����� ����
                glVertex3f(-1.0f, -1.0f, 0);
                // ������ ������ �������
                glColor3f(0.8f, 0.8f, 0.8f);  // ������-����� ����
                glVertex3f(-1.0f, 1.0f, 0);
                // ������� ������ �������
                glColor3f(0.7f, 0.7f, 0.7f);  // ��� ����� ������-�����
                glVertex3f(1.0f, 1.0f, 0);
                // ������� ����� �������
                glColor3f(0.4f, 0.4f, 0.4f);  // ����� �����
                glVertex3f(1.0f, -1.0f, 0);
            }

            glEnd();  // ����������� �������� �������
            glPopMatrix();
        }
    }

    // ������� ��������� �����
    static float theta = 0.0f;  // ���� ��� �������� �����
    float lightX, lightY, lightZ;
    updateLightPosition(theta, &lightX, &lightY, &lightZ);
    theta += 1.0f;  // �������� �������� �����

    // ��������� ���������
    SetupLighting(lightX * 2, lightY * 2, lightZ);  // ����������� ������ ��������

    // ������ ������ (�����) �� ������� ������� ����
    glPushMatrix();
        glColor3f(1.0, 1.0, 0.0); // ������ ���� ��� �����
        glTranslatef(lightX * 2, lightY * 2, lightZ); // ���������� ����� � ������� �����
        drawSphere(1.0f, 20, 20); // ������ � �������� �����
    glPopMatrix();

     // ������ 10 ��������� ������� � ���������� ������������
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (int i = 0; i < 10; ++i) {
        float angle = i * 2.0 * M_PI / 10;
        float prismX = 5.0f * cos(angle); // ����������� ������
        float prismY = 5.0f * sin(angle); // ����������� ������
        float alpha = 1.0f - (i / 10.0f);

        glPushMatrix();
            glColor4f(0.0f, 0.0f, 1.0f, alpha); // ���� � ������������ ��������� �������
            glTranslatef(prismX, prismY, 0.0f); // ������� ����� �� ��������� �����
            drawTruncatedPyramid(1.0f, 0.5f, 1.0f, 18); // ������ ���������, �������� ��������� � ������ ������
        glPopMatrix();
    }
    glDisable(GL_BLEND);

    // ������ ���
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, 3.9f); // ��������� ����
        Draw_Cube(0.0f, 0.0f, 0.0f); // ����� ������� ��� ��������� ����
    glPopMatrix();

    // ���
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

    // ���������� ���������
    DrawArrowHead(0, 1, 1, 1, 0, 0, 0, 2, -5); // ��������� ��� ��� X
    DrawArrowHead(1, 0, 1, 0, 1, 0, 214, 2, 2455); // ��������� ��� ��� Y
    DrawArrowHead(0, 0, 2, 0, 0, 1, 0, 1, 0); // ��������� ��� ��� Z

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
    glEnable(GL_DEPTH_TEST); /*���� �������*/

    glMatrixMode(GL_PROJECTION); /*������� ��������*/
    glLoadIdentity();
    glFrustum(-0.1, 0.1, -0.1, 0.1, 0.2, 1000);
    glMatrixMode(GL_MODELVIEW); /*������� �������*/
    glLoadIdentity();


    glEnable(GL_LIGHTING); /*���������� ������ � ������*/
    glEnable(GL_LIGHT0); /*��������� �������� ��������� �����*/
    glEnable(GL_COLOR_MATERIAL); /*�������� ���������*/
    glEnable(GL_NORMALIZE); /*�������� ������������ �������*/
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
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);/*���� �������*/

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
