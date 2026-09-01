	vpsrlq xmm7, xmm5, xmm4
	vpsrlq xmm7, xmm4
	vpsrlq xmm0, xmm5, xmm6
	vpsrlq xmm5, xmm6, 0xb7
	vpsrlq xmm5, 0xb7
	vpsrlq xmm1, xmm1, 0xce
	vpsrlq ymm5, ymm1, xmm3
	vpsrlq ymm5, xmm3
	vpsrlq ymm3, ymm1, xmm4
	vpsrlq ymm2, ymm6, 0x16
	vpsrlq ymm2, 0x16
	vpsrlq ymm1, ymm7, 0xf

%ifdef ERROR
	vpsrlq xmm13, xmm14, xmm14
	vpsrlq xmm10, xmm14, 0x88
	vpsrlq ymm11, ymm12, xmm15
	vpsrlq ymm8, ymm11, 0x50
	vpsrlq xmm22, xmm21, xmm19
	vpsrlq xmm24, xmm17, 0x95
	vpsrlq ymm30, ymm18, xmm24
	vpsrlq ymm19, ymm18, 0x11
%endif
