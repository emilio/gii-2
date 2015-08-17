#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#define PISTOLOS_DEBUG
char* pistolos_file_to_buffer(const char* source) {
    FILE* fptr;
    long length;
    char* buf;

    fptr = fopen(source, "rb"); /* Open file for reading */

    if (!fptr) /* Return NULL on failure */
        return NULL;

    fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */

    length = ftell(fptr); /* Find out how many bytes into the file we are */

    buf = (char*)malloc(length +
                        1); /* Allocate a buffer for the entire length of the
                               file and a null terminator */

    fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */

    fread(buf, length, 1,
          fptr); /* Read the contents of the file in to the buffer */

    fclose(fptr); /* Close the file */

    buf[length] = 0; /* Null terminator */

    return buf; /* Return the buffer */
}

typedef struct shader_info {
    GLenum type;
    GLuint value;
    const char source[2048];
} shader_info_t;

shader_info_t SHADERS[] = {
    //	{ GL_VERTEX_SHADER, 0, "shaders/shooter.vert" },
    //	{ GL_FRAGMENT_SHADER, 0, "shaders/shooter.frag" }
    {GL_VERTEX_SHADER, 0, "#version 150 core\n"
                          "in vec2 position;"
                          "void main() {"
                          "    gl_Position = vec4(position, 0.0, 1.0);"
                          "}"},
    {GL_FRAGMENT_SHADER, 0, "#version 150 core\n"
                            "out vec4 outColor;"
                            "void main() {"
                            "   outColor = vec4(1.0, 1.0, 1.0, 1.0);"
                            "}"}};

int pistolos_gl_compile_shader(shader_info_t* shader) {
    // const char* source = pistolos_file_to_buffer(shader->source);
    const char* source = shader->source;
    GLint status;

    printf("%s\n", source);

    if (source == NULL) {
#ifdef PISTOLOS_DEBUG
        fprintf(stderr, "%s shader source file %s not found.\n",
                shader->type == GL_VERTEX_SHADER ? "Vertex" : "Fragment",
                shader->source);
#endif
        return 0;
    }

    shader->value = glCreateShader(shader->type);
    glShaderSource(shader->value, 1, (const GLchar**)&source, NULL);

    // free(source);
    printf("After compiling\n");

    glCompileShader(shader->value);

    glGetShaderiv(shader->value, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE) {
#ifdef PISTOLOS_DEBUG
        int log_max_length;
        char* log;
        glGetShaderiv(shader->value, GL_INFO_LOG_LENGTH, &log_max_length);
        log = (char*)malloc(log_max_length);
        fprintf(stderr, "Compilation of %s failed.\nLog showed below: ",
                shader->source);
        glGetShaderInfoLog(shader->value, log_max_length, NULL, log);
        fprintf(stderr, "%s\n", log);
        free(log);
#endif
        return 0;
    }

    return 1;
}

int pistolos_gl_compile_shaders() {
    size_t i;
    size_t shaders = sizeof(SHADERS) / sizeof(shader_info_t);

    for (i = 0; i < shaders; ++i)
        if (!pistolos_gl_compile_shader(&SHADERS[i]))
            return 0;

    return 1;
}

void pistolos_gl_init_private() {
    // Clear to black
    glClearColor(0, 0, 0, 1);
}

void pistolos_gl_draw() {
    // Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Rotate the image
    glMatrixMode(GL_MODELVIEW); // Current matrix affects objects positions
    glLoadIdentity();           // Initialize to the identity
}

void pistolos_gl_init(int* argcp, char** argv) {
    glewInit();
    glutInit(argcp, argv);

    // We're going to animate it, so double buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Initial parameters for window position and size
    glutInitWindowPosition(60, 60);
    glutInitWindowSize(600, 600);

    glutCreateWindow("Pistolos");

    pistolos_gl_compile_shaders();

    // Initialize OpenGL parameters.
    pistolos_gl_init_private();

    // Call this whenever window needs redrawing
    glutDisplayFunc(pistolos_gl_draw);

    // Start the main loop.  glutMainLoop never returns.
    glutMainLoop();
}
