#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include"shader_s.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include<algorithm>
#include <time.h>
#include <unistd.h> // <windows.h> 대체
#include"map.h"
#include"rayc.h"
#include"bad_apple_data.h"
#include<string>

float renderTime;
Shader ourShader;
bool isChange = true;
bool isPlay = false;

int visBox = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    float min = (width < height) ? width : height;
    glViewport(0, 0, min, min);
    ourShader.setVec2("u_resolution", glm::vec2(min, min));
}

bool isColide(glm::vec3 move){
    glm::vec3 position = player + move;
    if(map[(int)floor(position.z)][mapSizeY - (int)floor(position.y) - 1][(int)floor(position.x)] == 1){
        return true;
    }
    return false;
}

void processInput(GLFWwindow *window)
{
    glm::vec3 Dxy = glm::vec3(cos(PI/180*angle.x), sin(PI/180*angle.x),0);
    glm::vec3 Dz = glm::vec3(0,0,1);
    float moveDst = playerSpeed * ((float)glfwGetTime() - renderTime);
    glm::vec3 move;

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS){
        isPlay = true;
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS){
        isPlay = false;
    }

    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
        ourShader.use();
        ourShader.setInt("visBox", 1);
    }
    if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){
        ourShader.use();
        ourShader.setInt("visBox", 2);
    }
    if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS){
        ourShader.use();
        ourShader.setInt("visBox", 3);
    }
    if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS){
        ourShader.use();
        ourShader.setInt("visBox", 4);
    }
    if(glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS){
        ourShader.use();
        ourShader.setInt("visBox", 5);
    }
    if(glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS){
        ourShader.use();
        ourShader.setInt("visBox", 6);
    }
    if(glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS){
        ourShader.use();
        ourShader.setInt("visBox", 7);
    }
    if(glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS){
        ourShader.use();
        ourShader.setInt("visBox", 8);
    }
    if(glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS){
        ourShader.use();
        ourShader.setInt("visBox", 9);
    }
    if(glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS){
        ourShader.use();
        ourShader.setInt("visBox", 0);
    }
    if(glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS){
        ourShader.use();
        ourShader.setInt("visBox", -1);
    }
    
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        move = scaleVector(Dxy, moveDst);

        if(!isColide(move)){
            player = player + move;
        }
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        move = scaleVector(Dxy, -moveDst);
        
        if(!isColide(move)){
            player = player + move;
        }
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        move = scaleVector(glm::vec3(-Dxy.y, Dxy.x, 0), moveDst);
        
        if(!isColide(move)){
            player = player + move;
        }
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        move = scaleVector(glm::vec3(-Dxy.y, Dxy.x, 0), -moveDst);
        
        if(!isColide(move)){
            player = player + move;
        }
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        move = scaleVector(Dz, moveDst);
        
        if(!isColide(move)){
            player = player + move;
        }
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        move = scaleVector(Dz, -moveDst);
        
        if(!isColide(move)){
            player = player + move;
        }
    }



    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        angle.y += playerThetaSpeed * ((float)glfwGetTime() - renderTime);
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        angle.y -= playerThetaSpeed * ((float)glfwGetTime() - renderTime);
    }

    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        angle.x -= playerThetaSpeed * ((float)glfwGetTime() - renderTime);
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        angle.x += playerThetaSpeed * ((float)glfwGetTime() - renderTime);
    }
}

float screenVertices[] = {
    1.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
};

unsigned int indices[] = {
    0,1,2,
    1,2,3
};


void setUniform(){
    glm::vec3 D = glm::vec3(cos(PI/180*angle.y) * cos(PI/180*angle.x), cos(PI/180*angle.y) * sin(PI/180*angle.x),sin(PI/180*angle.y));
    glm::vec3 Dnr = glm::vec3(sin(PI/180*angle.x), -cos(PI/180*angle.x), 0);// Srow를 위한 D의 평면직교벡터. 노트에 있는 Dㅗ
    glm::vec3 Dnc = glm::vec3(D.z*Dnr.y - D.y*Dnr.z, D.x*Dnr.z - D.z*Dnr.x, D.y*Dnr.x - D.x*Dnr.y);
    glm::vec3 Sr = scaleVector(Dnr, 1.0f/(tan(PI/180 * (1.0f/2)*povHorizontal)));
    glm::vec3 Sc = scaleVector(Dnc, 1.0f/(tan(PI/180 * (1.0f/2)*povVertical)));

    ourShader.use();
    ourShader.setVec3("player", player);
    ourShader.setVec3("D", D);
    ourShader.setVec3("Sr", Sr);
    ourShader.setVec3("Sc", Sc);
}

void changeScreen(int frame){
    for(int i=0;i<colPixelCount;i++){
        for(int j=0;j<rowPixelCount;j++){
            map[screenlayer][i][j] = badApplePixelData[frame][i][j];
        }
    }
    walls.clear();
    wallCoordinate();
    glUniform3fv(glGetUniformLocation(ourShader.ID, "wallsCoord"), walls.size(), glm::value_ptr(walls[0])); 
    ourShader.setInt("wallCount", walls.size());
    printf("done\n");
}

void animation(float time, int* currentFrameAdd){
    int frame = (int)(time*fps);
    // printf("%d\n", frame);
    if(frame != *currentFrameAdd){
        changeScreen(frame%frameCount);
        // printf("%d\n", *currentFrameAdd);
    }
    *currentFrameAdd = frame;
}

void setBoxUniform(int n){
    Box box = boxes[n];
    string location;
    string head = "boxes[";
    string tail = "].";
    string params[6] = {"areaP1", "areaP2", "childIdx1", "childIdx2", "cubeCount", "cubes"};
    string idx = to_string(n);
    string str3 = "]";
    ourShader.use();
    ourShader.setVec3(head + idx + tail + params[0], box.areaP1);
    ourShader.setVec3(head + idx + tail + params[1], box.areaP2);
    ourShader.setInt(head + idx + tail + params[2], box.childIdx1);
    ourShader.setInt(head + idx + tail + params[3], box.childIdx2);

    if(box.childIdx1 == 0){
        ourShader.setInt(head + idx + tail + params[4], box.cubes.size());
        glUniform3fv(glGetUniformLocation(ourShader.ID, (head + idx + tail + params[5]).c_str()), 
        box.cubes.size(), glm::value_ptr(box.cubes[0]));
    }
}

void setBoxesUniform(){
    ourShader.use();
    ourShader.setInt("visBox", visBox);
    for(int i=0;i<boxes.size();i++){
        setBoxUniform(i);
    }
}

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(600, 600, "raycasting3D", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  
    // 어디서 해도 상관없는데 glfwMakeContextCurrent후에 해야함. 그리고 opengl함수 쓰기 전에.
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0,0,600,600);
    glEnable(GL_DEPTH_TEST);
    ourShader.setShader("shader.vs", "distance_shader.frag");

    // wall Data uniform 보내주기
    changeScreen(30);
    wallCoordinate();
    root();
    setBoxesUniform();
    ourShader.use();
    glUniform3fv(glGetUniformLocation(ourShader.ID, "wallsCoord"), walls.size(), glm::value_ptr(walls[0])); 
    ourShader.setInt("wallCount", walls.size());
    ourShader.setFloat("wallArea", 0.5);
    ourShader.setFloat("maxSight", maxSightRange);
    ourShader.setVec2("u_resolution", glm::vec2(600,600));


    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //스크린을 그리기 위한 버퍼 설정
    unsigned int EBOS;
    glGenBuffers(1, &EBOS);
    unsigned int VBOS;
    glGenBuffers(1, &VBOS);
    unsigned int VAOS;
    glGenVertexArrays(1, &VAOS);  
    glBindVertexArray(VAOS);

    glBindBuffer(GL_ARRAY_BUFFER, VBOS);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOS);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);  
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);

    int currentFrame = -1;

    while(!glfwWindowShouldClose(window))
    {
        
        processInput(window);
        // animation((float)glfwGetTime(), &currentFrame);
        printf("%f\n", (float)glfwGetTime() - renderTime);
        renderTime = (float)glfwGetTime();
        
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        setUniform();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}