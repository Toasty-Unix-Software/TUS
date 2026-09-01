	vfmsubadd123ps xmm6, xmm5, oword [0xb04]
	vfmsubadd123ps xmm5, xmm1, xmm1
	vfmsubadd123ps ymm4, ymm7, yword [0x7e1]
	vfmsubadd123ps ymm6, ymm7, ymm7

%ifdef ERROR
	vfmsubadd123ps xmm10, xmm14, xmm10
	vfmsubadd123ps ymm14, ymm8, ymm15
%endif
