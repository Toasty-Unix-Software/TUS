	vfnmsub213ps xmm1, xmm5, xmm0
	vfnmsub213ps xmm4, xmm0, xmm6
	vfnmsub213ps ymm7, ymm6, yword [0xf76]
	vfnmsub213ps ymm4, ymm1, ymm5
	vfnmsub213ps xmm7, xmm1, oword [0x962]
	vfnmsub213ps xmm2, xmm4, xmm5
	vfnmsub213ps ymm6, ymm1, ymm7
	vfnmsub213ps ymm0, ymm0, ymm5

%ifdef ERROR
	vfnmsub213ps xmm14, xmm11, xmm12
	vfnmsub213ps ymm15, ymm11, ymm13
	vfnmsub213ps xmm13, xmm11, xmm8
	vfnmsub213ps ymm11, ymm15, ymm12
	vfnmsub213ps xmm31, xmm27, xmm24
	vfnmsub213ps ymm23, ymm30, ymm21
	vfnmsub213ps xmm30, xmm23, xmm21
	vfnmsub213ps ymm16, ymm16, ymm23
%endif
