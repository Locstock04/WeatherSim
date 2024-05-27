#include "Example.h"
#include "LineRenderer.h"
#include "imgui.h"
#include "TextStream.h"

Example::Example()
{
	//Initialise or otherwise set up your program here.
}

void Example::Update(float delta)
{
	//Everything that your program does every frame should go here.
	//This includes rendering done with the line renderer!

	TextStream testInfo(lines, cursorPos, 0.2f);
	testInfo.SetDecimalPlaces(1);
	testInfo << cursorPos << " is where the " << Colour::RED << "cursor" << Colour::WHITE << " is!";

}

void Example::OnLeftClick()
{
	//This is an example of the thing 
}
