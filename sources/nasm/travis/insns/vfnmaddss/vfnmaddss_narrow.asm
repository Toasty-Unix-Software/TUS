	vfnmaddss xmm5, xmm1, xmm4, xmm6
	vfnmaddss xmm5, xmm4, xmm6
	vfnmaddss xmm6, xmm0, xmm6, xmm3
	vfnmaddss xmm2, xmm7, xmm0, xmm4
	vfnmaddss xmm2, xmm0, xmm4
	vfnmaddss xmm7, xmm7, xmm0, dword [0x583]

%ifdef ERROR
	vfnmaddss xmm9, xmm14, xmm13, xmm8
	vfnmaddss xmm8, xmm14, xmm13, xmm14
%endif
