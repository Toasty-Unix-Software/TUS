	vfmaddss xmm5, xmm2, xmm4, xmm6
	vfmaddss xmm5, xmm4, xmm6
	vfmaddss xmm6, xmm0, xmm1, xmm7
	vfmaddss xmm6, xmm5, xmm5, dword [0xf1b]
	vfmaddss xmm6, xmm5, dword [0xf1b]
	vfmaddss xmm4, xmm1, xmm4, xmm4

%ifdef ERROR
	vfmaddss xmm15, xmm15, xmm10, xmm12
	vfmaddss xmm11, xmm11, xmm11, xmm14
%endif
