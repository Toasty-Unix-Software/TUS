	bndstx [0xacf], bnd1
	bndstx [0xdaf], bnd3
	bndstx [0x581], edx, bnd2
	bndstx [0xe7d], ebx, bnd0
	bndstx [0x69c], bnd1, ebx
	bndstx [0x53a], bnd1, esi

%ifdef ERROR
	bndstx [0x647], rbx, bnd0
	bndstx [0xaf5], rsi, bnd0
%endif
