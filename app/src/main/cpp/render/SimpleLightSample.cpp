//
// Created by okunu on 2023/1/25.
//

#include "SimpleLightSample.h"
#include "MyGlRenderContext.h"

SimpleLightSample::SimpleLightSample(): secProgramObj_(0), secVertexShader_(0),
    secFragmentShader_(0), vao_(0), secVao_(0){
    cameraPos   = glm::vec3(0.0f, 0.0f,  6.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
}

SimpleLightSample::~SimpleLightSample() noexcept {}


void SimpleLightSample::init() {
    LOGI("simple light init");
    auto vert = MyGlRenderContext::getInstance()->getAssetResource("simpleLight/object.vert");
    auto frag = MyGlRenderContext::getInstance()->getAssetResource("simpleLight/object.frag");
    m_ProgramObj = GLUtils::createProgram(vert.data(), frag.data(),
                                          m_VertexShader, m_FragmentShader);

    auto secVert = MyGlRenderContext::getInstance()->getAssetResource("simpleLight/light.vert");
    auto secFrag = MyGlRenderContext::getInstance()->getAssetResource("simpleLight/light.frag");
    secProgramObj_ = GLUtils::createProgram(secVert.data(), secFrag.data(),
                                          secVertexShader_, secFragmentShader_);
    prepareData();
}

void SimpleLightSample::prepareData() {
    LOGI("simple light prepareData");
    float vertices[] = {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };

    unsigned int VBO;
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(vao_);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    glGenVertexArrays(1, &secVao_);
    glBindVertexArray(secVao_);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void SimpleLightSample::draw() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_ProgramObj);
    glBindVertexArray(vao_);

    glm::mat4 model         = glm::mat4(1.0f);
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    view = glm::lookAt(cameraPos,
                       cameraPos + cameraFront,
                       cameraUp);
    auto rat = MyGlRenderContext::getInstance()->getWidth() * 1.0f / MyGlRenderContext::getInstance()->getHeight();
    projection = glm::perspective(glm::radians(45.0f), rat, 0.1f, 100.0f);

    auto modelLoc = glGetUniformLocation(m_ProgramObj, "model");
    auto viewLoc = glGetUniformLocation(m_ProgramObj, "view");
    auto projectionLoc = glGetUniformLocation(m_ProgramObj, "projection");
    auto objectColorLoc = glGetUniformLocation(m_ProgramObj, "objectColor");
    auto lightColorLoc = glGetUniformLocation(m_ProgramObj, "lightColor");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
    glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glUseProgram(secProgramObj_);
    auto modelLoc2 = glGetUniformLocation(secProgramObj_, "model");
    auto viewLoc2 = glGetUniformLocation(secProgramObj_, "view");
    auto projectionLoc2 = glGetUniformLocation(secProgramObj_, "projection");
    glBindVertexArray(secVao_);
    glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc2, 1, GL_FALSE, glm::value_ptr(projection));
    model = glm::mat4(1.0f);
    glm::vec3 lightPos(0.7f, 0.8f, 2.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
