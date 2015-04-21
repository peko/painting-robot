// ImGui - standalone example application for Glfw + OpenGL 2, using fixed pipeline

#include <stdio.h>
using namespace std;

#include <imgui.h>
#include "imgui_impl_glfw.h"
#include <stdio.h>
#include <GLFW/glfw3.h>

#include <opencv2/opencv.hpp>
using namespace cv;


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

Mat src;

// Function turn a Mat into a texture, and return the texture ID as a GLuint for use
GLuint matToTexture(Mat &mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter) {
    // Generate a number for our textureID's unique handle
    GLuint textureID;
    glGenTextures(1, &textureID);
 
    // Bind to our texture handle
    glBindTexture(GL_TEXTURE_2D, textureID);
 
    // Catch silly-mistake texture interpolation method for magnification
    if (magFilter == GL_LINEAR_MIPMAP_LINEAR  ||
        magFilter == GL_LINEAR_MIPMAP_NEAREST ||
        magFilter == GL_NEAREST_MIPMAP_LINEAR ||
        magFilter == GL_NEAREST_MIPMAP_NEAREST)
    {
        cout << "You can't use MIPMAPs for magnification - setting filter to GL_LINEAR" << endl;
        magFilter = GL_LINEAR;
    }
 
    // Set texture interpolation methods for minification and magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
 
    // Set texture clamping method
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapFilter);
 
    // Set incoming texture format to:
    // GL_BGR       for CV_CAP_OPENNI_BGR_IMAGE,
    // GL_LUMINANCE for CV_CAP_OPENNI_DISPARITY_MAP,
    // Work out other mappings as required ( there's a list in comments in main() )
    
    GLenum inputColourFormat = GL_RGB;
 
    // Create the texture
    cout << "glTexImage2D ";
    cout << mat.channels() << " " << mat.cols << " " << mat.rows << endl;
    
    glTexImage2D(GL_TEXTURE_2D,     // Type of texture
                 0,                 // Pyramid level (for mip-mapping) - 0 is the top level
                 GL_RGB,           // Internal colour format to convert to
                 mat.cols,          // Image width  i.e. 640 for Kinect in standard mode
                 mat.rows,          // Image height i.e. 480 for Kinect in standard mode
                 0,                 // Border width in pixels (can either be 1 or 0)
                 inputColourFormat, // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
                 GL_UNSIGNED_BYTE,  // Image data type
                 mat.ptr());        // The actual image data itself
 
    // If we're using mipmaps then generate them. Note: This requires OpenGL 3.0 or higher
    if (minFilter == GL_LINEAR_MIPMAP_LINEAR  ||
        minFilter == GL_LINEAR_MIPMAP_NEAREST ||
        minFilter == GL_NEAREST_MIPMAP_LINEAR ||
        minFilter == GL_NEAREST_MIPMAP_NEAREST)
    {
        // glGenerateMipmap(GL_TEXTURE_2D);
    }
 
    return textureID;
}

void DrawImage(GLuint tid) {

    // GLFWvidmode return_struct;
    // glfwGetDesktopMode( &return_struct );
    // int height = return_struct.Height;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glLoadIdentity();
    glDisable(GL_LIGHTING);

    glColor3f(1,1,1);
    // glEnable(GL_TEXTURE_2D);
    // glBindTexture(GL_TEXTURE_2D, tid);

    // Draw a textured quad
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(10  , 10, 0);
    glTexCoord2f(0, 1); glVertex3f(10  ,580, 0);
    glTexCoord2f(1, 1); glVertex3f(780, 580, 0);
    glTexCoord2f(1, 0); glVertex3f(780,  10, 0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);

}


GLuint loadTexture(){

    GLuint tid;

    src = imread("../data/test01.png");

    circle(src, Point(256,256),255, 0xFF0000);
    // Convert to texture
    tid = matToTexture(src, GL_NEAREST, GL_NEAREST, GL_CLAMP);
    // Bind texture
    // glBindTexture(GL_TEXTURE_2D, tid);

    return tid;
}

void unloadTexture(GLuint tid) {

    // Free the texture memory
    glDeleteTextures(1, &tid);

}

int main(int, char**) {

    // Setup window
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(1);
    GLFWwindow* window = glfwCreateWindow(800, 600, "ImGui OpenGL2 example", NULL, NULL);
    glfwMakeContextCurrent(window);
    cout << "GL_VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GL_SHADING_LANGUAGE_VERSION: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
    
    GLuint tid = loadTexture();

    // Setup ImGui binding
    ImGui_ImplGlfw_Init(window, true);
    //ImGuiIO& io = ImGui::GetIO();
    //ImFont* my_font0 = io.Fonts->AddFontDefault();
    //ImFont* my_font1 = io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
    //ImFont* my_font2 = io.Fonts->AddFontFromFileTTF("../../extra_fonts/Karla-Regular.ttf", 16.0f);
    //ImFont* my_font3 = io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f); my_font3->DisplayOffset.y += 1;
    //ImFont* my_font4 = io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f); my_font4->DisplayOffset.y += 1;
    //ImFont* my_font5 = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, io.Fonts->GetGlyphRangesJapanese());

    bool show_test_window = false;
    bool show_another_window = false;
    ImVec4 clear_color = ImColor(114, 144, 154);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplGlfw_NewFrame();

        // 1. Show a simple window
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
        {
            static float f = 0.0f;
            ImGui::Text("Hello, world!");
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ImGui::ColorEdit3("clear color", (float*)&clear_color);
            if (ImGui::Button("Test Window")) show_test_window ^= 1;
            if (ImGui::Button("Another Window")) show_another_window ^= 1;
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        // 2. Show another simple window, this time using an explicit Begin/End pair
        if (show_another_window)
        {
            ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiSetCond_FirstUseEver);
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello");
            ImGui::End();
        }

        // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
        if (show_test_window)
        {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
            ImGui::ShowTestWindow(&show_test_window);
        }

        // Rendering
        glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        DrawImage(tid);

        ImGui::Render();
        glfwSwapBuffers(window);
    }
    
    unloadTexture(tid);

    // Cleanup
    ImGui_ImplGlfw_Shutdown();
    glfwTerminate();

    return 0;
}
