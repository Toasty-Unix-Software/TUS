	vmovqqa ymm5, ymm3
	vmovqqa ymm1, yword [0x4b2]
	vmovqqa yword [0x135], ymm0
	vmovqqa ymm0, ymm7

%ifdef ERROR
	vmovqqa ymm15, ymm10
	vmovqqa ymm8, ymm11
%endif
