// Build with: g++ main.cpp -lglfw -lGLEW -lGL -lfreetype -ldl -lpthread -lX11
// -lXrandr -lXi -lXxf86vm -lXcursor -lXinerama On macOS: g++ main.cpp -lglfw
// -lGLEW -framework OpenGL -lfreetype

#include <iostream>
#include <map>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

// glm is only used for math, you can remove it and use plain floats if you wish
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Shader sources
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;
uniform mat4 projection;
void main()
{
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
in vec2 TexCoords;
out vec4 FragColor;
uniform sampler2D text;
uniform vec3 textColor;
void main()
{
    float alpha = texture(text, TexCoords).r;
    FragColor = vec4(textColor, alpha);
}
)";

// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    GLuint TextureID;    // ID handle of the glyph texture
    glm::ivec2 Size;     // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;      // Offset to advance to next glyph
};

std::map<GLchar, Character> Characters;
GLuint VAO, VBO;

void RenderText(GLuint shader, std::string text, float x, float y, float scale,
                glm::vec3 color, int win_height) {
    // Activate corresponding render state
    glUseProgram(shader);
    glUniform3f(glGetUniformLocation(shader, "textColor"), color.x, color.y,
                color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
        // OpenGL's y=0 is at bottom, FreeType's is top, so we invert y for
        // OpenGL coords
        ypos = win_height - ypos - ch.Size.y * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // Update VBO for each character
        float vertices[6][4] = {
            {xpos, ypos + h, 0.0f, 0.0f},    {xpos, ypos, 0.0f, 1.0f},
            {xpos + w, ypos, 1.0f, 1.0f},

            {xpos, ypos + h, 0.0f, 0.0f},    {xpos + w, ypos, 1.0f, 1.0f},
            {xpos + w, ypos + h, 1.0f, 0.0f}};
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint CompileShader(const char* src, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, 0);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        char buffer[512];
        glGetShaderInfoLog(shader, 512, NULL, buffer);
        std::cerr << "Shader Compile Failed:\n" << buffer << std::endl;
    }
    return shader;
}

GLuint CreateProgram(const char* vs, const char* fs) {
    GLuint vertex = CompileShader(vs, GL_VERTEX_SHADER);
    GLuint frag = CompileShader(fs, GL_FRAGMENT_SHADER);
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vertex);
    glAttachShader(prog, frag);
    glLinkProgram(prog);
    GLint status;
    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (!status) {
        char buffer[512];
        glGetProgramInfoLog(prog, 512, NULL, buffer);
        std::cerr << "Program Link Failed:\n" << buffer << std::endl;
    }
    glDeleteShader(vertex);
    glDeleteShader(frag);
    return prog;
}

int main() {
    // --------- Initialize GLFW ----------
    if (!glfwInit()) {
        std::cerr << "Could not init GLFW\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    int win_width = 800, win_height = 600;
    GLFWwindow* window = glfwCreateWindow(
        win_width, win_height, "OpenGL TTF Font Rendering", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // --------- Initialize GLEW ----------
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to init GLEW\n";
        return -1;
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // --------- Build Shader Program ----------
    GLuint shader = CreateProgram(vertexShaderSource, fragmentShaderSource);

    // --------- Set up projection matrix ----------
    glm::mat4 projection =
        glm::ortho(0.0f, float(win_width), 0.0f, float(win_height));
    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE,
                       &projection[0][0]);

    // --------- Initialize FreeType and load font ----------
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Could not init FreeType Library\n";
        return -1;
    }
    // Set this to the path of a real TTF font on your system!
    const char* font_path = "arial.ttf";
    FT_Face face;
    if (FT_New_Face(ft, font_path, 0, &face)) {
        std::cerr << "Failed to load font: " << font_path << std::endl;
        return -1;
    }
    FT_Set_Pixel_Sizes(face, 0, 48);  // height in pixels

    // --------- Load first 128 ASCII characters ----------
    glPixelStorei(GL_UNPACK_ALIGNMENT,
                  1);  // disable byte-alignment restriction
    for (GLubyte c = 32; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "Failed to load Glyph: " << (char)c << std::endl;
            continue;
        }
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                     face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                     face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (GLuint)face->glyph->advance.x};
        Characters.insert(std::pair<char, Character>(c, character));
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // --------- Configure VAO/VBO for texture quads ----------
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // --------- Render Loop ----------
    while (!glfwWindowShouldClose(window)) {
        glfwGetFramebufferSize(window, &win_width, &win_height);
        glViewport(0, 0, win_width, win_height);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Update projection for window resizing
        projection =
            glm::ortho(0.0f, float(win_width), 0.0f, float(win_height));
        glUseProgram(shader);
        glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1,
                           GL_FALSE, &projection[0][0]);

        // Draw text
        RenderText(shader, "Hello, OpenGL!", 25.0f, win_height - 80.0f, 1.0f,
                   glm::vec3(0.5, 0.8f, 0.2f), win_height);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
