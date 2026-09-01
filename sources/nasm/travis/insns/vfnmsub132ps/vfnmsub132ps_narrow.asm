	vfnmsub132ps xmm0, xmm4, xmm1
	vfnmsub132ps xmm0, xmm1, oword [0x58b]
	vfnmsub132ps ymm2, ymm7, ymm6
	vfnmsub132ps ymm2, ymm6, yword [0xd2b]
	vfnmsub132ps xmm7, xmm4, xmm4
	vfnmsub132ps xmm7, xmm3, xmm4
	vfnmsub132ps ymm1, ymm2, yword [0xc7d]
	vfnmsub132ps ymm4, ymm6, ymm5

%ifdef ERROR
	vfnmsub132ps xmm11, xmm15, xmm11
	vfnmsub132ps ymm8, ymm13, ymm12
	vfnmsub132ps xmm13, xmm9, xmm14
	vfnmsub132ps ymm13, ymm11, ymm13
	vfnmsub132ps xmm22, xmm30, xmm22
	vfnmsub132ps ymm30, ymm25, ymm20
	vfnmsub132ps xmm25, xmm18, xmm17
	vfnmsub132ps ymm29, ymm18, ymm31
%endif
