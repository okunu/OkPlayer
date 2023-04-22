//
// Created by okunu on 2023/4/5.
//

#ifndef DEMO_3DSAMPLE_H
#define DEMO_3DSAMPLE_H

#include "BaseSample.h"
#include "Shader_m.h"
#include "glm-0.9.9.8/glm/glm.hpp"
#include "glm-0.9.9.8/glm/gtx/transform.hpp"
#include "glm-0.9.9.8/glm/gtc/type_ptr.hpp"
#include "Shader_m.h"
#include "Camera_m.h"
#include "model_m.h"

class Type3DSample: public BaseSample {
public:
    Type3DSample();
    ~Type3DSample();

    void init() override;
    void draw() override;
    void rorate(float xoffset, float yoofset, float distance);

private:
    Shader shader;
    Camera camera;
    Model ourModel;
    float xoffset;
    float yoffset;
    float distance;
    glm::mat4 model;
};

#endif //DEMO_3DSAMPLE_H
