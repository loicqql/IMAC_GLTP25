#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "p6/p6.h"
#include "glm/glm.hpp"

#include "glimac/default_shader.hpp"

# include "PerlinNoise.hpp"

//https://opengl.developpez.com/tutoriels/apprendre-opengl/?page=systemes-de-coordonnees

int main()
{
    auto ctx = p6::Context{{1280, 720, "Project Ground"}};
    // ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    int width = 50;
    int height = 50;

    // float zde[width][height + 1];

    // for(int i = 0; i < width; i++){
    //     for(int j = 0; j < height + 1; j++){
    //         zde[i][j] = 0.f;
    //     }
    // }

    struct Vertex3D {
        glm::vec3 position;
        // glm::vec3 color;
    };

    /* code */
    //vbo
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    std::vector<Vertex3D> vertices;

    // std::vector<uint32_t> indices = {
    //     0, 1, 2,
    //     1, 2, 3
    // };

    std::vector<uint32_t> indices;

    // std::vector<Vertex3D> vertices = { 
    //     Vertex3D{{-0.5f, -0.5f, -0.0f}},
    //     Vertex3D{{ 0.5f, -0.5f, -0.0f}},
    //     Vertex3D{{-0.5f,  0.5f, -0.0f}},

        
    //     Vertex3D{{ 0.5f, -0.5f, -0.0f}},
    //     Vertex3D{{-0.5f,  0.5f, -0.0f}},
    //     Vertex3D{{0.5f, 0.5f, -0.0f}},
    // };

    const siv::PerlinNoise::seed_type seed = 123456u;
	const siv::PerlinNoise perlin{ seed };

    uint32_t indice = 0;

    for(float i = -1.0; i < 1; i += 1.0/(float)width){
        for(float j = -1.0; j < 1; j += 1.0/(float)height){
            
            vertices.push_back(Vertex3D{{i, j + 1.0/(float)height, (float)perlin.noise2D(i, j + 1.0/(float)height)}});
            vertices.push_back(Vertex3D{{i, j, (float)perlin.noise2D(i, j)}});
            vertices.push_back(Vertex3D{{i + 1.0/(float)width,  j + 1.0/(float)height, (float)perlin.noise2D(i, j + 1.0/(float)height)}});
            vertices.push_back(Vertex3D{{i + 1.0/(float)width, j, (float)perlin.noise2D(i + 1.0/(float)width, j)}});

            indices.push_back(indice);
            indices.push_back(indice + 1);
            indices.push_back(indice + 2);
            indices.push_back(indice + 1);
            indices.push_back(indice + 2);
            indices.push_back(indice + 3);

            indice += 4;

            // vertices.push_back(Vertex3D{{i, j + 1.0/(float)height, (float)perlin.noise2D(i, j + 1.0/(float)height)}});
            // vertices.push_back(Vertex3D{{i, j, (float)perlin.noise2D(i, j)}});
            // vertices.push_back(Vertex3D{{i + 1.0/(float)width,  j + 1.0/(float)height, (float)perlin.noise2D(i, j + 1.0/(float)height)}});
            // vertices.push_back(Vertex3D{{i, j, (float)perlin.noise2D(i, j)}});
            // vertices.push_back(Vertex3D{{i + 1.0/(float)width,  j + 1.0/(float)height, (float)perlin.noise2D(i, j + 1.0/(float)height)}});
            // vertices.push_back(Vertex3D{{i + 1.0/(float)width, j, (float)perlin.noise2D(i + 1.0/(float)width, j)}});
        }
    }

    // vertices.push_back(Vertex3D{{-0.5f, -0.5f, -0.0f}});
    // vertices.push_back(Vertex3D{{ 0.5f, -0.5f, -0.0f}});
    // vertices.push_back(Vertex3D{{-0.5f,  0.5f, -0.0f}});
    // vertices.push_back(Vertex3D{{ 0.5f, -0.5f, -0.0f}});
    // vertices.push_back(Vertex3D{{-0.5f,  0.5f, -0.0f}});
    // vertices.push_back(Vertex3D{{0.5f, 0.5f, -0.0f}});


    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex3D), &vertices.front(), GL_STATIC_DRAW); 

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices.front(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //vao
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao); 

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); 

    glEnableVertexAttribArray(0);
    // glEnableVertexAttribArray(8);   
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*) offsetof(Vertex3D, position));
    // glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*) offsetof(Vertex3D, color));
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(0);

    const p6::Shader shader = p6::load_shader(
        "shaders/camera.vs.glsl",
        "shaders/camera.fs.glsl"
    );

    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);

        shader.use();

        glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)1280 / (float)720, 0.1f, 100.0f);
        // retrieve the matrix uniform locations
        unsigned int modelLoc = glGetUniformLocation(shader.id(), "model");
        unsigned int viewLoc  = glGetUniformLocation(shader.id(), "view");
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        shader.set("projection", projection);

        // glimac::bind_default_shader();

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    
}