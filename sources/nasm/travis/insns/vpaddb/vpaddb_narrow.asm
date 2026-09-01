	vpaddb xmm1, xmm4, xmm0
	vpaddb xmm1, xmm0
	vpaddb xmm1, xmm5, xmm3
	vpaddb ymm2, ymm2, ymm7
	vpaddb ymm2, ymm7
	vpaddb ymm4, ymm0, ymm4
	vpaddb xmm0, xmm6, xmm2
	vpaddb xmm0, xmm2
	vpaddb xmm0, xmm2, oword [0x11f]
	vpaddb ymm4, ymm5, ymm0
	vpaddb ymm4, ymm0
	vpaddb ymm5, ymm0, yword [0xd79]

%ifdef ERROR
	vpaddb xmm11, xmm12, xmm8
	vpaddb ymm13, ymm9, ymm8
	vpaddb xmm8, xmm11, xmm14
	vpaddb ymm15, ymm9, ymm14
	vpaddb xmm24, xmm28, xmm31
	vpaddb ymm26, ymm23, ymm26
	vpaddb xmm19, xmm20, xmm18
	vpaddb ymm27, ymm28, ymm22
%endif
