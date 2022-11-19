#include "vielfalt.hpp"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

vector<int> VirtualMachine::open(vector<int> bytes) {

	std::string fileName = "a.all";    //ファイル名

    //ファイル名からバイナリファイルで読み込む
    std::ifstream ifs(fileName, std::ios::binary);

    //読込サイズを調べる。
    ifs.seekg(0, std::ios::end);
    long long int size = ifs.tellg();
    ifs.seekg(0);

    //読み込んだデータをchar型に出力する
    char *data = new char[size];
    ifs.read(data, size);

    //サイズを出力する
	//bytes = new int[size];
    for (int i = 0; i < size; i++)
		bytes.push_back((int)data[i]);

    delete[] data;
	return bytes;
}

void VirtualMachine::VM_main() {
	vector<int> bytes;
	bytes = open(bytes);

	bytesSize = bytes.size();

	constBuf = {};

	std::cout << "SIZE: " << bytesSize << std::endl;

	main(bytes, 0);

	for (auto b : constBuf)
		std::cout << "DEBUG constbuf: " << b << std::endl;

	for (auto b : Stack)
		std::cout << "DEBUG Stack: " << b << std::endl;

}

void VirtualMachine::popPopPush(int data) {
	Stack.pop_back();
	Stack.pop_back();
	Stack.push_back(data);
}

void VirtualMachine::main(vector<int> bytes, int offset) {
	int byte = bytes[offset];
	std::cout << "DEBUG: " << byte << std::endl;

	switch(byte) {
		case CONST: {
			offset++;
			constBuf.push_back(bytes[offset++]);
			break;
		}

		case PUSH: {
			offset++;
			Stack.push_back(constBuf[bytes[offset++]]);
			break;
		}

		case POP: {
			offset++;
			Stack.pop_back();
			break;
		}

		case ENTRY: {
			entry.push_back(offset++);
			offset++;
			break;
		}

		case BEQ: {
			offset++;
			int index = bytes[offset++];
			(Stack.at(Stack.size()-1) == 1)
				? offset = entry[index]
				: offset++;
			std::cout << "DEBUG    BEQ:" << bytes[offset] << std::endl;
			break;
		}

		case ADD: {
			offset++;
			int data = Stack[Stack.size()-1] + Stack[Stack.size()-2];
			popPopPush(data);
			break;
		}

		case SUB: {
			offset++;
			int data = Stack[Stack.size()-1] - Stack[Stack.size()-2];
			popPopPush(data);
			break;
		}

		case MUL: {
			offset++;
			int data = Stack[Stack.size()-1] * Stack[Stack.size()-2];
			popPopPush(data);
			break;
		}

		case DIV: {
			offset++;
			int data = Stack[Stack.size()-1] / Stack[Stack.size()-2];
			popPopPush(data);
			break;
		}

		defalt : {
			offset++;
			break;
		}
	}

	(bytes[offset] != 0x00) ? main(bytes, offset) : void();	
	return;
}

	/*
		ENTRY, 0x00,

		CONST, 0x09,       // index 1 iqual 0x09
		CONST, 0x2,        // index 2 iqual 0x02
		CONST, 0xc,        // index 3 iqual 0x12

		// 0x00 = 9*9 + 2;
		// =========
		PUSH, 0x1,         // push 0x02	
		PUSH, 0x1,         // push 0x02	
		PUSH, 0x0,         // push 0x09	
		
		MUL,
		ADD,

		
		WRITE, 0x00,       // 0x09 write to 0x00
		POP,               // pop
		POP,               // pop

		// =========
		PUSH, 0x02,

		WRITE, 0x00,

		READ, 0x00,
		0x00
	*/


/*
	switch(*byte) {
	    case OK: {
			byte++;
			std::cout << "OK" << std::endl;
			byte++;
			break;
	    }

		case ENTRY: {
			byte++;
			entry.push_back((byte+1));
			std::cout << "DEBUG    ENTRY:" << *(byte+1) << std::endl;
			byte++;
			break;
		}

		case GOTO: {
			byte++;
			byte = baseBytes + *entry[*byte++];// * sizeof(int);
			break;
		}
		case GT: {
			byte+=2;
			std::cout << "    GT: " << Stack[Stack.size()-2] << "<" << Stack[Stack.size()-1] << std::endl;
			(Stack[Stack.size()-1] > Stack[Stack.size()-2])
				? Stack.push_back(1)
				: Stack.push_back(0);
			break;
		}


		case LT: {
			byte+=2;
			// スタックトップとスタック2番目を比較してスタックトップの方が大きければ0をプッシュ
			std::cout << "    LT: " << Stack[Stack.size()-2] << ">" << Stack[Stack.size()-1] << std::endl;
			(Stack[Stack.size()-1] < Stack[Stack.size()-2])
				? Stack.push_back(1)
				: Stack.push_back(0);

			StackBuf.pop_back();
			--stackOffset;

			break;
		}

		case BEQ: {
			byte++;
			int index = *byte;
			(Stack.at(Stack.size()-1) == 0)
				? byte = baseBytes + *entry[index]
				: byte;
			std::cout << "DEBUG    BEQ:" << index << std::endl;
			break;
		}

		case PUSH: {
			byte++;
			Stack.push_back(constBuf[*byte]);
			++stackOffset;
			byte++;
			break;
		}

		case POP: {
			byte+=2;
			StackBuf.push_back(Stack.at(Stack.size()-1));
			Stack.pop_back();
			--stackOffset;
			break;
		}

		case CONST: {
			byte++;
			constBuf.push_back(*(byte++));
			break;
		}

		case WRITE: {
			byte++;
			int offset = *(byte++);

			if (Memory.size() > offset)
			    Memory.push_back(Byte{Stack[Stack.size()-1], I32});
			else
			    Memory.insert(Memory.begin() + offset, Byte{Stack[Stack.size()-1], I32});
			break;
		}

		case READ: {
			byte++;
			int offset = *(byte++);


			if (Stack.size() > offset)
				Stack[stackOffset] = Memory[offset].data;
			else
				Stack.push_back(Memory[offset].data);
			
			++stackOffset;
			break;
		}

		case ADD: {
		    byte++;
		    int data = Stack[Stack.size()-1] + Stack[Stack.size()-2];
			Stack.pop_back();
			Stack.pop_back();
			Stack.push_back(data);
		    stackOffset--;
		    break;
		}

		case SUB: {
		    byte++;
		    Stack[stackOffset - 2] -= Stack[stackOffset - 1];
		    stackOffset--;
		    break;
		}

		case MUL: {
		    byte++;
		    Stack[stackOffset - 2] *= Stack[stackOffset - 1];
		    stackOffset--;
		    break;
		}

		case DIV: {
		    byte++;
		    Stack[stackOffset - 2] /= Stack[stackOffset - 1];
		    stackOffset--;
		    break;
		}

		defalt:
			break;
	}
	(*byte != 0x00) ? VM_run(byte, ++offset) : void();
	*/
