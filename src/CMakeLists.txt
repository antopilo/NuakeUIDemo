add_compile_definitions(GLFW_INCLUDE_NONE)

add_executable(NuakeUIDemo 
main.cpp MyInputManager.h MyInputManager.cpp SliderNode.h TextInput.h stb_image.h)

target_compile_features(NuakeUIDemo PRIVATE cxx_std_17)
target_link_libraries(NuakeUIDemo PUBLIC NuakeUI)



