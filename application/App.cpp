#include <GL/glew.h>
#include "App.h"
#include "Core/Shader.h"
#include "freetype/freetype.h"
#include "glm/ext/matrix_clip_space.hpp"
#include <iostream>
#include <map>
#include <memory>

struct Character {
    GLuint TextureID;    // ID handle of the glyph texture
    glm::ivec2 Size;     // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;      // Offset to advance to next glyph
};

Example::Example(int width, int height, const std::string& name)
    : Application(width, height, name) {}

void Example::OnUpdate() {}

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
    uniform bool useAlphaTexture;
    uniform float radius;    // Rounded corner radius (in pixels)
    uniform vec2 rectSize;   // (width, height) in pixels
    
    void main()
    {
        float alpha = useAlphaTexture ? texture(text, TexCoords).r : 1.0;
    
        if (!useAlphaTexture && radius > 0.0) {
            // Signed distance to rounded rectangle
            vec2 local = TexCoords;
            vec2 halfSize = rectSize * 0.5;
            vec2 p = local - halfSize;
            vec2 rect = halfSize - vec2(radius);
            vec2 d = abs(p) - rect;
            float dist = length(max(d, 0.0)) + min(max(d.x, d.y), 0.0) - radius;
    
            // Smooth edge, 1-pixel transition
            float edgeWidth = 1.0;
            alpha *= 1.0 - smoothstep(0.0, edgeWidth, dist);
            if (alpha < 0.01)
                discard;
        }
    
        FragColor = vec4(textColor, alpha);
    }
    )";
std::unique_ptr<Shader> shader =
    std::make_unique<Shader>(vertexShaderSource, fragmentShaderSource);

void Example::OnInit() {
    glm::mat4 projection = glm::ortho(0.0f, float(1024), 0.0f, float(768));
    shader->Bind();
    shader->SetUniformMat4("projection", projection[0][0]);
    shader->SetUniformFloat("text", 0.0f);
    // glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1,
    // GL_FALSE,
    //                    &projection[0][0]);
    // glUniform1i(glGetUniformLocation(shader, "text"), 0);  // Texture unit 0

    // --------- Initialize FreeType and load font ----------
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Could not init FreeType Library\n";
    }
    // Set this to the path of a real TTF font on your system!
    const char* font_path = "/Users/anirban/Documents/Code/vision/Arial.ttf";
    FT_Face face;
    if (FT_New_Face(ft, font_path, 0, &face)) {
        std::cerr << "Failed to load font: " << font_path << std::endl;
    }
    FT_Set_Pixel_Sizes(face, 0, 25);  // height in pixels

    // --------- Load first 128 ASCII characters ----------
    glPixelStorei(GL_UNPACK_ALIGNMENT,
                  1);  // disable byte-alignment restriction

    std::map<GLchar, Character> Characters;
    GLuint VAO, VBO;
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

    // --------- Configure VAO/VBO for texture quads and squares ----------
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Example::OnRender() {
    int win_width = 1024, win_height = 768;

    glfwGetFramebufferSize(window->GetGLFWWindow(), &win_width, &win_height);
    glViewport(0, 0, win_width, win_height);
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // // Update projection for window resizing
    // projection = glm::ortho(0.0f, float(win_width), 0.0f, float(win_height));
    // glUseProgram(shader);
    // glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1,
    // GL_FALSE,
    //                    &projection[0][0]);

    // Draw text

    // RenderText(shader,
    //            "Line 2: Hello World! A quick brown fox jumped over a lazy
    //            dog", 25.0f, win_height - 200.0f, 1.0f, glm::vec3(0.0f),
    //            win_height);

    // RenderSquare(shader, 0.0f, 0.0f, 1024, 120, ConvertColor("#ffff00"),
    //              win_height, 0.0f, true);

    // RenderText(shader,
    //            "Line 1: Hello World! A quick brown fox jumped over a lazy
    //            dog", 25.0f, win_height - 80.0f, 1.0f, glm::vec3(0.0f),
    //            win_height);

    glfwSwapBuffers(window->GetGLFWWindow());
    glfwPollEvents();
}