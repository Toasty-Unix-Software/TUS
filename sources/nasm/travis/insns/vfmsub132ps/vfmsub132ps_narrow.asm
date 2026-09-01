	vfmsub132ps xmm0, xmm5, xmm0
	vfmsub132ps xmm0, xmm0, xmm3
	vfmsub132ps ymm5, ymm7, yword [0x5c4]
	vfmsub132ps ymm3, ymm7, ymm7
	vfmsub132ps xmm0, xmm0, xmm6
	vfmsub132ps xmm4, xmm5, xmm3
	vfmsub132ps ymm3, ymm5, yword [0x2b8]
	vfmsub132ps ymm7, ymm3, ymm6

%ifdef ERROR
	vfmsub132ps xmm12, xmm10, xmm11
	vfmsub132ps ymm11, ymm10, ymm13
	vfmsub132ps xmm8, xmm10, xmm9
	vfmsub132ps ymm12, ymm15, ymm14
	vfmsub132ps xmm28, xmm16, xmm20
	vfmsub132ps ymm27, ymm19, ymm20
	vfmsub132ps xmm23, xmm25, xmm28
	vfmsub132ps ymm22, ymm26, ymm17
%endif
