	vfnmaddsd xmm6, xmm1, xmm2, xmm1
	vfnmaddsd xmm6, xmm2, xmm1
	vfnmaddsd xmm7, xmm7, xmm7, xmm3
	vfnmaddsd xmm2, xmm6, xmm1, xmm6
	vfnmaddsd xmm2, xmm1, xmm6
	vfnmaddsd xmm7, xmm5, xmm5, xmm7

%ifdef ERROR
	vfnmaddsd xmm11, xmm14, xmm13, xmm14
	vfnmaddsd xmm9, xmm12, xmm9, xmm12
%endif
