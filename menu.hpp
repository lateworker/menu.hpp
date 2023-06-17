#ifndef MENU_HPP
#define MENU_HPP
#include <windows.h>
#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
#include <sys/time.h>
COORD operator+ (const COORD &a, const COORD &b) {
	return {(SHORT)(a.X + b.X), (SHORT)(a.Y + b.Y)};
}
COORD operator- (const COORD &a, const COORD &b) {
	return {(SHORT)(a.X - b.X), (SHORT)(a.Y - b.Y)};
}
COORD operator* (const COORD &a, const SHORT &b) {
	return {(SHORT)(a.X * b), (SHORT)(a.Y * b)};
}
COORD operator/ (const COORD &a, const SHORT &b) {
	return {(SHORT)(a.X / b), (SHORT)(a.Y / b)};
}
COORD operator% (const COORD &a, const SHORT &b) {
	return {(SHORT)(a.X % b), (SHORT)(a.Y % b)};
}
void operator+= (COORD &a, const COORD &b) {
	a = {(SHORT)(a.X + b.X), (SHORT)(a.Y + b.Y)};
}
void operator-= (COORD &a, const COORD &b) {
	a = {(SHORT)(a.X - b.X), (SHORT)(a.Y - b.Y)};
}
void operator*= (COORD &a, const SHORT &b) {
	a = {(SHORT)(a.X * b), (SHORT)(a.Y * b)};
}
void operator/= (COORD &a, const SHORT &b) {
	a = {(SHORT)(a.X / b), (SHORT)(a.Y / b)};
}
void operator%= (COORD &a, const SHORT &b) {
	a = {(SHORT)(a.X % b), (SHORT)(a.Y % b)};
}
bool operator== (const COORD &a, const COORD &b) {
	return a.X == b.X and a.Y == b.Y;
}
bool operator!= (const COORD &a, const COORD &b) {
	return !operator==(a, b);
}
std::istream &operator<<(std::istream &pout, COORD &x) {
	scanf("%hd%hd", &x.X, &x.Y);
	return pout;
}
std::ostream &operator<<(std::ostream &pout, const COORD x) {
	printf("%hd %hd", x.X, x.Y);
	return pout;
}
long getTimeDifference(const struct timeval &start,  const struct timeval &end) {
	return (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
}
inline void setCursorVisible(bool visible = true) {
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CursorInfo);
	CursorInfo.bVisible = visible;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CursorInfo);
}
inline void setConsoleAttributes(bool QuickEditMode = true, bool InsertMode = true, bool MouseInput = true) {
	DWORD mode;
	GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);
	if (QuickEditMode) {
		mode |= ENABLE_QUICK_EDIT_MODE;
	} else {
		mode &= ~ENABLE_QUICK_EDIT_MODE;
	}
	if (InsertMode) {
		mode |= ENABLE_INSERT_MODE;
	} else {
		mode &= ~ENABLE_INSERT_MODE;
	}
	if (MouseInput) {
		mode |= ENABLE_MOUSE_INPUT;
	} else {
		mode &= ~ENABLE_MOUSE_INPUT;
	}
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode);
}
inline void getCursorPosition(COORD &cursorPosition) {
	CONSOLE_SCREEN_BUFFER_INFO temp;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &temp);
	cursorPosition = temp.dwCursorPosition;
}
inline void setCursorPosition(const COORD &cursorPosition = {0, 0}) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}
inline void print(std::string text, COORD position) {
	COORD position_now;
	getCursorPosition(position_now);
	setCursorPosition(position);
	printf("%s", text.c_str());
	setCursorPosition(position_now);
}
inline bool getMouseEvent(MOUSE_EVENT_RECORD &mouse_record) {
	INPUT_RECORD record;
	DWORD temp;
	ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &record, 1, &temp);
	if (record.EventType == MOUSE_EVENT) {
		mouse_record = record.Event.MouseEvent;
		return false;
	}
	return true;
}
struct {
	const WORD Black = 0;
	const WORD Blue = 1;
	const WORD Green = 2;
	const WORD Cyan = 3;
	const WORD Red = 4;
	const WORD Purple = 5;
	const WORD Yellow = 6;
	const WORD White = 7;
	const WORD brightBlack = 8;
	const WORD brightBlue = 9;
	const WORD brightGreen = 10;
	const WORD brightCyan = 11;
	const WORD brightRed = 12;
	const WORD brightPurple = 13;
	const WORD brightYellow = 14;
	const WORD brightWhite = 15;
} consoleColor;
inline WORD mixConsoleColor(const WORD &foregroundColor = 7, const WORD &backgroundColor = 0) {
	return backgroundColor << 4 | foregroundColor;
}
class ButtonColor {
	public:
		WORD highlightColor;
		WORD defaultColor;
		ButtonColor() {
			highlightColor = consoleColor.brightCyan;
			defaultColor = consoleColor.White;
		}
		inline void set(WORD button_highlight_color, WORD button_default_color = consoleColor.White) {
			highlightColor = button_highlight_color;
			defaultColor = button_default_color;
		}
		inline void toDefaultColor() {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), defaultColor);
		}
		inline void toHighlightColor() {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), highlightColor);
		}
};
bool operator== (const ButtonColor &a, const ButtonColor &b) {
	return a.highlightColor == b.highlightColor and a.defaultColor == b.defaultColor;
}
class Button {
	public:
		COORD position;
		std::string text;
		std::string event;
		ButtonColor color;
		bool clickable;
		bool visible;
		Button() {
			position = {0, 0};
			text = "";
			event = "";
			color.set(consoleColor.brightCyan, consoleColor.White);
			clickable = true;
			visible = true;
		}
		bool operator== (const Button &button) const {
			return position == button.position and text == button.text and event == button.event and color == button.color and clickable == button.clickable and visible == button.visible;
		}
		bool operator== (const COORD &mousePosition) const {
			if (clickable == false or visible == false) return false;
			if (position.Y == mousePosition.Y and position.X <= mousePosition.X and mousePosition.X <= position.X + (SHORT)text.length() - 1) {
				return true;
			}
			return false;
		}
		bool operator!= (const COORD &mousePosition) const {
			return !operator==(mousePosition);
		}
};
inline void printButton(Button &button, COORD offset = {0, 0}) {
	if (button.visible) {
		print(button.text, button.position + offset);
	}
}
std::ostream &operator<<(std::ostream &pout, Button x) {
	printButton(x);
	return pout;
}
bool cmp_button(const Button &a, const Button &b) {
	if (a.position.Y != b.position.Y) return a.position.Y < b.position.Y;
	return a.position.X < b.position.X;
}
class Menu {
	protected:
		struct History {
			History() {
				color = consoleColor.White;
				visible = true;
				position = {-1, -1};
				offset = {0, 0};
				text = "";
			}
			WORD color;
			bool visible;
			COORD position;
			COORD offset;
			std::string text;
			bool operator== (const Button &o) const {
				return visible == o.visible and position == o.position and text == o.text;
			}
			bool operator!= (const Button &o) const {
				return !operator==(o);
			}
		};
	public:
		struct timeval time_start;
		std::vector<Button> buttonData;
		std::vector<COORD> buttonOffset;
		std::vector<History> buttonHistory;
		COORD position;
		long timeLimit;

		Menu() {
			buttonData.clear();
			buttonOffset.clear();
			buttonHistory.clear();
			position = {0, 0};
			timeLimit = -1;
		}
		inline size_t size() {
			return buttonData.size();
		}
		inline size_t find(Button button) {
			size_t l = std::lower_bound(buttonData.begin(), buttonData.end(), button, cmp_button) - buttonData.begin();
			size_t r = std::upper_bound(buttonData.begin(), buttonData.end(), button, cmp_button) - buttonData.begin();
			for (size_t i = l; i < r; ++i) {
				if (buttonData[i] == button)
					return i;
			}
			return buttonData.size();
		}
		inline size_t findinText(std::string buttonText) {
			for (size_t i = 0; i < buttonData.size(); ++i) {
				if (buttonData[i].text == buttonText)
					return i;
			}
			return buttonData.size();
		}
		inline size_t findinEvent(std::string buttonEvent) {
			for (size_t i = 0; i < buttonData.size(); ++i) {
				if (buttonData[i].event == buttonEvent)
					return i;
			}
			return buttonData.size();
		}
		inline bool reloadinEvent(Button button) {
			size_t pos = findinEvent(button.event);
			if (pos == buttonData.size()) return true;
			buttonData[pos] = button;
			return false;
		}
		inline bool fold(size_t pos, size_t length) {
			if (pos + length > size()) return true;
			for (size_t i = 0; i < length; ++i) {
				buttonData[pos + i].visible = false;
			}
			COORD offset = {0, SHORT(buttonData[pos].position.Y - buttonData[pos + length].position.Y)};
			for (size_t i = pos; i < buttonData.size(); ++i) {
				buttonOffset[i] += offset;
			}
			return false;
		}
		inline bool unfold(size_t pos, size_t length) {
			if (pos + length > size()) return true;
			for (size_t i = 0; i < length; ++i) {
				buttonData[pos + i].visible = true;
			}
			COORD offset = {0, SHORT(buttonData[pos].position.Y - buttonData[pos + length].position.Y)};
			for (size_t i = pos; i < buttonData.size(); ++i) {
				buttonOffset[i] -= offset;
			}
			return false;
		}
		inline size_t push(Button button) {
			if (buttonData.size() == 0) {
				buttonData.push_back(button);
				buttonOffset.push_back({0, 0});
				buttonHistory.push_back(History());
			} else {
				size_t pos = std::upper_bound(buttonData.begin(), buttonData.end(), button, cmp_button) - buttonData.begin();
				buttonData.insert(buttonData.begin() + pos, button);
				buttonOffset.insert(buttonOffset.begin() + pos, {0, 0});
				buttonHistory.insert(buttonHistory.begin() + pos, History());
			}
			return buttonData.size();
		}
		inline bool erase(size_t pos) {
			if (pos != buttonData.size()) {
				buttonData.erase(buttonData.begin() + pos);
				buttonOffset.erase(buttonOffset.begin() + pos);
				buttonHistory.erase(buttonHistory.begin() + pos);
				return false;
			}
			return true;
		}
		inline bool erase(Button button) {
			return erase(find(button));
		}
		inline bool eraseinText(std::string text) {
			return erase(findinText(text));
		}
		inline bool eraseinEvent(std::string event) {
			return erase(findinEvent(event));
		}
		inline Menu &clear() {
			buttonData.clear();
			buttonOffset.clear();
			buttonHistory.clear();
			return *this;
		}
		inline void start();
		inline void stop();
		inline void cls();
};
#define mouseLeftButton FROM_LEFT_1ST_BUTTON_PRESSED	// 左键
#define mouseMiddleButton FROM_LEFT_2ND_BUTTON_PRESSED	// 中键
#define mouseRightButton RIGHTMOST_BUTTON_PRESSED		// 右键
#define mouseClick 0x0					// 鼠标按键按下或松开时触发
#define mouseMove MOUSE_MOVED			// 鼠标移动时触发
#define mouseDoubleClick DOUBLE_CLICK	// 鼠标第二次按下时触发，触发此事件前一定会触发 mouseClick 事件
#define mouseWheel MOUSE_WHEELED		// 鼠标滚轮滚动时触发
inline bool checkColor(Button &button, COORD hangPosition, WORD &historyColor) {
	return (button == hangPosition and historyColor == button.color.highlightColor) or (button != hangPosition and historyColor == button.color.defaultColor);
}
inline void printMenu(Menu &menu, COORD hangPosition = {-1, -1}) {
	if (menu.size() == 0) return;
	for (size_t i = 0; i < menu.buttonData.size(); ++i) {
		if (checkColor(menu.buttonData[i], hangPosition - menu.buttonOffset[i], menu.buttonHistory[i].color) and menu.buttonHistory[i] == menu.buttonData[i] and menu.buttonHistory[i].offset == menu.buttonOffset[i]) continue;
		if (menu.buttonHistory[i].visible) {
			menu.buttonData[i].color.toDefaultColor();
			print(std::string(menu.buttonHistory[i].text.length(), ' '), menu.buttonHistory[i].position + menu.buttonHistory[i].offset + menu.position);
		}
		if (menu.buttonData[i].visible) {
			if (menu.buttonData[i] == hangPosition - menu.buttonOffset[i]) {
				menu.buttonData[i].color.toHighlightColor();
				menu.buttonHistory[i].color = menu.buttonData[i].color.highlightColor;
			} else {
				menu.buttonData[i].color.toDefaultColor();
				menu.buttonHistory[i].color = menu.buttonData[i].color.defaultColor;
			}
			printButton(menu.buttonData[i], menu.position + menu.buttonOffset[i]);
//			menu.buttonData[i].color.toDefaultColor();
		}
		if (menu.buttonHistory[i] != menu.buttonData[i] or menu.buttonHistory[i].offset != menu.buttonOffset[i]) {
			menu.buttonHistory[i].offset = menu.buttonOffset[i];
			menu.buttonHistory[i].position = menu.buttonData[i].position;
			menu.buttonHistory[i].visible = menu.buttonData[i].visible;
			menu.buttonHistory[i].text = menu.buttonData[i].text;
		}
	}
}
inline void Menu::start() {
	gettimeofday(&time_start, nullptr);
	setConsoleAttributes(false, false, true);
	setCursorVisible(false);
	for (size_t i = 0; i < buttonData.size(); ++i) {
		buttonHistory[i].visible = !buttonData[i].visible;
	}
	printMenu(*this);
}
inline void Menu::stop() {
	printMenu(*this);
	short maxY = -1;
	for (size_t i = 0; i < buttonData.size(); ++i) {
		if (buttonData[i].visible)
			maxY = std::max(maxY, short(buttonData[i].position.Y + buttonOffset[i].Y + position.Y));
	}
	setCursorPosition({0, (short)(maxY + 1)});
	setCursorVisible(true);
	setConsoleAttributes(true, true, true);
}
inline void Menu::cls() {
	if (size() == 0) return;
	for (size_t i = 0; i < buttonData.size(); ++i) {
		buttonData[i].color.toDefaultColor();
		print(std::string(buttonHistory[i].text.length(), ' '), buttonHistory[i].position + buttonHistory[i].offset + position);
	}
	setCursorPosition(buttonData[0].position + position);
}
inline bool runMenu(Menu &menu, std::string &event) {
	event = "";
	if (menu.size() == 0) return true;
	MOUSE_EVENT_RECORD mouseEvent;
	struct timeval time_now;
	gettimeofday(&time_now, nullptr);
	bool res = true;
	if (menu.timeLimit == -1 or getTimeDifference(menu.time_start, time_now) < menu.timeLimit) {
		res = false;
		if (!getMouseEvent(mouseEvent)) {
			switch (mouseEvent.dwEventFlags) {
				case mouseMove: {
					printMenu(menu, mouseEvent.dwMousePosition - menu.position);
					break;
				}
				case mouseClick: {
					if (mouseEvent.dwButtonState and mouseEvent.dwButtonState != mouseWheel) {
						std::string res = "";
						for (size_t i = 0; i < menu.buttonData.size(); ++i) {
							if (menu.buttonData[i] == mouseEvent.dwMousePosition - menu.buttonOffset[i] - menu.position) {
								printButton(menu.buttonData[i], menu.buttonOffset[i] + menu.position);
								event = menu.buttonData[i].event;
							}
						}
					}
					break;
				}
			}
		}
	}
	setCursorPosition({0, 0});
	Sleep(1);
	return res;
}
std::ostream &operator<<(std::ostream &pout, Menu x) {
	printMenu(x);
	return pout;
}
#endif
