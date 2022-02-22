#pragma once

#include "components/simple_scene.h"
#include "defines.h"
#include "colors.h"

namespace m1
{
    class Globe
    {
    public:
        glm::vec3 coord;
        Texture2D* texture;
        float time;

        Globe(glm::vec3 _coord, Texture2D* _texture)
            : coord(_coord)
            , texture(_texture)
            , time(0) { }

        void update(float deltaTimeSeconds);
    };

}   // namespace m1