#include "pch.h"
#include <utility>
#include <limits.h>
#include "Laba6DLL.h"
#include "windows.h"

HANDLE hThread; //���������� ��� ������ � �������

Turn::Turn()
{
	heaphandle = GetProcessHeap();
	size = 0;
	buffer = 0;
	mass = (int*)HeapAlloc(heaphandle, HEAP_ZERO_MEMORY, size * sizeof(int));
}

void Turn::PushToBuf(int value)
{
	HANDLE turnBufF = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"turnBufF");
	HANDLE turnBufE = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"turnBufE");
	WaitForSingleObject(turnBufE, INFINITE);
	buffer = value;
	SetEvent(turnBufF);
}

DWORD WINAPI FunctionWithThread(PVOID p)     //������� ������������ � ���� �������� ������
{
	Turn& turn = *((Turn*)p);

	HANDLE stopTurnThread = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"stopTurnThread");
	HANDLE turnBufF = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"turnBufF");
	HANDLE turnBufE = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"turnBufE");

	while (WaitForSingleObject(stopTurnThread, 0) != WAIT_OBJECT_0) {
		WaitForSingleObject(turnBufF, INFINITE);

		if (WaitForSingleObject(stopTurnThread, 0) != WAIT_OBJECT_0) {
			turn.Push(turn.buffer);
			turn.WriteToFile();
		}
		SetEvent(turnBufE);
	}
	SetEvent(turnBufF);
	return 0;
}
Turn::Turn(int i)
{
	heaphandle = GetProcessHeap();
	size = 0;
	buffer = 0;
	mass = (int*)HeapAlloc(heaphandle, HEAP_ZERO_MEMORY, size * sizeof(int));
	HANDLE fileTo = CreateFile(L"file.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	CloseHandle(fileTo);
	DWORD id;
	hThread = CreateThread(NULL, 0, FunctionWithThread, this, 0, &id);
}

Turn::Turn(const Turn& turn)
{
	heaphandle = GetProcessHeap();
	size = turn.size;
	mass = (int*)HeapAlloc(heaphandle, NULL, size * sizeof(int));
	for (int i = 0; i < size; i++)
	{
		mass[i] = turn.mass[i];
	}
}

Turn Turn::ReadFromFile ()
{
	Turn turn;
	DWORD dwCounter, dwTemp;
	HANDLE hFile = CreateFile(L"file.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwEOF = SetFilePointer(hFile, 0, 0, FILE_END);
	SetFilePointer(hFile, 0, 0, FILE_BEGIN);
	while (SetFilePointer(hFile, 0, 0, FILE_CURRENT) < dwEOF)
	{
		ReadFile(hFile, &dwCounter, sizeof(dwCounter), &dwTemp, NULL);
		turn.Push(dwCounter);
	}
	CloseHandle(hFile);

	return turn;
}


int Turn::EjectionElemet() // �������� 1 ��������
{
	//�������� ������ ������ � ����������� � �����
	if (size == 0)
	{
		return NULL;
	}
	else
	{
		HANDLE heap = GetProcessHeap();
		int newsize = size - 1;
		int* mass1 = (int*)HeapAlloc(heaphandle, HEAP_ZERO_MEMORY, newsize * sizeof(int));
		int first = mass[0];
		for (int i = 1; i < size; i++)
		{
			mass1[i - 1] = mass[i];
		}
		HeapFree(heaphandle, HEAP_NO_SERIALIZE, mass);
		size = newsize;
		mass = (int*)HeapAlloc(heaphandle, HEAP_ZERO_MEMORY, size * sizeof(int));
		for (int i = 0; i < size; i++)
		{
			mass[i] = mass1[i];
		}
		HeapFree(heap, HEAP_NO_SERIALIZE, mass1);
		return first;
	}
}

int Turn::GetFirstEl() //��������� ������� ��������
{
	if (size == 0)
	{
		return 0;
	}
	else
	{
		return mass[0];
	}
}

int Turn::GetSize() //��������� ��������
{
	return size;
}

void Turn::WriteToFile() //������ � ����
{
	DWORD dwTemp;
	HANDLE hFile = CreateFile(L"file.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile) {
		return;
	}
	SetFilePointer(hFile, 0, 0, FILE_BEGIN);
	for (int i = 0; i < size; i++)
	{
		WriteFile(hFile, &(mass[i]), sizeof(int), &dwTemp, NULL);
	}
	CloseHandle(hFile);
}

void Turn::Push(int value) //����� ���������� �������� � �������
{
	if (size == 0)
	{
		HeapFree(heaphandle, HEAP_NO_SERIALIZE, mass);
		size = 1;
		mass = (int*)HeapAlloc(heaphandle, HEAP_ZERO_MEMORY, size * sizeof(int));
		mass[0] = value;
	}
	else
	{
		//�������� ������ ������ � ����������� � �����
		HANDLE heap = GetProcessHeap();
		int* mass1 = (int*)HeapAlloc(heaphandle, HEAP_ZERO_MEMORY, size * sizeof(int));
		for (int i = 0; i < size; i++)
		{
			mass1[i] = mass[i];
		}
		HeapFree(heaphandle, HEAP_NO_SERIALIZE, mass);
		size = size + 1;
		mass = (int*)HeapAlloc(heaphandle, HEAP_ZERO_MEMORY, size * sizeof(int));
		for (int i = 0; i < size; i++)
		{
			mass[i] = mass1[i];
		}
		mass[size - 1] = value;
		HeapFree(heap, HEAP_NO_SERIALIZE, mass1);
	}
}


Turn::~Turn()
{
	HeapFree(heaphandle, HEAP_NO_SERIALIZE, mass);
}
//AddThread = CreateThread(NULL, 0, MyThread, this, 0, &id);