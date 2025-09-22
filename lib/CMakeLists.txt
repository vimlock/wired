add_library(Wired SHARED
	wArgs.c
	wAssert.c
	wBuffer.c
	wError.c
	wFile.c
	wGui.c
	wImage.c
	wLog.c
	wMath.c
	wMemory.c
	wPainter.c
	wPlatform.c
	wPlatformGL.c
	wPlatformSDL.c
	wScript.c
	wShader.c
	wSprite.c
	wString.c
	wTexture.c
	wWindow.c

	wWrapBuffer.c
	wWrapFile.c
	wWrapGui.c
	wWrapImage.c
	wWrapLog.c
	wWrapMath.c
	wWrapPainter.c
	wWrapShader.c
	wWrapSprite.c
	wWrapString.c
	wWrapTexture.c
	wWrapWindow.c
)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(Wired PUBLIC lua5.3)
target_link_libraries(Wired PUBLIC SDL2::SDL2)
target_link_libraries(Wired PUBLIC OpenGL::GL)

add_executable(WiredEngine
	wMain.c
)

target_link_libraries(WiredEngine PUBLIC Wired)