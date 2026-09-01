	vfnmsub123ps xmm5, xmm7, xmm4
	vfnmsub123ps xmm6, xmm2, xmm5
	vfnmsub123ps ymm3, ymm7, yword [0x748]
	vfnmsub123ps ymm2, ymm0, ymm5

%ifdef ERROR
	vfnmsub123ps xmm10, xmm8, xmm8
	vfnmsub123ps ymm13, ymm15, ymm10
%endif
