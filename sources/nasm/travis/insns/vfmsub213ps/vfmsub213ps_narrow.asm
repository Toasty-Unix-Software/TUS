	vfmsub213ps xmm2, xmm4, xmm7
	vfmsub213ps xmm3, xmm2, xmm2
	vfmsub213ps ymm4, ymm2, ymm0
	vfmsub213ps ymm1, ymm4, ymm6
	vfmsub213ps xmm2, xmm1, oword [0xf87]
	vfmsub213ps xmm3, xmm4, xmm4
	vfmsub213ps ymm4, ymm1, ymm3
	vfmsub213ps ymm2, ymm1, yword [0x2dd]

%ifdef ERROR
	vfmsub213ps xmm15, xmm11, xmm14
	vfmsub213ps ymm15, ymm12, ymm10
	vfmsub213ps xmm9, xmm14, xmm9
	vfmsub213ps ymm12, ymm12, ymm9
	vfmsub213ps xmm29, xmm20, xmm21
	vfmsub213ps ymm17, ymm27, ymm30
	vfmsub213ps xmm30, xmm20, xmm18
	vfmsub213ps ymm20, ymm25, ymm19
%endif
