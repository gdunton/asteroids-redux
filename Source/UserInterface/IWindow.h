#ifndef IWINDOW_H
#define IWINDOW_H

class IWindow
{
public:
	virtual ~IWindow() = default;
	virtual void Close() = 0;
	virtual void* GetWindowHandle() const = 0;
};

#endif