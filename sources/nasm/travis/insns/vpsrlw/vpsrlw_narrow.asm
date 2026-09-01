	vpsrlw xmm7, xmm2, xmm0
	vpsrlw xmm7, xmm0
	vpsrlw xmm1, xmm4, xmm6
	vpsrlw xmm1, xmm0, 0x4b
	vpsrlw xmm1, 0x4b
	vpsrlw xmm5, xmm6, 0x46
	vpsrlw ymm1, ymm4, oword [0xa98]
	vpsrlw ymm1, oword [0xa98]
	vpsrlw ymm1, ymm0, xmm2
	vpsrlw ymm2, ymm4, 0x42
	vpsrlw ymm2, 0x42
	vpsrlw ymm0, ymm6, 0x37

%ifdef ERROR
	vpsrlw xmm14, xmm14, xmm13
	vpsrlw xmm11, xmm13, 0x42
	vpsrlw ymm14, ymm14, xmm10
	vpsrlw ymm13, ymm13, 0xf2
	vpsrlw xmm30, xmm16, xmm20
	vpsrlw xmm17, xmm30, 0x34
	vpsrlw ymm31, ymm27, xmm23
	vpsrlw ymm20, ymm16, 0x2c
%endif
