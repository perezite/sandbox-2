#pragma once

#include "Vector2.h"
#include "Window.h"
#include "SDL.h"
#include <set>

namespace sb 
{
	enum class KeyCode;

	class Input
	{
	public:
		static void update();

		inline static bool hasQuitEvent() { return m_hasQuitEvent; }

		inline static bool isKeyDown(const KeyCode key) { return m_keysDown.count(key) > 0; }
		
		inline static bool isKeyGoingDown(const KeyCode key) { return m_keysGoingDown.count(key) > 0; }

		inline static bool isTouchDown(const int index) { return m_touchesDown.count(index) > 0; }

		inline static bool isTouchGoingDown(const int index) { return m_touchesGoingDown.count(index) > 0; }

		static const sb::Vector2f getTouchPosition(const sb::Window& window);

	protected:
		static const sb::Vector2f getTouchPixelPosition(const sb::Window& window);

	private:
		static bool m_hasQuitEvent;

		static std::set<KeyCode> m_keysDown;

		static std::set<KeyCode> m_keysGoingDown;

		static std::set<SDL_FingerID> m_touchesDown;

		static std::set<SDL_FingerID> m_touchesGoingDown;

		static sb::Vector2f m_mousePosition;

		static sb::Vector2f m_fingerPosition;

		static bool m_insideWindow;
	};

	enum class KeyCode
	{
		Unknown = SDLK_UNKNOWN,
		Backspace = SDLK_BACKSPACE,
		Tab = SDLK_TAB,
		Clear = SDLK_CLEAR,
		Return = SDLK_RETURN,
		Pause = SDLK_PAUSE,
		Escape = SDLK_ESCAPE,
		Space = SDLK_SPACE,
		Exclaim = SDLK_EXCLAIM,
		Doublequote = SDLK_QUOTEDBL,
		Hash = SDLK_HASH,
		Dollar = SDLK_DOLLAR,
		Ampersand = SDLK_AMPERSAND,
		Quote = SDLK_QUOTE,
		Leftparen = SDLK_LEFTPAREN,
		Rightparen = SDLK_RIGHTPAREN,
		Asterisk = SDLK_ASTERISK,
		Plus = SDLK_PLUS,
		Comma = SDLK_COMMA,
		Minus = SDLK_MINUS,
		Period = SDLK_PERIOD,
		Slash = SDLK_SLASH,
		Zero = SDLK_0,
		One = SDLK_1,
		Two = SDLK_2,
		Three = SDLK_3,
		Four = SDLK_4,
		Five = SDLK_5,
		Six = SDLK_6,
		Seven = SDLK_7,
		Eight = SDLK_8,
		Nine = SDLK_9,
		Colon = SDLK_COLON,
		Semicolon = SDLK_SEMICOLON,
		Less = SDLK_LESS,
		Equals = SDLK_EQUALS,
		Greater = SDLK_GREATER,
		Question = SDLK_QUESTION,
		At = SDLK_AT,
		LeftBracket = SDLK_LEFTBRACKET,
		Backslash = SDLK_BACKSLASH,
		Rightbracket = SDLK_RIGHTBRACKET,
		Caret = SDLK_CARET,
		Underscore = SDLK_UNDERSCORE,
		Backquote = SDLK_BACKQUOTE,
		a = SDLK_a,
		b = SDLK_b,
		c = SDLK_c,
		d = SDLK_d,
		e = SDLK_e,
		f = SDLK_f,
		g = SDLK_g,
		h = SDLK_h,
		i = SDLK_i,
		j = SDLK_j,
		k = SDLK_k,
		l = SDLK_l,
		m = SDLK_m,
		n = SDLK_n,
		o = SDLK_o,
		p = SDLK_p,
		q = SDLK_q,
		r = SDLK_r,
		s = SDLK_s,
		t = SDLK_t,
		u = SDLK_u,
		v = SDLK_v,
		w = SDLK_w,
		x = SDLK_x,
		y = SDLK_y,
		z = SDLK_z,
		Delete = SDLK_DELETE,
		KeypadPeriod = SDLK_KP_PERIOD,
		KeypadDivide = SDLK_KP_DIVIDE,
		KeypadMultiply = SDLK_KP_MULTIPLY,
		KeypadMinus = SDLK_KP_MINUS,
		KeypadPlus = SDLK_KP_PLUS,
		KeypadEnter = SDLK_KP_ENTER,
		KeypadEquals = SDLK_KP_EQUALS,
		Up = SDLK_UP,
		Down = SDLK_DOWN,
		Right = SDLK_RIGHT,
		Left = SDLK_LEFT,
		Insert = SDLK_INSERT,
		Home = SDLK_HOME,
		End = SDLK_END,
		PageUp = SDLK_PAGEUP,
		PageDown = SDLK_PAGEDOWN,
		F1 = SDLK_F1,
		F2 = SDLK_F2,
		F3 = SDLK_F3,
		F4 = SDLK_F4,
		F5 = SDLK_F5,
		F6 = SDLK_F6,
		F7 = SDLK_F7,
		F8 = SDLK_F8,
		F9 = SDLK_F9,
		F10 = SDLK_F10,
		F11 = SDLK_F11,
		F12 = SDLK_F12,
		F13 = SDLK_F13,
		F14 = SDLK_F14,
		F15 = SDLK_F15,
		CapsLock = SDLK_CAPSLOCK,
		RightShift = SDLK_RSHIFT,
		LeftShift = SDLK_LSHIFT,
		RightControl = SDLK_RCTRL,
		LeftControl = SDLK_LCTRL,
		RightAlt = SDLK_RALT,
		LeftAlt = SDLK_LALT,
		Mode = SDLK_MODE,
		Help = SDLK_HELP,
		SysReq = SDLK_SYSREQ,
		Menu = SDLK_MENU,
		Power = SDLK_POWER,
		Undo = SDLK_UNDO
	};
}