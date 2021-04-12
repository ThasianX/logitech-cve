#include <windows.h>
#include "mouse.h"
#include <stdio.h>

int main(void) {
	if (!open_mouse()) {
		printf("[-] failed to open ghub macro driver\n");
	}


	for (int i = 0; i < 32; i++) {
		Sleep(100);
		printf("[+] moving mouse\n");

		move_mouse(0, 10);
	}

	while (true) {}
}

