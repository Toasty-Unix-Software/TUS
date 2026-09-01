	vxorpd xmm6, xmm6, xmm2
	vxorpd xmm6, xmm2
	vxorpd xmm2, xmm4, xmm2
	vxorpd ymm0, ymm6, ymm3
	vxorpd ymm0, ymm3
	vxorpd ymm2, ymm7, ymm3
	vxorpd xmm2, xmm3, xmm2
	vxorpd xmm2, xmm2
	vxorpd xmm5, xmm3, xmm7
	vxorpd ymm0, ymm4, ymm0
	vxorpd ymm0, ymm0
	vxorpd ymm5, ymm4, ymm2

%ifdef ERROR
	vxorpd xmm10, xmm13, xmm15
	vxorpd ymm12, ymm13, ymm8
	vxorpd xmm10, xmm10, xmm15
	vxorpd ymm15, ymm14, ymm15
	vxorpd xmm17, xmm30, xmm30
	vxorpd ymm17, ymm21, ymm31
	vxorpd xmm24, xmm23, xmm16
	vxorpd ymm26, ymm24, ymm17
%endif
