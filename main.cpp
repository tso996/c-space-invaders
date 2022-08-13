//  Created by Joe on 18/06/2022.
//
#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GL/glew.h>//this needs to be included before any other gl header
#include <GLFW/glfw3.h>

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

struct Buffer
{
    size_t width, height;
    uint32_t* data;
};

void buffer_clear(Buffer* buffer, uint32_t color)
{
    for(size_t i = 0; i < buffer->width * buffer->height; ++i)
    {
        buffer->data[i] = color;
    }
}

uint32_t rgb_to_uint32(uint8_t r, uint8_t g, uint8_t b)
{
    return (r << 24) | (g << 16) | (b << 8) | 255;
}

int main(int argc, const char * argv[]) {
    
    std::cout<<"STARTING PROGRAM"<<std::endl;


    constexpr size_t buffer_width = 224;
    constexpr size_t buffer_height = 256;

    #ifdef DEBUG
        std::cout<<"debug build"<<std::endl;
    #else
        std::cout<<"release build"<<std::endl;
    #endif
    
    if (!glfwInit())
    {
        std::cout<<"glfw did not initialise properly.."<<std::endl;
        return -1;
    }else{
        std::cout<<"glfw initialised correctly.."<<std::endl;
    }
    
    //The following is required to change the opengl version from 2.1 to 4.1 in macs..
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //===============================================================

    //setting the glfw error callback defined above
    glfwSetErrorCallback(error_callback);


    GLFWwindow* window = glfwCreateWindow(640, 480, "Space Invaders", NULL, NULL);
    
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;

    GLenum err = glewInit();
    if(err!=GLEW_OK){//this needs to happen after the context is made
        fprintf(stderr, "Error initializing glew!\n");
        glfwTerminate();
        return -1;
    }else{
        std::cout<<"glew initialised correctly.."<<std::endl;
    }
    
    std::cout<<"gl version: "<<glGetString(GL_VERSION)<<std::endl;
    
   
    //imgui code
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window,true);
    ImGui_ImplOpenGL3_Init("#version 410");
    //========


    uint32_t clear_color = rgb_to_uint32(0, 128, 0);
    Buffer buffer;
    buffer.width  = buffer_width;
    buffer.height = buffer_height;
    buffer.data   = new uint32_t[buffer.width * buffer.height];
    buffer_clear(&buffer, clear_color);
  
  const char* vertex_shader =
    "\n"
    // "#version 330\n"
    "\n"
    "noperspective out vec2 TexCoord;\n"
    "\n"
    "void main(void){\n"
    "\n"
    "    TexCoord.x = (gl_VertexID == 2)? 2.0: 0.0;\n"
    "    TexCoord.y = (gl_VertexID == 1)? 2.0: 0.0;\n"
    "    \n"
    "    gl_Position = vec4(2.0 * TexCoord - 1.0, 0.0, 1.0);\n"
    "}\n";
const char* fragment_shader =
    "\n"
    // "#version 330\n"
    "\n"
    "uniform sampler2D buffer;\n"
    "noperspective in vec2 TexCoord;\n"
    "\n"
    "out vec3 outColor;\n"
    "\n"
    "void main(void){\n"
    "    outColor = texture(buffer, TexCoord).rgb;\n"
    "}\n";
   glClearColor(1.0, 0.0, 0.0, 1.0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //imgui initialization
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if(!io.WantCaptureMouse){
            //mouse not on gui
            
        }else{
            //mouse on gui
        }
  
      
        //imgui function
        ImGui::Begin("Space invaders");
        ImGui::Text("Menu loaded");
       
        ImGui::End();
        

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    
    //imgui wind up
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    std::cout<<"Quitting.."<<std::endl;
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

//#if defined DEBUG
//    std::cout<<"debug build"<<std::endl;
//#else
//    std::cout<<"release build"<<std::endl;
//    std::filesystem::current_path(Game::exeParentPath);//if it is release build, then we change the current working directory to the parent path of where the executable file is located
//    std::cout<<"the current working directory has been changed to the executable file path.."<<std::endl;
//#endif
