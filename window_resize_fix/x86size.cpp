#include "x86size.h"
namespace MemTools {
	const void* readLEPtr(const void* at) {
		byte* ptr = 0;
		for (int i = 0; i < 4; i++) {
			ptr += (uint32_t)(*((const byte*)at + i)) << (i * 8);
		}
		return ptr;
	}
	const void* readLERPtr(const void* at, const void* anchor) {
		byte* ptr = 0;
		for (int i = 0; i < 4; i++) {
			ptr += (uint32_t)(*((const byte*)at + i)) << (i * 8);
		}
		return (const byte*)anchor + (int32_t)ptr;
	}
}

const void* x86_find_call_after(const byte* start, size_t max, const void* call) {
	auto p = start;
	auto till = p + max;
	auto next = false;
	while (p < till) {
		if (x86_is_eof(*p)) break;
		if (x86_is_call(*p)) {
			auto fn = MemTools::readRCall(p);
			if (next) {
				if (fn != call) return fn;
			} else {
				if (fn == call) next = true;
			}
		}
		auto n = x86size(p);
		if (n) p += n; else return nullptr;
	}
	return nullptr;
}

const byte* x86_find_call_pos(const byte* start, size_t max, const void* call) {
	auto p = start;
	auto till = p + max;
	auto next = false;
	while (p < till) {
		if (x86_is_eof(*p)) break;
		if (x86_is_call(*p)) {
			auto fn = MemTools::readRCall(p);
			if (fn == call) return p;
		}
		auto n = x86size(p);
		if (n) p += n; else return nullptr;
	}
	return nullptr;
}

int x86_count_call(const byte* start, size_t max, const void* call) {
	auto p = start;
	auto till = p + max;
	auto next = false;
	int found = 0;
	while (p < till) {
		if (x86_is_eof(*p)) break;
		if (x86_is_call(*p)) {
			auto fn = MemTools::readRCall(p);
			if (fn == call) found++;
		}
		auto n = x86size(p);
		if (n) p += n; else return found;
	}
	return found;
}

const void* x86_find_call_with_count(const byte* start, size_t max, int count) {
	auto p = start;
	auto till = p + max;
	auto next = false;
	while (p < till) {
		if (x86_is_eof(*p)) break;
		if (x86_is_call(*p)) {
			auto fn = MemTools::readRCall(p);
			auto p1 = p;
			auto found = 0;
			while (p1 < till) {
				if (x86_is_eof(*p1)) break;
				if (x86_is_call(*p)) {
					if (MemTools::readRCall(p1) == fn) found++;
				}
				auto n1 = x86size(p1);
				if (n1) p1 += n1; else break;
			}
			if (found == count) return fn;
		}
		auto n = x86size(p);
		if (n) p += n; else return nullptr;
	}
	return nullptr;
}

const void* x86_find_call_before(const byte* start, size_t max, const void* call) {
	auto p = start;
	auto till = p + max;
	const void* last = nullptr;
	while (p < till) {
		if (x86_is_eof(*p)) break;
		if (x86_is_call(*p)) {
			auto fn = MemTools::readRCall(p);
			if (fn == call) return last;
			last = fn;
		}
		auto n = x86size(p);
		if (n) p += n; else return nullptr;
	}
	return nullptr;
}

const void* x86_find_first_call(const byte* start, size_t max) {
	auto p = start;
	auto till = p + max;
	while (p < till) {
		if (x86_is_eof(*p)) break;
		if (x86_is_call(*p)) return MemTools::readRCall(p);
		auto n = x86size(p);
		if (n) p += n; else return nullptr;
	}
	return nullptr;
}

const void* x86_find_last_call(const byte* start, size_t max) {
	auto p = start;
	auto till = p + max;
	const void* last = nullptr;
	while (p < till) {
		if (x86_is_eof(*p)) break;
		if (x86_is_call(*p)) {
			last = MemTools::readRCall(p);
		}
		auto n = x86size(p);
		if (n) p += n; else return nullptr;
	}
	return last;
}

/* (C) Copyright 2012-2014 Semantic Designs, Inc.
   You may freely use this code provided you retain this copyright message
*/
uint32_t x86size(const byte* pc)
{ // returns length of instruction at PC
	uint32_t length = 0;
	uint32_t opcode, opcode2;
	uint32_t modrm;
	uint32_t sib;
	const byte* p = pc;

	while (true)
	{  // scan across prefix bytes
		opcode = *p++;
		switch (opcode)
		{
			case 0x64: case 0x65: // FS: GS: prefixes
			case 0x36: // SS: prefix
			case 0x66: case 0x67: // operand size overrides
			case 0xF0: case 0xF2: // LOCK, REPNE prefixes
				length++;
				break;
			case 0x2E: // CS: prefix, used as HNT prefix on jumps
			case 0x3E: // DS: prefix, used as HT prefix on jumps
				length++;
				// goto process relative jmp // tighter check possible here
				break;
			default:
				goto process_instruction_body;
		}
	}

	process_instruction_body:
	switch (opcode) // switch on main opcode
	{
		// ONE byte OPCODE, move to next opcode without remark
		case 0x27: case 0x2F:
		case 0x37: case 0x3F:
		case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
		case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x4F:
		case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
		case 0x58: case 0x59: case 0x5A: case 0x5B: case 0x5C: case 0x5D: case 0x5E: case 0x5F:
		case 0x90: // nop
		case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97: // xchg
		case 0x98: case 0x99:
		case 0x9C: case 0x9D: case 0x9E: case 0x9F:
		case 0xA4: case 0xA5: case 0xA6: case 0xA7: case 0xAA: case 0xAB: // string operators
		case 0xAC: case 0xAD: case 0xAE: case 0xAF:
			/* case 0xC3: // RET handled elsewhere */
		case 0xC9:
		case 0xCC: // int3
		case 0xF5: case 0xF8: case 0xF9: case 0xFC: case 0xFD:
		case 0xA8: // +y: test
		case 0x06: // +y: push ES
			return length + 1; // include opcode

		case 0xC3: // RET
			return length + 1; // +y: we don't strictly care, we stop at 0xCC?
			if (*p++ != 0xCC)
				return length + 1;
			if (*p++ != 0xCC)
				return length + 2;
			if (*p++ == 0xCC
				&& *p++ == 0xCC)
				return length + 5;
			goto error;

			// TWO byte INSTRUCTION
		case 0x04: case 0x0C: case 0x14: case 0x1C: case 0x24: case 0x2C: case 0x34: case 0x3C:
		case 0x6A:
		case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB6: case 0xB7:
		case 0xC2:
		case 0xD8: // +y: Is FCOMP always 2 bytes?
			return length + 2;

			// TWO byte RELATIVE BRANCH
		case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77:
		case 0x78: case 0x79: case 0x7A: case 0x7B: case 0x7C: case 0x7D: case 0x7E: case 0x7F:
		case 0xE0: case 0xE1: case 0xE2: case 0xE3: case 0xEB:
			return length + 2;

			// THREE byte INSTRUCTION (NONE!)

		// FIVE byte INSTRUCTION:
		case 0x05: case 0x0D: case 0x15: case 0x1D:
		case 0x25: case 0x2D: case 0x35: case 0x3D:
		case 0x68:
		case 0xA9:
		case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC: case 0xBD: case 0xBE: case 0xBF:
			return length + 5;

			// FIVE byte RELATIVE CALL
		case 0xE8:
			return length + 5;

			// FIVE byte RELATIVE BRANCH
		case 0xE9:
			if (p[4] == 0xCC)
				return length + 6; // <jmp near ptr ...  int 3>
			return length + 5; // plain <jmp near ptr>

		  // FIVE byte DIRECT ADDRESS
		case 0xA1: case 0xA2: case 0xA3: // MOV AL,AX,EAX moffset...
		case 0xA0: // +y: boy, more MOV!
			return length + 5;
			break;

			// ModR/M with no immediate operand
		case 0x00: case 0x01: case 0x02: case 0x03: case 0x08: case 0x09: case 0x0A: case 0x0B:
		case 0x10: case 0x11: case 0x12: case 0x13: case 0x18: case 0x19: case 0x1A: case 0x1B:
		case 0x20: case 0x21: case 0x22: case 0x23: case 0x28: case 0x29: case 0x2A: case 0x2B:
		case 0x30: case 0x31: case 0x32: case 0x33: case 0x38: case 0x39: case 0x3A: case 0x3B:
		case 0x84: case 0x85: case 0x86: case 0x87: case 0x88: case 0x89: case 0x8A: case 0x8B: case 0x8D: case 0x8F:
		case 0xD1: case 0xD2: case 0xD3:
		case 0xFE: case 0xFF: // misinterprets JMP far and CALL far, not worth fixing
			length++; // count opcode
			goto modrm;

			// ModR/M with immediate 8 bit value
		case 0x80: case 0x82: case 0x83:
		case 0xC0: case 0xC1:
		case 0xC6:  // with r=0?
		case 0x6B: // +y: imul
			length += 2; // count opcode and immediate byte
			goto modrm;

			// ModR/M with immediate 32 bit value
		case 0x81:
		case 0xC7:  // with r=0?
		case 0x69: // +y: other imul
			length += 5; // count opcode and immediate byte
			goto modrm;

		case 0x9B: // FSTSW AX = 9B DF E0
			if (*p++ == 0xDF)
			{
				if (*p++ == 0xE0)
					return length + 3;
				trace("InstructionLength: Unimplemented 0x9B tertiary opcode %2x at %x", *p, (uint32_t)p);
				goto error;
			} else {
				trace("InstructionLength: Unimplemented 0x9B secondary opcode %2x at %x", *p, (uint32_t)p);
				goto error;
			}

		case 0xD9: // various FP instructions
			modrm = *p++;
			length++; //  account for FP prefix
			switch (modrm)
			{
				case 0xC9: case 0xD0:
				case 0xE0: case 0xE1: case 0xE4: case 0xE5:
				case 0xE8: case 0xE9: case 0xEA: case 0xEB: case 0xEC: case 0xED: case 0xEE:
				case 0xF8: case 0xF9: case 0xFA: case 0xFB: case 0xFC: case 0xFD: case 0xFE: case 0xFF:
					return length + 1;
				default:  // r bits matter if not one of the above specific opcodes
					switch ((modrm & 0x38) >> 3)
					{
						case 0: goto modrm_fetched;  // fld
						case 1: return length + 1; // fxch
						case 2: goto modrm_fetched; // fst
						case 3: goto modrm_fetched; // fstp
						case 4: goto modrm_fetched; // fldenv
						case 5: goto modrm_fetched; // fldcw
						case 6: goto modrm_fetched; // fnstenv
						case 7: goto modrm_fetched; // fnstcw
					}
					goto error; // unrecognized 2nd byte
			}

		case 0xDB: // various FP instructions
			modrm = *p++;
			length++; //  account for FP prefix
			switch (modrm)
			{
				case 0xE3:
					return length + 1;
				default:  // r bits matter if not one of the above specific opcodes
					#if 1 // +y: if this was 0-ed, does that mean it's bad?
					switch ((modrm & 0x38) >> 3)
					{
						case 0: goto modrm_fetched;  // fld
						case 1: return length + 1; // fxch
						case 2: goto modrm_fetched; // fst
						case 3: goto modrm_fetched; // fstp
						case 4: goto modrm_fetched; // fldenv
						case 5: goto modrm_fetched; // fldcw
						case 6: goto modrm_fetched; // fnstenv
						case 7: goto modrm_fetched; // fnstcw
					}
					#endif
					goto error; // unrecognized 2nd byte
			}

		case 0xDC: { // +y: FMUL,FADD
			length++;
			goto modrm;
		};

		case 0xDD: // various FP instructions
			modrm = *p++;
			length++; //  account for FP prefix
			switch (modrm)
			{
				case 0xE1: case 0xE9:
					return length + 1;
				default:  // r bits matter if not one of the above specific opcodes
					switch ((modrm & 0x38) >> 3)
					{
						case 0: goto modrm_fetched;  // fld
				  // case 1: return length+1; // fisttp
						case 2: goto modrm_fetched; // fst
						case 3: goto modrm_fetched; // fstp
						case 4: return length + 1; // frstor
						case 5: return length + 1; // fucomp
						case 6: goto modrm_fetched; // fnsav
						case 7: goto modrm_fetched; // fnstsw
					}
					goto error; // unrecognized 2nd byte
			}

		case 0xF3: {// funny prefix REPE
			opcode2 = *p++;  // get second opcode byte
			switch (opcode2)
			{
				case 0x90: // == PAUSE
				case 0xA4: case 0xA5: case 0xA6: case 0xA7: case 0xAA: case 0xAB: // string operators
					return length + 2;
				case 0xC3: // (REP) RET
					if (*p++ != 0xCC)
						return length + 2; // only (REP) RET
					if (*p++ != 0xCC)
						goto error;
					if (*p++ == 0xCC)
						return length + 5; // (REP) RET CLONE IS LONG JUMP RELATIVE
					goto error;
				case 0x66: // operand size override (32->16 bits)
					if (*p++ == 0xA5) // "rep movsw"
						return length + 3;
					goto error;
				case 0x0F: // +y:
					opcode2 = *p++;
					switch (opcode2) {
						case 0xE6: // CVTDQ2PD
							length += 3;
							goto modrm;
						case 0x10: // MOVSS
							length += 3;
							goto modrm;
						case 0x11: // /r MOVSS xmm2/m32, xmm1
							length += 3;
							goto modrm;
						case 0x58: // ADDSS
							length += 3;
							goto modrm;
						case 0x5C: // F3 0F 5C /r SUBSS xmm1, xmm2/m32
							length += 3;
							goto modrm;
						default:
							trace("InstructionLength: Unimplemented 0xF3 0x0F tetriary opcode in clone %2x at %x", opcode2, (uint32_t)(p - 2));
							goto error;
					}
				default:
					trace("InstructionLength: Unimplemented 0xF3 secondary opcode in clone %2x at %x", opcode2, (uint32_t)(p - 1));
					goto error;
			}
		}
		case 0xF6: {// funny subblock of opcodes
			modrm = *p++;
			if ((modrm & 0x20) == 0)
				length++; // 8 bit immediate operand
			goto modrm_fetched;
		}
		case 0xF7: {// funny subblock of opcodes
			modrm = *p++;
			if ((modrm & 0x30) == 0)
				length += 4; // 32 bit immediate operand
			goto modrm_fetched;
		}
		case 0x0F: {// Intel's special prefix opcode
			length += 2; // add one for special prefix, and one for following opcode
			opcode2 = *p++;
			switch (opcode2)
			{
				case 0x31: // RDTSC
					return length;

					// CMOVxx
				case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
				case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x4F:
					goto modrm;

					// JC relative 32 bits
				case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
				case 0x88: case 0x89: case 0x8A: case 0x8B: case 0x8C: case 0x8D: case 0x8E: case 0x8F:
					return length + 4; // account for subopcode and displacement

			// SETxx rm32
				case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
				case 0x98: case 0x99: case 0x9A: case 0x9B: case 0x9C: case 0x9D: case 0x9E: case 0x9F:
					goto modrm;

				case 0xA2: // CPUID
					return length + 2;

				case 0xAE: // LFENCE, SFENCE, MFENCE
					opcode2 = *p++;
					switch (opcode2)
					{
						case 0xE8: // LFENCE
						case 0xF0: // MFENCE
						case 0xF8: // SFENCE
							return length + 1;
						default:
							trace("InstructionLength: Unimplemented 0x0F, 0xAE tertiary opcode in clone  %2x at %x", opcode2, (uint32_t)(p - 1));
							goto error;
					}

				case 0xAF: // imul
				case 0xB0: // cmpxchg 8 bits
					goto error;

				case 0xB1: // cmpxchg 32 bits
				case 0xB6: case 0xB7: // movzx
				case 0xBC: /* bsf */ case 0xBD: // bsr
				// case 0xBE: case 0xBF: // movsx 
				case 0xC1: // xadd
				case 0xC7: // cmpxchg8b
					// +y:
				case 0x57: // xorps
				case 0x11: // movsd
				case 0x2A: // cvtsi2sd
				case 0x10: // whatever this was
				case 0x1F: // a fancy NOP?
				case 0xBE: // movsx
				case 0x6E: // movd
				case 0x2C: // CVTTSD2SI
				case 0x2E: // ucomisd
				case 0x2F: // comisd
				case 0x5C: // subsd
				case 0x58: // addsd
				case 0x59: // mulsd
					goto modrm;
				default:
					trace("InstructionLength: Unimplemented 0x0F secondary opcode in clone %2x at %x", opcode2, (uint32_t)(p - 1));
					goto error;
			} // switch
		}
				 // ALL THE THE REST OF THE INSTRUCTIONS; these are instructions that runtime system shouldn't ever use
		default:
			/* case 0x26: case 0x36: // ES: SS: prefixes
			   case 0x9A:
			   case 0xC8: case 0xCA: case 0xCB: case 0xCD: case 0xCE: case 0xCF:
			   case 0xD6: case 0xD7:
			   case 0xE4: case 0xE5: case 0xE6: case 0xE7: case 0xEA: case 0xEB: case 0xEC: case 0xED: case 0xEF:
			   case 0xF4: case 0xFA: case 0xFB:
				*/
			trace("InstructionLength: Unexpected opcode %2x", opcode);
			goto error;
	}

	modrm:
	modrm = *p++;
	modrm_fetched:
	if (modrm >= 0xC0)
		return length + 1;  // account for modrm opcode
	else
	{  /* memory access */
		if ((modrm & 0x7) == 0x04)
		{ /* instruction with SIB byte */
			length++; // account for SIB byte
			sib = *p++; // fetch the sib byte
			if ((sib & 0x7) == 0x05)
			{
				if ((modrm & 0xC0) == 0x40)
					return length + 1 + 1; // account for MOD + byte displacment
				else return length + 1 + 4; // account for MOD + dword displacement
			}
		}
		switch (modrm & 0xC0)
		{
			case 0x0:
				if ((modrm & 0x07) == 0x05)
					return length + 5; // 4 byte displacement
				else return length + 1; // zero length offset
			case 0x80:
				return length + 5;  // 4 byte offset
			default:
				return length + 2;  // one byte offset
		}
	}

	error:
	trace("InstructionLength: unhandled opcode at %8x with opcode %2x", (uint32_t)pc, opcode);
	return 0; // can't actually execute this
}