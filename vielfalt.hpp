#include <iostream>
#include <string.h>
#include <vector>
#include <stdlib.h>

#define PUSH 0x01
#define POP 0x02
#define WRITE 0x03
#define READ 0x04
#define ADD 0x05
#define SUB 0x06
#define MUL 0x07
#define DIV 0x08
#define CONST 0x09
#define ENTRY 0x0a
#define OK 0x0b
#define GOTO 0x0c
#define GT 0x0d
#define LT 0x0e
#define BEQ 0x0f

#define I32 0
#define I64 1
#define CHAR 2

#define OPERATOR 0
#define OPERAND 1

using std::vector;

typedef struct Byte {
	int data;
	int type;
} Byte;

class VirtualMachine {
	vector<int> Stack;
	vector<int> StackBuf;
	vector<int> constBuf;
	vector<int> entry;
	vector<Byte> Memory;
	void main(vector<int> bytes, int offset);
	void popPopPush(int data);
	vector<int> open(vector<int> bytes);
	int *entryPoint;
	size_t bytesSize;
	public:
	    void VM_main();
};

