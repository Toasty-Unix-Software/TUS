	vfmsub123ps xmm2, xmm5, oword [0x3c8]
	vfmsub123ps xmm1, xmm3, xmm1
	vfmsub123ps ymm3, ymm3, ymm2
	vfmsub123ps ymm4, ymm4, ymm5

%ifdef ERROR
	vfmsub123ps xmm12, xmm11, xmm10
	vfmsub123ps ymm13, ymm15, ymm12
%endif
