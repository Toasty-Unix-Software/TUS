	vfmaddsub123pd xmm4, xmm5, oword [0xf1b]
	vfmaddsub123pd xmm4, xmm3, oword [0x2cd]
	vfmaddsub123pd ymm4, ymm1, ymm3
	vfmaddsub123pd ymm3, ymm7, yword [0xb79]

%ifdef ERROR
	vfmaddsub123pd xmm9, xmm15, xmm13
	vfmaddsub123pd ymm10, ymm11, ymm11
%endif
