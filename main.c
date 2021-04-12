#include <windows.h>
#include "mouse.h"
#include <stdio.h>

int main(void) {
	if (!open_mouse()) {
		printf("[-] failed to open ghub macro driver\n");
	}

	move_mouse(-535, -190);

	while (true) {}
}

