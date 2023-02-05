//
// Created by okunu on 2023/2/4.
//

#ifndef DEMO_CAMERA_M_H
#define DEMO_CAMERA_M_H

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
__attribute__((unused)) const float ZOOM =  45.0f;

class Camera {
public:
    Camera(glm::vec3 tmpPosition = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 tmpUp = glm::vec3(0.0f, 1.0f, 0.0f),
           float tmpYaw = YAW, float tmpPitch = PITCH):
           front(glm::vec3(0.0f, 0.0f, -1.0f)){
        position = tmpPosition;
        worldUp = tmpUp;
        yaw = tmpYaw;
        pitch = tmpPitch;
        updateCameraVectors();
    }

    Camera(float posX, float posY, float posZ, float upX, float upY,
           float upZ, float tmpyaw, float tmppitch):
           front(glm::vec3(0.0f, 0.0f, -1.0f)){
        position = glm::vec3 (posX, posY, posZ);
        worldUp = glm::vec3 (upX, upY, upZ);
        yaw = tmpyaw;
        pitch = tmppitch;
        updateCameraVectors();
    }

    glm::mat4 getViewMatrix() {
        return glm::lookAt(position, position + front, up);
    }

public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

private:
    void updateCameraVectors() {
        glm::vec3 tmpfront;
        tmpfront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        tmpfront.y = sin(glm::radians(pitch));
        tmpfront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(tmpfront);
        // also re-calculate the Right and Up vector
        right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up    = glm::normalize(glm::cross(right, front));
    }
};

#endif //DEMO_CAMERA_M_H
