#pragma once
#include <queue>
class Mouse
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			LPress, LRelease, RPress, RRelease, WheelUp, WheelDown, Move, Invalid, Enter, Leave
		};
	private:
		Type type;
		bool leftPressed;
		bool rightPressed;
		int x, y;
	public:
		Event() noexcept : type(Type::Invalid), leftPressed(false), rightPressed(false), x(0), y(0) {};
		Event(Type type, const Mouse& parent) noexcept : type(type), leftPressed(parent.isLeftPressed), rightPressed(parent.isRightPressed), x(parent.x), y(parent.y) {}
		Type GetType() const noexcept
		{
			return type;
		}
		std::pair<int, int> Pos() const noexcept
		{
			return { x,y };
		}
		int GetPosX() const noexcept
		{
			return x;
		}
		int GetPosY() const noexcept
		{
			return y;
		}
		bool IsLeftPressed() const noexcept
		{
			return leftPressed;
		}
		bool IsRightPressed() const noexcept
		{
			return rightPressed;
		}
	};
public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator = (const Mouse&) = delete;
	//eventy
	std::pair<int, int>	Pos() const noexcept;
	bool IsLeftPressed() const noexcept;
	bool IsRightPressed() const noexcept;
	bool IsInWindow() const noexcept;
	int GetPosX() const noexcept;
	int GetPosY() const noexcept;
	Mouse::Event Read() noexcept;
	bool IsEmpty()
	{
		return buffer.empty();
	}
	void Flush() noexcept;
private:
	void OnMouseLeave() noexcept;
	void OnMouseEnter() noexcept;
	void OnMouseMove(int x, int y) noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void TrimBuffer() noexcept;
private:
	static constexpr unsigned int bufferSize = 16u;
	int x, y;
	bool isLeftPressed = false;
	bool isRightPressed = false;
	bool isInWindow = false;
	std::queue<Event> buffer;
};