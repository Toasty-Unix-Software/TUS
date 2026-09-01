	vpsllvq xmm7, xmm0, oword [0x74d]
	vpsllvq xmm7, oword [0x74d]
	vpsllvq xmm7, xmm2, xmm1
	vpsllvq ymm1, ymm2, ymm2
	vpsllvq ymm1, ymm2
	vpsllvq ymm7, ymm0, ymm5
	vpsllvq xmm7, xmm0, xmm4
	vpsllvq xmm7, xmm4
	vpsllvq xmm0, xmm7, xmm0
	vpsllvq ymm3, ymm1, yword [0x86d]
	vpsllvq ymm3, yword [0x86d]
	vpsllvq ymm2, ymm1, ymm3

%ifdef ERROR
	vpsllvq xmm10, xmm13, xmm13
	vpsllvq ymm12, ymm11, ymm11
	vpsllvq xmm14, xmm8, xmm14
	vpsllvq ymm10, ymm13, ymm15
	vpsllvq xmm22, xmm31, xmm21
	vpsllvq ymm23, ymm20, ymm28
	vpsllvq xmm27, xmm22, xmm30
	vpsllvq ymm22, ymm28, ymm18
%endif
