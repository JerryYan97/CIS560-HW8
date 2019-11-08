#include "camera.h"

#include <la.h>
#include <iostream>


Camera::Camera():
    Camera(400, 400)
{
    look = glm::vec3(0,0,1);
    up = glm::vec3(0,1,0);
    right = glm::vec3(1,0,0);
}

Camera::Camera(unsigned int w, unsigned int h):
    Camera(w, h, glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,1,0))
{}

Camera::Camera(unsigned int w, unsigned int h, const glm::vec3 &e, const glm::vec3 &r, const glm::vec3 &worldUp):
    fovy(45),
    width(w),
    height(h),
    near_clip(0.1f),
    far_clip(1000),
    eye(glm::vec3(0, 0, 0)),
    ref(r),
    world_up(worldUp),
    zoom(10.f),
    up(glm::vec3(0, 1, 0))
{
    RecomputeAttributes();
}

Camera::Camera(const Camera &c):
    fovy(c.fovy),
    width(c.width),
    height(c.height),
    near_clip(c.near_clip),
    far_clip(c.far_clip),
    aspect(c.aspect),
    eye(glm::vec3(0, 0, 0)),
    ref(c.ref),
    look(glm::vec3(0, 0, 1)),
    up(glm::vec3(0, 1, 0)),
    right(glm::vec3(1, 0, 0)),
    world_up(c.world_up),
    V(c.V),
    H(c.H),
    zoom(10.f),
    theta(0.f),
    phi(0.f)
{}

glm::mat4 Camera::CameraModelToSpherecal()
{
    glm::mat4 r1 = glm::rotate(glm::mat4(1.0f), glm::radians(theta), glm::vec3(0, 1, 0));
    glm::mat4 r2 = glm::rotate(glm::mat4(1.0f), glm::radians(phi), glm::vec3(1, 0, 0));
    glm::mat4 t1 = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, zoom));
    return r1 * r2 * t1;
}

void Camera::RecomputeAttributes()
{
    look = glm::vec3(0, 0, 1);
    right = glm::vec3(1, 0, 0);
    up = glm::vec3(0, 1, 0);

    float tan_fovy = tan(glm::radians(fovy/2));
    //float len = glm::length(ref - eye);
    //float len = 1;
    glm::mat4 spherecalToWorld = glm::translate(glm::mat4(1.0f), this->ref);
    glm::vec4 eyeWorld = spherecalToWorld * this->CameraModelToSpherecal() * glm::vec4(eye.x, eye.y, eye.z, 1);
    float len = glm::length(ref - glm::vec3(eyeWorld.x, eyeWorld.y, eyeWorld.z));

    aspect = float(width)/float(height);
    V = up*len*tan_fovy;
    H = right*len*aspect*tan_fovy;
}

glm::mat4 Camera::getViewProj()
{
    glm::mat4 spherecalToWorld = glm::translate(glm::mat4(1.0f), ref);
    glm::vec4 eyeWorld = spherecalToWorld * CameraModelToSpherecal() * glm::vec4(eye.x, eye.y, eye.z, 1);
    glm::vec4 upWorld = spherecalToWorld * CameraModelToSpherecal() * glm::vec4(up.x, up.y, up.z, 0);

    return glm::perspective(glm::radians(fovy), width / (float)height, near_clip, far_clip) *
                            glm::lookAt(glm::vec3(eyeWorld.x, eyeWorld.y, eyeWorld.z),
                                        ref,
                                        glm::vec3(upWorld.x, upWorld.y, upWorld.z));
}

void Camera::RotateAboutUp(float deg)
{
    theta += deg;
    RecomputeAttributes();
}

void Camera::RotateAboutRight(float deg)
{
    phi += deg;
    RecomputeAttributes();
}

void Camera::TranslateAlongLook(float amt)
{
    zoom -= amt;
}

void Camera::TranslateAlongRight(float amt)
{
    glm::vec3 translation = right * amt;
    ref += translation;
}

void Camera::TranslateAlongUp(float amt)
{
    glm::vec3 translation = up * amt;
    ref += translation;
}
