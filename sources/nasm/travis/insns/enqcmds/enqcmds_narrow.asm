	enqcmds bp, zword [0xb81]
	enqcmds di, zword [0x974]
	enqcmds esi, zword [0x573]
	enqcmds ebx, zword [0x407]

%ifdef ERROR
	enqcmds rcx, zword [0x1bd]
	enqcmds rax, zword [0xb26]
%endif
