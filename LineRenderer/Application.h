#pragma once
#include "Maths.h"
class LineRenderer;

class Application
{
public:
	virtual void Update(float delta) = 0;
	virtual ~Application() = default;
	bool leftMouseDown;
	bool rightMouseDown;
	Vec2 cursorPos;
	LineRenderer* lines = nullptr;

	virtual void OnLeftClick() {}
	virtual void OnLeftRelease() {}
	virtual void OnRightClick() {}
	virtual void OnRightRelease() {}
	virtual void OnMiddleClick() {}
	virtual void OnMiddleRelease() {}

	virtual inline const char* GetWindowTitle() const
	{
		return "Override GetWindowTitle() to give your program a name!";
	}
};