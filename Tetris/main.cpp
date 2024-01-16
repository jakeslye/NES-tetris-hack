#include <iostream>
#include <Windows.h>

#include "globals.h"
#include "helpers.h"

std::ostream hexcout(std::cout.rdbuf());

enum Pieces : BYTE {
	VerticalI = 17,
	HorizontalI = 18,
	Box = 10,
	J = 4,
	UpHoriztoalJ = 5,
	UpSideDownJ = 6,
	DownHorizontalJ = 7,
	HorizontalZ = 8
};

void WaitForPieceToDrop(uintptr_t yAddr) {
	int wait = Read<BYTE>(yAddr);
	wait++;
	while (Read<BYTE>(yAddr) < wait) {
	
	}
	while (Read<BYTE>(yAddr) >= wait) {

	}
}


int main() {

	hexcout << std::hex;

	std::cout << "TETRIS HACK" << std::endl << std::endl;

	DWORD procID = GetProcId(L"nestopia.exe");

	std::cout << "Tetris process id: " << procID << std::endl;

	handle = OpenProcess(PROCESS_ALL_ACCESS, false, procID);

	hexcout << "Opened handle at: " << handle << std::endl;

	uintptr_t baseAddr = GetModuleBaseAddress(procID, L"nestopia.exe");

	hexcout << "nestopia.exe base address: " << baseAddr << std::endl;

	uintptr_t basePointerAddress = baseAddr + 0x001B1300;
	//uintptr_t basePointerAddress = 0x027f3ea0;

	hexcout << "basePointerAddress: " << basePointerAddress << std::endl;

	std::vector<unsigned int> path = { 0x4c, 0xa74, 0xA00, 0xC04 };
	uintptr_t workspace = FindDMAaddy(basePointerAddress, path);

	hexcout << "workspace: " << workspace << std::endl;

	uintptr_t xAddr = FindDMAaddy(workspace, std::vector<unsigned int>{0x60});
	uintptr_t yAddr = FindDMAaddy(workspace, std::vector<unsigned int>{0x61});
	uintptr_t currentPieceAddr = FindDMAaddy(workspace, std::vector<unsigned int>{0x62});
	uintptr_t currentLevelAddr = FindDMAaddy(workspace, std::vector<unsigned int>{0x64});
	uintptr_t NNBAddr = FindDMAaddy(workspace, std::vector<unsigned int>{0xDF});

	std::cout << "Waiting for start... (PRESS INS)" << std::endl;
	while (!GetAsyncKeyState(0x2D)) {

	}

	// 10 == box
	bool doubleJump = false;
	bool newColumn = false;
	int jumpSize = 1;
	int currentCollumn = 0;
	int currentLevel = -1;
	int i = 0;
	int neededPiece = Pieces::VerticalI;
	bool allGood = true;

	int pushDownLevel = 17;

	int wait;

	Write<BYTE>(NNBAddr, 1);

	std::cout << "Running (PRESS END TO STOP)" << std::endl;
	while (!GetAsyncKeyState(0x23)) {
		int newCurrentLevel = (int)Read<BYTE>(currentLevelAddr);


		currentLevel = newCurrentLevel;

		if (currentLevel == 0) {
			Write<BYTE>(NNBAddr, 0);
		}
		if (currentLevel == 248) {
			Write<BYTE>(NNBAddr, 1);
		}
		if (currentLevel == 250) {
			Write<BYTE>(NNBAddr, 0);
		}
		if (currentLevel == 255) {
			Write<BYTE>(NNBAddr, 1);
		}

		if (currentLevel >= 165 && currentLevel <= 174 && currentLevel != 253) {
			//single sideways, signle sideways, box, single sideways, signle sideways - 5 times
			for (int i = 0; i < 5; i++) {
				Write<BYTE>(currentPieceAddr, Pieces::HorizontalI);
				Write<BYTE>(xAddr, 2);
				WaitForPieceToDrop(yAddr);

				Write<BYTE>(currentPieceAddr, Pieces::HorizontalI);
				Write<BYTE>(xAddr, 6);
				WaitForPieceToDrop(yAddr);


				Write<BYTE>(currentPieceAddr, Pieces::Box);
				Write<BYTE>(xAddr, 9);
				WaitForPieceToDrop(yAddr);


				Write<BYTE>(currentPieceAddr, Pieces::HorizontalI);
				Write<BYTE>(xAddr, 2);
				WaitForPieceToDrop(yAddr);


				Write<BYTE>(currentPieceAddr, Pieces::HorizontalI);
				Write<BYTE>(xAddr, 6);
				WaitForPieceToDrop(yAddr);
			}
		}else if (currentLevel == 253 || currentLevel == 0) {
			for (int i = 0; i < 3; i++) {
				Write<BYTE>(currentPieceAddr, Pieces::UpHoriztoalJ);
				Write<BYTE>(xAddr, 2);
				WaitForPieceToDrop(yAddr);


				Write<BYTE>(currentPieceAddr, Pieces::UpHoriztoalJ);
				Write<BYTE>(xAddr, 5);
				WaitForPieceToDrop(yAddr);


				Write<BYTE>(currentPieceAddr, Pieces::UpHoriztoalJ);
				Write<BYTE>(xAddr, 8);
				WaitForPieceToDrop(yAddr);



				Write<BYTE>(currentPieceAddr, Pieces::HorizontalZ);
				Write<BYTE>(xAddr, 5);
				WaitForPieceToDrop(yAddr);


				Write<BYTE>(currentPieceAddr, Pieces::HorizontalZ);
				Write<BYTE>(xAddr, 8);
				WaitForPieceToDrop(yAddr);


				Write<BYTE>(currentPieceAddr, Pieces::DownHorizontalJ);
				Write<BYTE>(xAddr, 5);
				WaitForPieceToDrop(yAddr);


				Write<BYTE>(currentPieceAddr, Pieces::DownHorizontalJ);
				Write<BYTE>(xAddr, 8);
				WaitForPieceToDrop(yAddr);


				Write<BYTE>(currentPieceAddr, Pieces::J);
				Write<BYTE>(xAddr, 3);
				WaitForPieceToDrop(yAddr);


				Write<BYTE>(currentPieceAddr, Pieces::UpSideDownJ);
				Write<BYTE>(xAddr, 0);
				WaitForPieceToDrop(yAddr);


				Write<BYTE>(currentPieceAddr, Pieces::DownHorizontalJ);
				Write<BYTE>(xAddr, 1);
				WaitForPieceToDrop(yAddr);

			}
		} else {
			for (int i = 0; i < 10; i++) {
				Write<BYTE>(currentPieceAddr, Pieces::VerticalI);
				Write<BYTE>(xAddr, i);
				WaitForPieceToDrop(yAddr);

			}
		}
	}

	std::cout << "Current Level: " << currentLevel << std::endl;

	return 0;
}