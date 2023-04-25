/*
        Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "p6/p6.h"

class ShadowFrameBuffers {
public:
    ShadowFrameBuffers();

    ~ShadowFrameBuffers();

    void BindForWriting() const;

    GLuint getShadowTexture() const;

private:
    GLuint fbo;
    GLuint shadowMap;

    int SHADOW_WIDTH = 4096;
    int SHADOW_HEIGHT = 4096;
};
