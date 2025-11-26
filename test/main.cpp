#include "../header.h"
#include "../FrameBuffer.h"
#include "../Mesh.h"
#include "../Shader.h"
#include "../ModelImporter.h"
#include <stb/stb_image.h>
#include "../Camera.h"
#include "../Scene.h"
#include "UIComponent.h"
#include "../editor/UIContext.h"
#include "../editor/SceneView.h"
#include "../TextHandler.h"
#include "../editor/TabView.h"
#include "../EventHandler.h"
#include <thread>
#include <chrono>
#include <Windows.h>
#include "../Window.h"
#include "Panel.h"
#include "Tab.h"


//using namespace std;


class Point {
    double x, y;
public:
    Point() : x(0), y(0) {};
    Point(double a, double b) : x(a), y(b) {};
    double getX() { return x; }
    double getY() { return y; }
    void setX(double a) { x = a; }
    void setY(double a) { y = a; }
};

Point pos;
Point pos2;
Point lastpos,firstpos;
GLFWwindow* window;
GLFWcursor* cursor;
Scene* scene;
Shader* screenShader;
//UIContext* context;
//TextHandler* textHandler;
int windowwidth, windowheight;
int lastclick = 0;
unsigned int screenvbo, screenvao, screenebo;

//FrameBuffer framebuffer = FrameBuffer();

double deltatime = 0.0f, lastframe = 0.0f;

bool firstmouse = true;
void convertpt(GLFWwindow* window, Point& pos);
void drawRect(int framebuffer1, Point& pos, Point& pos2);



std::ostream& operator<<(std::ostream& stream, glm::vec4& vector) {
    stream << vector.x << ' ' << vector.y << ' ' << vector.z << ' ' << vector.w << std::endl;
    return stream;
}

std::ostream& operator<<(std::ostream& stream, glm::vec3& vector) {
    stream << vector.x << ' ' << vector.y << ' ' << vector.z << std::endl;
    return stream;
}


void init() {

    float vertices[] = {
        1.0f,1.0f,0.0f,1.0f,1.0f,
        1.0f,-1.0f,0.0f,1.0f,0.0f,
        -1.0f,-1.0f,0.0f,0.0f,0.0f,
        -1.0f,1.0f,0.0f,0.0f,1.0f
    };

    unsigned int indices[] = {
        0,1,3,
        1,2,3
    };

    glGenVertexArrays(1, &screenvao);
    glGenBuffers(1, &screenvbo);
    glGenBuffers(1, &screenebo);
    glBindVertexArray(screenvao);
    glBindBuffer(GL_ARRAY_BUFFER, screenvbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screenebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

}


void keypress(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
    scene->getCamera()->moveCamera(window, deltatime);
}

void DrawScreenBuffer(const unsigned int& texture) {
    screenShader->use();
    glBindVertexArray(screenvao);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

int main(void)
{

    /* Initialize the library */

    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1700, 900, "this is a title", NULL, NULL);

    cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwGetFramebufferSize(window, &windowwidth, &windowheight);
    glViewport(0, 0, windowwidth, windowheight);
    glfwSetCursor(window, cursor);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    stbi_set_flip_vertically_on_load(true);
    scene = new Scene(windowwidth, windowheight);
    screenShader = new Shader("resources/shader/screenvertex.glsl", "resources/shader/screenfrag.glsl");

    //textHandler = new TextHandler(windowwidth, windowheight, "resources/font/font (32px).png");
    UIContext::getInstance()->init(windowwidth,windowheight);
    TextHandler::makeInstance(windowwidth,windowheight, "resources/font/font (32px).png");

    //eventHandler = new EventHandler();
    init();
    //glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, 1920, 1080, 0);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    /*glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {

        });*/
    glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
        scene->getCamera()->fov -= yoffset * 5;
    });
    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {

        glfwSwapBuffers(window);
        windowwidth = width; windowheight = height;
        glViewport(0, 0, width, height);
        scene->getCamera()->projection = glm::perspective(glm::radians(scene->getCamera()->fov), (float)width / (float)height, 0.1f, 100.0f);
        scene->resizeSceneBuffer(width, height);
        UIContext::getInstance()->setSize(width, height);
        std::cout << width << ' ' << height << std::endl;
        });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        if (button == GLFW_MOUSE_BUTTON_LEFT && action) {
            int aa = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
            firstpos.setX(xpos); firstpos.setY(ypos);
            if (aa - lastclick >= GetDoubleClickTime()) {
                UIContext::getInstance()->queueEvent("click", glm::vec2(xpos, ypos)); lastclick = aa;
            }
            else UIContext::getInstance()->queueEvent("doubleclick", glm::vec2(xpos, ypos));
        }
        else if (!action) {
            UIContext::getInstance()->queueEvent("release", glm::vec2(xpos, ypos));
            firstmouse = true;
        }
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) != 1) {
            return;
        }
        if (firstmouse) {
            lastpos.setX(xpos); lastpos.setY(ypos);
            firstmouse = false;
        }
        //Point offset(xpos - lastpos.getX(), lastpos.getY() - ypos);

        UIContext::getInstance()->queueEvent("drag", glm::vec2(xpos,ypos), glm::vec2(lastpos.getX(), lastpos.getY()));
        lastpos.setX(xpos); lastpos.setY(ypos);

        /*float sensitivity = 0.2f;
        offset.setX(offset.getX() * sensitivity);
        offset.setY(offset.getY() * sensitivity);
        scene->getCamera()->yaw += offset.getX();
        scene->getCamera()->pitch += offset.getY();
        if (scene->getCamera()->pitch > 89.0f) scene->getCamera()->pitch = 89.0f;
        if (scene->getCamera()->pitch < -89.0f) scene->getCamera()->pitch = -89.0f;
        glm::vec3 direction, direction2;
        direction.x = cos(glm::radians(scene->getCamera()->yaw)) * cos(glm::radians(scene->getCamera()->pitch));
        direction.y = sin(glm::radians(scene->getCamera()->pitch));
        direction.z = sin(glm::radians(scene->getCamera()->yaw)) * cos(glm::radians(scene->getCamera()->pitch));
        direction = glm::normalize(direction);
        direction2.x = cos(glm::radians(scene->getCamera()->yaw)) * cos(glm::radians(0.0f));
        direction2.y = sin(glm::radians(0.0f));
        direction2.z = sin(glm::radians(scene->getCamera()->yaw)) * cos(glm::radians(0.0f));
        direction2 = glm::normalize(direction2);
        scene->getCamera()->camerafront = direction;
        scene->getCamera()->cameradirection = direction2;*/
        });
    glfwSwapInterval(0);


    ModelImporter* importer = new ModelImporter();

    Shader* objectshader = new Shader("resources/shader/vertex.glsl", "resources/shader/frag.glsl");




    /* Loop until the user closes the window */
    Object obj = Object(importer->loadModel("resources/test/cube-tex.obj")), obj2 = Object(importer->loadModel("resources/models/spider.obj"));
    obj2.getModel().scale = glm::vec3(0.01f, 0.01f, 0.01f);
    scene->addObject(obj);
    scene->addObject(obj2);


    int browhy = 0;
    SceneView* sceneview = new SceneView();
    sceneview->addScene(scene);
    TabView* tabview = new TabView(glm::vec2(0, 0), glm::vec2(950, 25));
    //tabview->addChildComponent(new Panel<SceneView>(glm::vec2(tabview->position.x, tabview->position.y + tabview->size.y), glm::vec2(tabview->size.x, 450), sceneview));
    dynamic_cast<Tab*>(tabview->getTab(1))->setViewComponent(sceneview);
    UIContext::getInstance()->add(tabview);
    UIContext::getInstance()->setup();
    int x = 0, y = 0;
    GLsync previousFrameSync = nullptr;
    int fps = TextHandler::getInstance()->addText(10, 10, "");

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltatime = currentFrame - lastframe;
        lastframe = currentFrame;
        TextHandler::getInstance()->editText(10, 10, std::to_string(1/deltatime), fps);
        /*if (previousFrameSync) {
            GLenum res = glClientWaitSync(previousFrameSync, GL_SYNC_FLUSH_COMMANDS_BIT, 1'000'000);
            if (res == GL_TIMEOUT_EXPIRED) {
                glWaitSync(previousFrameSync, 0, GL_TIMEOUT_IGNORED);
            }
            glDeleteSync(previousFrameSync);
        }*/
        std::this_thread::sleep_for(std::chrono::milliseconds(1/60));
        //glfwPollEvents();
        glfwWaitEventsTimeout(0.007);
        UIContext::getInstance()->dispatchEvent();
        UIContext::getInstance()->dispatchUpdate();
        keypress(window);
        scene->getCamera()->projection = glm::perspective(glm::radians(scene->getCamera()->fov), (float)windowwidth / (float)windowheight, 0.1f, 100.0f);
        /* Render here */
        //sceneview->renderScene();
        glDisable(GL_DEPTH_TEST);
        //glClearColor(1.0,1,1,1.0);
        glClearColor(0.09, 0.09, 0.09, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*objectshader->use();
        glBindVertexArray(screenvao);
        glBindTexture(GL_TEXTURE_2D, scene->getSceneTexture());

        glm::mat4 wah = glm::mat4(1.0);
        wah = glm::translate(wah, glm::vec3(0.0, 0.0, 0.0));
        wah = glm::scale(wah, glm::vec3(1.0, 1.0, 0.0));

        glUniformMatrix4fv(glGetUniformLocation(objectshader->getId(), "model"), 1, GL_FALSE, glm::value_ptr(wah));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);*/
        UIContext::getInstance()->DrawComponents();
        TextHandler::getInstance()->draw();

        //context->queueEvent("drag", glm::vec2(x, y), glm::vec2(lastpos.getX(), lastpos.getY()));
        x += 100, y += 100;
        //DrawScreenBuffer(textHandler->getFrameTexture());
        //previousFrameSync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        glFinish();

        /* Poll for and process events */

    }
    glDeleteVertexArrays(1, &screenvao);
    glDeleteBuffers(1, &screenvbo);
    glDeleteBuffers(1, &screenebo);
    glfwDestroyCursor(cursor);
    delete importer;
    UIContext::destroyInstance();
    delete objectshader;
    //delete textHandler;
    delete screenShader;
    glfwTerminate();
    return 0;
}