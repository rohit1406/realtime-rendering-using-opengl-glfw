//compile and link - for native windowing
cl.exe /EHsc Window.cpp /link user32.lib gdi32.lib /SUBSYSTEM:WINDOWS

//seperate compile and link
cl.exe /c /EHsc Window.cpp
link.exe Window.obj user32.lib gdi32.lib kernel32.lib

//compile and link with resource file
rc my_icon.ico
cl.exe /c /EHsc Window.cpp
link.exe Window.obj user32.lib gdi32.lib kernel32.lib my_icon_resource.res
link.exe Window.obj user32.lib gdi32.lib kernel32.lib opengl32.lib my_icon_resource.res

/*
/EH specifies the kind of exception handling used by the compiler, when to optimize away the exception checks
and whether to destroy c++ objects that goes out of scope because of an exception.

if EH is not specified - compiler catches both asynchronous structured exceptions and c++ exceptions, but does not destroy
c++ objects that goes out of scope because of an asynchronous exception

a - exception handling model that catches both asynchronous(structured) and synchronous(c++) exceptions
s - catches c++ exceptions only and tells the compiler that extern "C" may throw an exception
c - if used with s, catches c++ exceptions only and tells extern "C" never throws c++ exception
*/


/*****************************FREEGLUT******************/
cl.exe /c /EHsc /I C:\freeglut\include Window.cpp
link.exe Window.obj /LIBPATH:C:\freeglut\lib freeglut.lib /SUBSYSTEM:CONSOLE glut_icons.res
/******************************FREEGLUT END**********************/

/***************************GLFW*************************/
cl.exe /MD /c /EHsc /I Include Window.cpp glad.c

/MD - Causes the application to use the multithread-specific and DLL-specific version of the run-time library
you can also use - #define GLFW_DLL

link.exe Window.obj glad.obj /LIBPATH:Libs GLFW/glfw3.lib user32.lib gdi32.lib kernel32.lib shell32.lib opengl32.lib /NODEFAULTLIB:msvcrtd.lib

//using glfw3
link.exe Window.obj glad.obj /LIBPATH:Libs GLFW/glfw3.lib user32.lib gdi32.lib kernel32.lib shell32.lib opengl32.lib /SUBSYSTEM:CONSOLE /NODEFAULTLIB:msvcrtd.lib

//using assimp
link.exe Window.obj glad.obj /LIBPATH:Libs GLFW/glfw3.lib user32.lib gdi32.lib kernel32.lib shell32.lib opengl32.lib assimp/assimp-vc140-mt.lib /NODEFAULTLIB:msvcrtd.lib

//using glfw3 and freetype
link.exe Window.obj glad.obj /LIBPATH:Libs GLFW/glfw3.lib freetype/freetyped.lib user32.lib gdi32.lib kernel32.lib shell32.lib opengl32.lib /SUBSYSTEM:CONSOLE /NODEFAULTLIB:msvcrtd.lib


//break out
cl.exe /MD /c /EHsc program.cpp game_setting_up.cpp resource_manager.cpp shader.cpp texture.cpp sprite_renderer.cpp game_object.cpp game_level.cpp ball_object.cpp particle_generator.cpp

link.exe program.obj game_setting_up.obj resource_manager.obj shader.obj texture.obj sprite_renderer.obj game_object.obj game_level.obj ball_object.obj particle_generator.obj /LIBPATH:lib opengl32.lib GLFW/glfw3.lib GL/glew32.lib user32.lib gdi32.lib kernel32.lib shell32.lib SOIL.lib /NODEFAULTLIB:msvcrtd.lib
/***************************GLFW ENDS********************/