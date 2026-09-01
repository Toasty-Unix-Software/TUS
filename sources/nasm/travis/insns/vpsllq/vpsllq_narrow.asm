	vpsllq xmm2, xmm7, xmm3
	vpsllq xmm2, xmm3
	vpsllq xmm3, xmm0, xmm5
	vpsllq xmm5, xmm1, 0xa0
	vpsllq xmm5, 0xa0
	vpsllq xmm3, xmm1, 0x13
	vpsllq ymm4, ymm4, oword [0xc7a]
	vpsllq ymm4, oword [0xc7a]
	vpsllq ymm3, ymm3, oword [0x185]
	vpsllq ymm2, ymm7, 0x1
	vpsllq ymm2, 0x1
	vpsllq ymm7, ymm4, 0xda

%ifdef ERROR
	vpsllq xmm14, xmm13, xmm14
	vpsllq xmm8, xmm14, 0xab
	vpsllq ymm8, ymm14, xmm11
	vpsllq ymm13, ymm10, 0x44
	vpsllq xmm27, xmm17, xmm21
	vpsllq xmm24, xmm28, 0x9f
	vpsllq ymm17, ymm19, xmm20
	vpsllq ymm24, ymm22, 0x81
%endif
