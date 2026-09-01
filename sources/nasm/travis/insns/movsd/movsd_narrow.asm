	movsd
	movsd
	movsd xmm0, qword [0xd65]
	movsd xmm1, qword [0x792]
	movsd qword [0x3c8], xmm0
	movsd qword [0xf2d], xmm6

%ifdef ERROR
	movsd xmm14, xmm10
	movsd xmm14, xmm13
%endif
