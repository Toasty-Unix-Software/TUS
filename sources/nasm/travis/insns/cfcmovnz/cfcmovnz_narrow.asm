	cfcmovnz bx, si
	cfcmovnz word [0xf13], di
	cfcmovnz edx, edx
	cfcmovnz ecx, eax
	cfcmovnz di, bx
	cfcmovnz di, word [0x64a]

%ifdef ERROR
	cfcmovnz rdx, rsi
	cfcmovnz rbp, rsi
	cfcmovnz r13w, r8w
	cfcmovnz r10d, r12d
	cfcmovnz r12, r11
	cfcmovnz r12w, r14w
	cfcmovnz r20w, r19w
	cfcmovnz r22d, r17d
	cfcmovnz r31, r21
	cfcmovnz r30w, r21w
	cfcmovnz qword [eax+1], rdx
	cfcmovnz qword [eax+64], rbx
	cfcmovnz rsi, qword [eax+1]
	cfcmovnz rbx, qword [eax+64]
	cfcmovnz rbp, rbp, qword [eax+1]
	cfcmovnz rbp, rsi, qword [eax+64]
	cfcmovnz [0x1b7], rsi
	cfcmovnz rsi, [0x46b]
	cfcmovnz rsi, rbp, [0xaa2]
%endif
