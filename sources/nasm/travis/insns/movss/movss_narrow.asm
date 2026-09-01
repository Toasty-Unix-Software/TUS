	movss xmm0, dword [0xd5d]
	movss xmm5, xmm0
	movss dword [0x506], xmm6
	movss xmm0, xmm3

%ifdef ERROR
	movss xmm12, xmm12
	movss xmm12, xmm15
%endif
