# menu.hpp
一个用来在控制台内实现gui和鼠标交互的c++库
现在处于测试阶段
# 简单的示例代码
```c++
#include "menu.hpp"
#include <bits/stdc++.h>
using namespace std;
int main() {
	Button tmp;
	Menu menu;
	for(int i=0;i<6;++i) {
		tmp.event = tmp.text = string(6, 'a'+i);
		tmp.position = {0, i};
		menu.push(tmp);
	}
//	menu.timeLimit = 5000;
	menu.start();
	int tot = 0;
	string event;
	while(tot < 6) {
		if(runMenu(menu, event)) break;
		if(event != "") {
			menu.stop();
			menu.cls();
			cout<<event<<"\n";
			system("pause");
			system("cls");
			menu.start();
			++tot;
			menu.buttonData[menu.findinEvent(event)].visible = false;
		}
		Sleep(50);
	}
	menu.stop();
	return 0;
}
```
