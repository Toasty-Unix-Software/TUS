	enqcmd dx, zword [0xfdc]
	enqcmd ax, zword [0xd8a]
	enqcmd ebx, zword [0x86e]
	enqcmd edx, zword [0x968]

%ifdef ERROR
	enqcmd rax, zword [0x3ac]
	enqcmd rbx, zword [0xbe2]
%endif
