	cmovnz cx, cx
	cmovnz ax, word [0x582]
	cmovnz eax, edx
	cmovnz ecx, eax
	cmovnz bp, cx, cx
	cmovnz ax, di, dx

%ifdef ERROR
	cmovnz rcx, rbx
	cmovnz rcx, rax
	cmovnz r12w, r12w
	cmovnz r12d, r15d
	cmovnz r15, r8
	cmovnz r10w, r8w, r11w
	cmovnz r25w, r20w
	cmovnz r26d, r22d
	cmovnz r28, r17
	cmovnz r22w, r27w, r23w
	cmovnz rsi, qword [eax+1]
	cmovnz rdx, qword [eax+64]
	cmovnz rbx, rcx, qword [eax+1]
	cmovnz rbx, rdi, qword [eax+64]
	cmovnz rsi, [0xc75]
	cmovnz rsi, rdi, [0x320]
%endif
