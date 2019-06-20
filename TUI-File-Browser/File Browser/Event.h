#ifndef __Event_GUARD__
#define __Event_GUARD__

#include <Windows.h>
#include <string>
#include <vector>


class Event
{
	// -------- CLASS MEMBERS --------
private:
	INPUT_RECORD	ir_;

	// -------- CONSTRUCTOR --------
public:
	Event();
	Event(INPUT_RECORD const ir) : ir_(ir) { };

	// -------- DEPENDENCY CLASSES --------
public:
	enum class EventType
	{
		KEY = KEY_EVENT,
		MOUSE = MOUSE_EVENT
	};

	class Keyboard
	{
		// -------- FRIEND STATUS --------
		friend Event;

		// -------- CLASS MEMBERS --------
	private:
		KEY_EVENT_RECORD const&	ker_;

		// -------- CONSTRUCTOR --------
	private:
		Keyboard(INPUT_RECORD const& ir) : ker_(ir.Event.KeyEvent) { };

		// -------- OPERATIONS --------
	public:
		bool KeyDown() const { return ker_.bKeyDown ? true : false; }
		char AsciiChar() const { return ker_.uChar.AsciiChar; }
		unsigned short VirtualKeyCode() const { return ker_.wVirtualKeyCode; }
	};
	class Mouse
	{
		// -------- FRIEND STATUS --------
		friend Event;

		// --------- DEPENDENCY CLASS --------
	public:
		enum class MouseType
		{
			WHEELED = MOUSE_WHEELED,
			BUTTON = 0
		};

		// -------- CLASS MEMBERS --------
	private:
		MOUSE_EVENT_RECORD const&	mer_;

		// -------- CONSTRUCTOR --------
	private:
		Mouse(INPUT_RECORD const& ir) : mer_(ir.Event.MouseEvent) { };

		// -------- OPERATIONS --------
	public:
		bool LeftPressed() const { return mer_.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED ? true : false; }
		bool MouseWheelDown() const {
			short state = HIWORD(mer_.dwButtonState);
			return state < 0;
		}
		bool MouseWheelUp() const {
			short state = HIWORD(mer_.dwButtonState);
			return state > 0;
		}

		COORD MousePosition() const { return mer_.dwMousePosition; }

		MouseType GetType() const { return MouseType(mer_.dwEventFlags); }
	};

	// -------- ACCESSORS --------
public:
	EventType GetType() const { return EventType(ir_.EventType); }

	Keyboard GetKeyboardEvent() const { return Keyboard(ir_); }
	Mouse GetMouseEvent() const { return Mouse(ir_); }
};
#endif