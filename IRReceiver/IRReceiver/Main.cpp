#include <string>
#define NOMINMAX
#include <Windows.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

// Command mapping
const char* IR_PLAY_PAUSE = "FFC23D";
const char* IR_NEXT = "FF02FD";
const char* IR_PREV = "FF22DD";
const char* IR_VOLUME_UP = "FFA857";
const char* IR_VOLUME_DOWN = "FFE01F";
const char* IR_LOCK = "FF629D";
const char* IR_REPEAT = "FFFFFFFF";


std::vector<std::string> split_string(const std::string& str, char delim)
{
	std::stringstream ss(str);
	std::vector<std::string> parts;
	for (std::string elem; std::getline(ss, elem, delim); parts.push_back(elem));
	return parts;
}

void send_input(int keycode)
{
	INPUT input = { 0 };
	input.ki.wVk = keycode;
	input.type = INPUT_KEYBOARD;
	SendInput(1, &input, sizeof(INPUT));
	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));
}

void handle_command(const std::string& command, std::string& prev_command)
{
	if (command == IR_REPEAT)
	{
		if (prev_command == IR_VOLUME_UP || prev_command == IR_VOLUME_DOWN)
		{
			handle_command(prev_command, prev_command);
		}
	}
	if (command == IR_PLAY_PAUSE)
	{
		prev_command = "";
		send_input(VK_MEDIA_PLAY_PAUSE);
	}
	if (command == IR_NEXT)
	{
		prev_command = "";
		send_input(VK_MEDIA_NEXT_TRACK);
	}
	if (command == IR_PREV)
	{
		prev_command = "";
		send_input(VK_MEDIA_PREV_TRACK);
	}
	if (command == IR_VOLUME_UP)
	{
		prev_command = IR_VOLUME_UP;
		send_input(VK_VOLUME_UP);
	}
	if (command == IR_VOLUME_DOWN)
	{
		prev_command = IR_VOLUME_DOWN;
		send_input(VK_VOLUME_DOWN);
	}
	if (command == IR_LOCK)
	{
		prev_command = "";
		LockWorkStation();
	}
}

char read_byte(HANDLE file)
{
	char data_byte = '\0';
	DWORD bytes_read;
	if (!ReadFile(file, &data_byte, 1, nullptr, nullptr))
	{
		std::cerr << "Could not read file";
		return EXIT_FAILURE;
	}
	return data_byte;
}

std::string read_until_newline(HANDLE file)
{
	std::string data;
	while (true)
	{
		char b = read_byte(file);
		if (b == '\0')
		{
			continue;
		}
		if (b == '\r')
		{
			continue;
		}
		else if (b == '\n')
		{
			return data;
		}
		data += b;
	}
}

int main()
{
	std::string port = "COM3";
	int baud = 9600;

	auto handlePort = CreateFileA(port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (handlePort == INVALID_HANDLE_VALUE)
	{
		std::cerr << "Error opening port";
		return EXIT_FAILURE;
	}
	
	DCB dcb = { 0 };
	dcb.DCBlength = sizeof(DCB);

	if (!GetCommState(handlePort, &dcb))
	{
		std::cerr << "Cant get comm state";
		return EXIT_FAILURE;
	}

	dcb.BaudRate = baud;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;

	if (!SetCommState(handlePort, &dcb))
	{
		std::cerr << "Cant set comm state";
		return EXIT_FAILURE;
	}

	COMMTIMEOUTS comtimeouts = { 0 };
	comtimeouts.ReadIntervalTimeout = 3;
	comtimeouts.ReadTotalTimeoutMultiplier = 3;
	comtimeouts.ReadTotalTimeoutConstant = 2;
	comtimeouts.WriteTotalTimeoutMultiplier = 3;
	comtimeouts.WriteTotalTimeoutMultiplier = 2;
	if (!SetCommTimeouts(handlePort, &comtimeouts))
	{
		std::cerr << "Cant set com timeouts";
		return EXIT_FAILURE;
	}

	std::string prev_command = "";
	while (true)
	{
		std::string command = read_until_newline(handlePort);
		handle_command(command, prev_command);
	}
}