	vfmsub231ps xmm5, xmm7, xmm3
	vfmsub231ps xmm5, xmm4, xmm0
	vfmsub231ps ymm4, ymm2, ymm5
	vfmsub231ps ymm4, ymm7, ymm4
	vfmsub231ps xmm3, xmm6, xmm6
	vfmsub231ps xmm6, xmm7, xmm7
	vfmsub231ps ymm2, ymm4, yword [0xed4]
	vfmsub231ps ymm5, ymm7, ymm5

%ifdef ERROR
	vfmsub231ps xmm15, xmm13, xmm11
	vfmsub231ps ymm11, ymm10, ymm15
	vfmsub231ps xmm15, xmm12, xmm14
	vfmsub231ps ymm14, ymm15, ymm12
	vfmsub231ps xmm21, xmm25, xmm25
	vfmsub231ps ymm16, ymm18, ymm28
	vfmsub231ps xmm27, xmm31, xmm26
	vfmsub231ps ymm25, ymm28, ymm21
%endif
