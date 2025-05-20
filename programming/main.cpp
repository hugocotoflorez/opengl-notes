#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/exponential.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

float Aspect;
using namespace glm;

/* 1: do not apply vector sum and modulo division for each vertex */
#define DO_NO_CALC_NORMALS 0
/* 1: apply phong */
#define PHONG 1

#if !defined(PHONG) || PHONG
const char *vShader = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 Normal;
out vec3 Color;
out vec3 FragPos;
void main() {
        Normal = mat3(transpose(inverse(model))) * aNormal;
        Color = aColor;
        FragPos = vec3( model * vec4(aPos, 1.0));
        gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

const char *fShader = R"(
#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float Ka;
uniform float Kd;
uniform float Ks;

in vec3 Normal;
in vec3 Color;
in vec3 FragPos;
void main() {
        // Ambient
        vec3 ambient = lightColor * Ka;

        // diffuse
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos); // direction from fragment to light
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor * Kd;

        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
        vec3 specular = Ks * spec * lightColor;

        FragColor = vec4((ambient + diffuse + specular) * Color, 1.0);
}
)";

#else
const char *vShader = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float Ka;
uniform float Kd;
uniform float Ks;
out vec3 Color;
void main() {
        vec3 Normal = mat3(transpose(inverse(model))) * aNormal;
        vec3 FragPos = vec3(model * vec4(aPos, 1.0));
        gl_Position = projection * view * model * vec4(aPos, 1.0);

        // Ambient
        vec3 ambient = lightColor * Ka;

        // diffuse
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos); // direction from fragment to light
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor * Kd;

        Color = (ambient + diffuse) * aColor;
}
)";

const char *fShader = R"(
#version 330 core
out vec4 FragColor;
in vec3 Color;
void main() {
        FragColor = vec4(Color, 1.0);
}
)";
#endif

#define Point(a, b, c) a, b, c
#define Normal(a, b, c) a, b, c
#define Color(r, g, b) r, g, b
#define Face(a, b, c, d) a, b, c, c, d, a
#define Vertex(a, b, c) a, b, c

void
calc_med(float *vec, int size)
{
#if defined(DO_NO_CALC_NORMALS) && DO_NO_CALC_NORMALS
        return;
#endif
        for (int i = 0; i < size; i += 9) {
                int count = 1;
                for (int j = 0; j < size; j += 9) {
                        if (i != j &&
                            vec[i + 0] == vec[j + 0] &&
                            vec[i + 1] == vec[j + 1] &&
                            vec[i + 2] == vec[j + 2]) {
                                vec[i + 3] += vec[j + 3];
                                vec[i + 4] += vec[j + 4];
                                vec[i + 5] += vec[j + 5];
                                count++;
                        }
                }
                float mod = sqrt(
                vec[i + 3] * vec[i + 3] +
                vec[i + 4] * vec[i + 4] +
                vec[i + 5] * vec[i + 5]);
                vec[i + 3] /= mod;
                vec[i + 4] /= mod;
                vec[i + 5] /= mod;
        }
}


float cube[] = {
        Vertex(Point(-0.5, -0.5, -0.5), Normal(0, 0, -1), Color(1, 0, 0)),
        Vertex(Point(0.5, -0.5, -0.5), Normal(0, 0, -1), Color(1, 0, 0)),
        Vertex(Point(0.5, 0.5, -0.5), Normal(0, 0, -1), Color(1, 0, 0)),
        Vertex(Point(-0.5, 0.5, -0.5), Normal(0, 0, -1), Color(1, 0, 0)),

        Vertex(Point(-0.5, -0.5, 0.5), Normal(0, 0, 1), Color(0, 1, 0)),
        Vertex(Point(0.5, -0.5, 0.5), Normal(0, 0, 1), Color(0, 1, 0)),
        Vertex(Point(0.5, 0.5, 0.5), Normal(0, 0, 1), Color(0, 1, 0)),
        Vertex(Point(-0.5, 0.5, 0.5), Normal(0, 0, 1), Color(0, 1, 0)),

        Vertex(Point(-0.5, -0.5, -0.5), Normal(0, -1, 0), Color(0, 0, 1)),
        Vertex(Point(0.5, -0.5, -0.5), Normal(0, -1, 0), Color(0, 0, 1)),
        Vertex(Point(0.5, -0.5, 0.5), Normal(0, -1, 0), Color(0, 0, 1)),
        Vertex(Point(-0.5, -0.5, 0.5), Normal(0, -1, 0), Color(0, 0, 1)),

        Vertex(Point(-0.5, 0.5, -0.5), Normal(0, 1, 0), Color(1, 1, 0)),
        Vertex(Point(0.5, 0.5, -0.5), Normal(0, 1, 0), Color(1, 1, 0)),
        Vertex(Point(0.5, 0.5, 0.5), Normal(0, 1, 0), Color(1, 1, 0)),
        Vertex(Point(-0.5, 0.5, 0.5), Normal(0, 1, 0), Color(1, 1, 0)),

        Vertex(Point(-0.5, -0.5, -0.5), Normal(-1, 0, 0), Color(0, 1, 1)),
        Vertex(Point(-0.5, 0.5, -0.5), Normal(-1, 0, 0), Color(0, 1, 1)),
        Vertex(Point(-0.5, 0.5, 0.5), Normal(-1, 0, 0), Color(0, 1, 1)),
        Vertex(Point(-0.5, -0.5, 0.5), Normal(-1, 0, 0), Color(0, 1, 1)),

        Vertex(Point(0.5, -0.5, -0.5), Normal(1, 0, 0), Color(1, 0, 1)),
        Vertex(Point(0.5, 0.5, -0.5), Normal(1, 0, 0), Color(1, 0, 1)),
        Vertex(Point(0.5, 0.5, 0.5), Normal(1, 0, 0), Color(1, 0, 1)),
        Vertex(Point(0.5, -0.5, 0.5), Normal(1, 0, 0), Color(1, 0, 1)),
};

unsigned int indices[] = {
        Face(0, 1, 2, 3), // frontal
        Face(4, 5, 6, 7), // trasera
        Face(8, 9, 10, 11), // inferior
        Face(12, 13, 14, 15), // superior
        Face(16, 17, 18, 19), // izquierda
        Face(20, 21, 22, 23), // derecha
};

unsigned int
compileShader(unsigned int type, const char *src)
{
        unsigned int s = glCreateShader(type);
        glShaderSource(s, 1, &src, nullptr);
        glCompileShader(s);
        int success;
        char infoLog[512];
        glGetShaderiv(s, GL_COMPILE_STATUS, &success);
        if (!success) {
                glGetShaderInfoLog(s, 512, NULL, infoLog);
                std::cerr << "Shader compilation failed: " << infoLog << std::endl;
        }
        return s;
}

int
main()
{
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#define W 600
#define H 600
        GLFWwindow *w = glfwCreateWindow(W, H, "Rotating Cube", nullptr, nullptr);
        if (w == nullptr) {
                std::cerr << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
                return -1;
        }
        Aspect = (float) W / (float) H;
        glfwMakeContextCurrent(w);
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
                std::cerr << "Failed to initialize GLAD" << std::endl;
                return -1;
        }
        glViewport(0, 0, W, H);

        // Compile shaders
        unsigned int vs = compileShader(GL_VERTEX_SHADER, vShader);
        unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fShader);
        unsigned int prog = glCreateProgram();
        glAttachShader(prog, vs);
        glAttachShader(prog, fs);
        glLinkProgram(prog);
        int success;
        glGetProgramiv(prog, GL_LINK_STATUS, &success);
        if (!success) {
                char infoLog[512];
                glGetProgramInfoLog(prog, 512, nullptr, infoLog);
                std::cerr << "Shader linking failed:\n"
                          << infoLog << std::endl;
        }
        glDeleteShader(vs);
        glDeleteShader(fs);

        calc_med(cube, sizeof cube / sizeof *cube);

        // VAO/VBO/EBO
        unsigned int vao, vbo, ebo;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // vertices
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        // normales
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // color
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *) (6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glEnable(GL_DEPTH_TEST);

        glUseProgram(prog);
        while (!glfwWindowShouldClose(w)) {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                float t = glfwGetTime();
#define camPos 0, 0.0f, -3.0f
#define lightPos 0.0f, 0.0f, -1.0f
                glm::mat4 model = glm::rotate(glm::mat4(1.0f), t, glm::vec3(0.5f, 1.0f, 0.0f));
                glm::mat4 view = glm::lookAt(vec3(camPos), vec3(0, 0, 0), vec3(0, 1.0f, 0));
                glm::mat4 projection = glm::perspective(glm::radians(45.0f), Aspect, 0.1f, 100.0f);

                glUniformMatrix4fv(glGetUniformLocation(prog, "model"), 1, GL_FALSE, glm::value_ptr(model));
                glUniformMatrix4fv(glGetUniformLocation(prog, "view"), 1, GL_FALSE, glm::value_ptr(view));
                glUniformMatrix4fv(glGetUniformLocation(prog, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
                glUniform3f(glGetUniformLocation(prog, "lightColor"), 0.5f, 0.5f, 0.5f);
                glUniform3f(glGetUniformLocation(prog, "lightPos"), lightPos);
                glUniform3f(glGetUniformLocation(prog, "viewPos"), camPos);
                glUniform1f(glGetUniformLocation(prog, "Ka"), 1.0f);
                glUniform1f(glGetUniformLocation(prog, "Kd"), 1.0f);
                glUniform1f(glGetUniformLocation(prog, "Ks"), 1.0f);

                glBindVertexArray(vao);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                glfwSwapBuffers(w);
                glfwPollEvents();
        }

        glfwTerminate();
}
