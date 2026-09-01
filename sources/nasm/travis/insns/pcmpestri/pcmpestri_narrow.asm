	pcmpestri xmm5, oword [0xc4c], 0xf6
	pcmpestri xmm5, oword [0x94e], 0x5c

%ifdef ERROR
	pcmpestri xmm13, xmm11, 0xb1
%endif
