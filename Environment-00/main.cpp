#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <Eigen/Core>



#include <iostream>

int main() {


    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::cout << extensionCount << " extensions supported\n";

    auto matrix = Eigen::Matrix4d::Ones();
    auto vec = Eigen::Vector4d::Ones();
    Eigen::Vector4d test = matrix * vec;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}