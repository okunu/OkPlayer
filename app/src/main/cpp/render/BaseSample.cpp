//
// Created by okunu on 2022/10/30.
//

#include "BaseSample.h"
#include "LogUtil.h"

BaseSample::BaseSample(): m_ProgramObj(0),
    m_VertexShader(0),
    m_FragmentShader(0){
}

BaseSample::~BaseSample() {}

void BaseSample::init() {
    LOGI("base sample init");
}

void BaseSample::draw() {
}

void BaseSample::prepareData() {}

void BaseSample::prepareTexture() {}

void BaseSample::changeDirection(int direction) {
}

void BaseSample::setAngle(float yaw, float pitch) {
}