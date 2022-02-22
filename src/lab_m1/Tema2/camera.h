#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

namespace camera
{
    class Camera
    {
    public:
        Camera(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
        {
            Set(position, center, up);
        }

        ~Camera()
        {
        }

        /* Update camera */
        void Set(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
        {
            this->position = position;
            this->forward = glm::normalize(center - position);
            this->right = glm::cross(forward, up);
            this->up = glm::cross(right, forward);
            this->distanceToTarget = glm::length(position - center);
        }

        /* Move camera forward */
        void MoveForward(float distance)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
            position += dir * distance;
        }

        /* Translate camera forward */
        void TranslateForward(float distance)
        {
            position += glm::normalize(forward) * distance;
        }

        /* Translate camera upward */
        void TranslateUpward(float distance)
        {
            position += glm::normalize(up) * distance;
        }

        /* Translate camera right */
        void TranslateRight(float distance)
        {
            position += glm::normalize(glm::vec3(right.x, 0, right.z)) * distance;
        }

        /* Rotate camera in first person mode on Ox */
        void RotateFirstPerson_OX(float angle)
        {
            forward = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, right) * glm::vec4(forward, 1)));
            up = glm::normalize(glm::cross(right, forward));
        }

        /* Rotate camera in first person mode on Oy */
        void RotateFirstPerson_OY(float angle)
        {
            forward = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, up) * glm::vec4(forward, 1)));
            right = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, up) * glm::vec4(right, 1)));
            up = glm::normalize(glm::cross(right, forward));
        }

        /* Rotate camera in first person mode on Oz */
        void RotateFirstPerson_OZ(float angle)
        {
            right = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, forward) * glm::vec4(right, 1)));
            up = glm::normalize(glm::cross(right, forward));
        }

        /* Rotate camera in third person mode on Ox */
        void RotateThirdPerson_OX(float angle)
        {
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OX(angle);
            TranslateForward(-distanceToTarget);
        }

        /* Rotate camera in third person mode on Oy */
        void RotateThirdPerson_OY(float angle)
        {
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OY(angle);
            TranslateForward(-distanceToTarget);
        }

        /* Rotate camera in third person mode on Oz */
        void RotateThirdPerson_OZ(float angle)
        {
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OZ(angle);
            TranslateForward(-distanceToTarget);
        }

        /* Get view matrix to render objects */
        glm::mat4 GetViewMatrix()
        {
            return glm::lookAt(position, position + forward, up);
        }

        /* Get the position of the object the camera looks at */
        glm::vec3 GetTargetPosition()
        {
            return position + forward * distanceToTarget;
        }

    public:
        float distanceToTarget;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
    };
}   // namespace camera
