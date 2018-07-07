#ifndef IWINDOW_H
#define IWINDOW_H

class IWindow
{
public:
	IWindow() = default;
	virtual ~IWindow() = default;
	virtual void Close() = 0;
	virtual void* GetWindowHandle() const = 0;

	IWindow(const IWindow& other) = default;
	IWindow(IWindow&& other) noexcept = default;
	IWindow& operator=(const IWindow& other) = default;
	IWindow& operator=(IWindow&& other) noexcept = default;
};

#endif