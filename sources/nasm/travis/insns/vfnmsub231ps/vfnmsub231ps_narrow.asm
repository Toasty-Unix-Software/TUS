	vfnmsub231ps xmm5, xmm5, oword [0xb48]
	vfnmsub231ps xmm4, xmm1, oword [0xf21]
	vfnmsub231ps ymm7, ymm7, ymm3
	vfnmsub231ps ymm4, ymm6, ymm4
	vfnmsub231ps xmm2, xmm1, xmm2
	vfnmsub231ps xmm4, xmm3, oword [0xae3]
	vfnmsub231ps ymm7, ymm5, ymm0
	vfnmsub231ps ymm1, ymm5, ymm3

%ifdef ERROR
	vfnmsub231ps xmm13, xmm8, xmm14
	vfnmsub231ps ymm12, ymm9, ymm15
	vfnmsub231ps xmm12, xmm15, xmm10
	vfnmsub231ps ymm11, ymm10, ymm12
	vfnmsub231ps xmm23, xmm28, xmm16
	vfnmsub231ps ymm28, ymm23, ymm29
	vfnmsub231ps xmm27, xmm21, xmm25
	vfnmsub231ps ymm26, ymm22, ymm25
%endif
