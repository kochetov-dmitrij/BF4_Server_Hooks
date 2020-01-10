#include <comutil.h>

#define OFFSET_healthSetterInstruction 0x140C1A423
#define OFFSET_healthSetterCave 0x140E45E1A


/**
 * Fills @param instruction with 'call' instruction where first byte defines 'call' opcode and next 4 bytes define relative address of the desired function
 * [0xE8, 0x??, 0x??, 0x??, 0x??]
 *
 * @param instruction   Array of 5 bytes
 * @param callsFromAdr  Address of asm instruction that must be substituted with 'call' instruction
 * @param funcAdr       Address of first asm instruction to execute in the cave
 */
void makeCallInstructionWithOffset(BYTE* instruction, LPVOID callsFromAdr, LPVOID funcAdr) {
	instruction[0] = 0xE8;
	*(int*)(instruction + 1) = int((BYTE*)funcAdr - (BYTE*)callsFromAdr - 5);
}


namespace ASM_Hooks
{
	/** 
	 * Makes pillars indestructible
	 * 
	 * Substitutes OFFSET_Instruction with call[OFFSET_Cave] instruction
	 * Code inside the cave checks all HP values passing through. If it's equal to 1250(init pillars HP) then changes it to a large number ~10^21
	 *
	 * @param OFFSET_Instruction  Address of xmm0,[rcx+30] instruction that sets HP to all props
	 * @param OFFSET_Cave         Address of any code cave equal or larger than 26 bytes
	 */
	inline auto enableNolevolution(LPVOID OFFSET_Instruction, LPVOID OFFSET_Cave) {
		DWORD old;

		const size_t LENGTH_Cave = 26;
		BYTE MOD_Cave[LENGTH_Cave] = {
			0xEB, 0x18,                                  //   ---- jmp [RIP+18]            Code that executes before the cave must skip it. Jump to the end of the cave
			0xF3, 0x0F, 0x10, 0x41, 0x30,                //  |     movss xmm0,[rcx+30]     Function starts here (OFFSET_CaveCode). Copy HP value into xmm0 register like it's done in OFFSET_Instruction
			0x0F, 0x2E, 0x05, 0x08, 0x00, 0x00, 0x00,    //  |     ucomiss xmm0,[RIP+8]    Compare value of xmm0 register with 1250.0f stored in the last 4 bytes
			0x75, 0x05,                                  //  |  -- jne                     If it's not equal then all fine, jump to ret
			0xF3, 0x0F, 0x10, 0x41, 0x8D,                //  | |   movss xmm0,[rcx-73]     Tower pillar detected. Copy a shifted HP value (~10^21) into xmm0 register
			0xC3,                                        //  |  -> ret                     Return from the function
			0x00, 0x40, 0x9C, 0x44                       //  |     ####                    1250.0f in 4 bytes representation
		};	                                             //   -->	

		VirtualProtect(OFFSET_Cave, LENGTH_Cave, PAGE_EXECUTE_WRITECOPY, &old);
		memcpy(OFFSET_Cave, MOD_Cave, LENGTH_Cave);
		VirtualProtect(OFFSET_Cave, LENGTH_Cave, old, &old);

		LPVOID OFFSET_CaveCode = LPVOID((BYTE*)OFFSET_Cave + 2);

		const size_t LENGTH_Instruction = 5;
		BYTE MOD_Instruction[LENGTH_Instruction];
		makeCallInstructionWithOffset(MOD_Instruction, OFFSET_Instruction, OFFSET_CaveCode);

		VirtualProtect(OFFSET_Instruction, LENGTH_Instruction, PAGE_EXECUTE_WRITECOPY, &old);
		memcpy(OFFSET_Instruction, MOD_Instruction, LENGTH_Instruction);
		VirtualProtect(OFFSET_Instruction, LENGTH_Instruction, old, &old);
	}
}
