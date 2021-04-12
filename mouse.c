#include <windows.h>
#include <winternl.h>
#include "mouse.h"
#pragma comment(lib, "ntdll.lib")

static HANDLE mouse_file_handle;
static IO_STATUS_BLOCK mouse_io_status; // neccessary for `NtCreateFile` and `NtDeviceIoControlFile` to succeed

static bool send_payload_to_mouse(MOUSE_IO* buffer) {
	return NtDeviceIoControlFile(mouse_file_handle, 0, 0, 0, &mouse_io_status, 0x2a2010, buffer, sizeof(MOUSE_IO), 0, 0) == 0L;
}

static NTSTATUS initialize_mouse(PCWSTR device_name) {
	UNICODE_STRING name;
	OBJECT_ATTRIBUTES attributes;

	RtlInitUnicodeString(&name, device_name);
	InitializeObjectAttributes(&attributes, &name, 0, NULL, NULL);

	NTSTATUS status = NtCreateFile(&mouse_file_handle, GENERIC_WRITE | SYNCHRONIZE, &attributes, &mouse_io_status, 0,
		FILE_ATTRIBUTE_NORMAL, 0, 3, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT, 0, 0);
	return status;
}

bool open_mouse() {
	NTSTATUS initialization_status = 0;

	if (mouse_file_handle == 0) {
		wchar_t locationBuffer1[] = L"\\??\\ROOT#SYSTEM#0002#{1abc05c0-c378-41b9-9cef-df1aba82b015}";
		initialization_status = initialize_mouse(locationBuffer1);

		if (!NT_SUCCESS(initialization_status)) {
			wchar_t locationBuffer2[] = L"\\??\\ROOT#SYSTEM#0001#{1abc05c0-c378-41b9-9cef-df1aba82b015}";
			initialization_status = initialize_mouse(locationBuffer2);
		}
	}

	return initialization_status == 0;
}

void close_mouse() {
	if (mouse_file_handle == 0) {
		return;
	}

	ZwClose(mouse_file_handle);
	mouse_file_handle = 0;
}

/*
 * Function:  send_mouse_message
 * --------------------
 * Manipulates the clicking, moving, and scrolling of mouse:
 *
 *  button: the button to press on the mouse. 0 for press out. 1 and above for other buttons.
 *  x: the distance to move the mouse horizontally. Must be between -127 and 127.
 *	   Positive corresponds to moving the mouse right and negative left.
 *  y: the distance to move the mouse vertically. Must be between -127 and 127.
 *	   Positive corresponds to moving the mouse down and negative up.
 *  wheel: the speed at which to scroll the wheel. Must be between -127 and 127.
 * 
 */
void send_mouse_message(char button, char x, char y, char wheel) {
	MOUSE_IO io = { .unk1 = 0, .button = button, .x = x, .y = y, .wheel = wheel };

	if (!send_payload_to_mouse(&io)) {
		close_mouse();
		open_mouse();
	}
}

void left_press_in_mouse() {
	send_mouse_message(1, 0, 0, 0);
}

void right_press_in_mouse() {
	send_mouse_message(2, 0, 0, 0);
}

void middle_press_in_mouse() {
	send_mouse_message(3, 0, 0, 0);
}

void press_out_mouse() {
	send_mouse_message(0, 0, 0, 0);
}

void move_mouse(char x, char y) {
	send_mouse_message(0, x, y, 0);
}

void move_wheel(char wheel) {
	send_mouse_message(0, 0, 0, wheel);
}

void button_press_in(char button) {
	send_mouse_message(button, 0, 0, 0);
}