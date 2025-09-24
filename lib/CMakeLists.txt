add_library(Wired SHARED
	wArgs.c
	wAssert.c
	wBuffer.c
	wError.c
	wEvent.c
	wFile.c
	wFont.c
	wGui.c
	wGuiButton.c
	wGuiCanvas.c
	wGuiGrid.c
	wGuiHBox.c
	wGuiImage.c
	wGuiLabel.c
	wGuiScroll.c
	wGuiVBox.c
	wImage.c
	wImagePNG.c
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
	wWrapFont.c
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
find_package(Freetype REQUIRED)

target_link_libraries(Wired PUBLIC lua5.4)
target_link_libraries(Wired PUBLIC SDL2::SDL2)
target_link_libraries(Wired PUBLIC OpenGL::GL)
target_link_libraries(Wired PUBLIC png)
target_link_libraries(Wired PUBLIC Freetype::Freetype)

add_executable(WiredEngine
	wMain.c
)

target_link_libraries(WiredEngine PUBLIC Wired)
