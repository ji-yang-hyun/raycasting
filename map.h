#ifndef MAP
#define MAP

#include<vector>
#include<iostream>
#include"bad_apple_data.h"
using namespace std;

#define mapSizeX 20
#define mapSizeY 15
#define mapSizeZ 10
// 그냥 정사각형 세상으로 하자.

int screenlayer = 0;

int map[mapSizeZ][mapSizeY][mapSizeX] = {{{0}}};

vector<glm::vec3> walls;

#define boxDepth 4
#define boxCount 9 // 2 ^ 3 + 1(root)

struct Box{
    glm::vec3 areaP1;
    glm::vec3 areaP2;
    int childIdx1;
    int childIdx2;
    vector<glm::vec3> cubes;
};

vector<Box> boxes;

void sep(int n, int depth){
    if(depth > boxDepth){
        boxes[n].childIdx1 = 0;
        boxes[n].childIdx2 = 0;
        return;
    }

    Box box = boxes[n];
    int maxAxis;
    glm::vec3 gap = box.areaP2 - box.areaP1;
    if(gap.x >= gap.y && gap.x >= gap.z) maxAxis = 0;
    if(gap.y >= gap.x && gap.y >= gap.z) maxAxis = 1;
    if(gap.z >= gap.y && gap.z >= gap.x) maxAxis = 2;
    
    glm::vec3 cube;
    float b = (box.areaP1[maxAxis] + box.areaP2[maxAxis])/2.0f;
    // 큐브 크기에 의한 두 바운딩 박스의 경계(겹칠 수도 있음)
    float b1 = b;
    float b2 = b;

    vector<glm::vec3> cubes1;
    vector<glm::vec3> cubes2;

    for(int i=0;i<box.cubes.size();i++){
        cube = box.cubes[i];
        if(cube[maxAxis] <= b){ // 더 작은 쪽이니 1를 유지
            if(b1 < cube[maxAxis] + 0.5){
                b1 = cube[maxAxis] + 0.5;
            }
            cubes1.push_back(cube);
        }
        else{ // 더 큰 쪽이니 2를 유지
            if(b2 > cube[maxAxis] - 0.5){
                b2 = cube[maxAxis] - 0.5;
            }
            cubes2.push_back(cube);
        }
    }

    glm::vec3 newP2 = box.areaP2;
    newP2[maxAxis] = b1;
    Box box1;
    box1.areaP1 = box.areaP1;
    box1.areaP2 = newP2;
    box1.cubes = cubes1;

    glm::vec3 newP1 = box.areaP1;
    newP1[maxAxis] = b2;
    Box box2;
    box2.areaP1 = newP1;
    box2.areaP2 = box.areaP2;
    box2.cubes = cubes2;

    boxes.push_back(box1);
    box.childIdx1 = boxes.size() - 1;
    boxes.push_back(box2);
    box.childIdx2 = boxes.size() - 1;

    sep(box.childIdx1, depth + 1);
    sep(box.childIdx2, depth + 1);

    return;
}

void root(){
    Box rt;
    rt.areaP1 = glm::vec3(0,0,0);
    rt.areaP2 = glm::vec3(mapSizeX,mapSizeY,mapSizeZ);
    rt.cubes = walls;

    boxes.push_back(rt);

    sep(0, 0);
}

void wallCoordinate(){
    for(int z=0;z<mapSizeZ;z++){
        for(int y=0;y<mapSizeY;y++){
            for(int x=0;x<mapSizeX;x++){
                if(map[z][y][x] == 1){
                    // 배열은 y좌표 뒤집히므로 그냥 좌표계를 사용하기 위해 y값을 거꾸로!
                    walls.push_back(glm::vec3(x + 0.5, mapSizeY - y - 0.5, z + 0.5));
                }
            }
        }
    }
}

#endif