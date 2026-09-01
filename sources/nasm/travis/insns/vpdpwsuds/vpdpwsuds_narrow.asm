	vpdpwsuds xmm1, xmm2, oword [0x674]
	vpdpwsuds xmm3, xmm5, oword [0x834]
	vpdpwsuds ymm6, ymm7, ymm6
	vpdpwsuds ymm0, ymm7, ymm3
	vpdpwsuds xmm5, xmm0, xmm1
	vpdpwsuds xmm0, xmm2, xmm6
	vpdpwsuds ymm4, ymm2, ymm6
	vpdpwsuds ymm2, ymm6, ymm2

%ifdef ERROR
	vpdpwsuds xmm13, xmm14, xmm14
	vpdpwsuds ymm11, ymm11, ymm13
	vpdpwsuds xmm14, xmm10, xmm8
	vpdpwsuds ymm8, ymm15, ymm15
	vpdpwsuds xmm29, xmm24, xmm27
	vpdpwsuds ymm29, ymm27, ymm17
	vpdpwsuds xmm23, xmm18, xmm16
	vpdpwsuds ymm21, ymm24, ymm18
%endif
