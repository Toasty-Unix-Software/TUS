	vfmsubadd123pd xmm7, xmm0, oword [0xc3c]
	vfmsubadd123pd xmm1, xmm2, xmm7
	vfmsubadd123pd ymm3, ymm7, yword [0x96c]
	vfmsubadd123pd ymm0, ymm2, ymm5

%ifdef ERROR
	vfmsubadd123pd xmm8, xmm13, xmm11
	vfmsubadd123pd ymm9, ymm10, ymm8
%endif
