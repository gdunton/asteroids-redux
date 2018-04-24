#pragma once

class IWindow
{
public:
	virtual ~IWindow() = default;
	virtual void Close() = 0;
	virtual void* GetWindowHandle() const = 0;
};
