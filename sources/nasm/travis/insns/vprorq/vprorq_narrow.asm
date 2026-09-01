	vprorq xmm7, oword [0x249], 0x45
	vprorq xmm7, 0x45
	vprorq xmm0, oword [0xcc2], 0xcc
	vprorq ymm5, ymm2, 0x3b
	vprorq ymm5, 0x3b
	vprorq ymm6, ymm0, 0x38
	vprorq zmm5, zmm6, 0xe4
	vprorq zmm5, 0xe4
	vprorq zmm0, zword [0xd60], 0xe0

%ifdef ERROR
	vprorq xmm13, xmm9, 0xa7
	vprorq ymm10, ymm12, 0x9c
	vprorq zmm14, zmm14, 0x92
	vprorq xmm21, xmm29, 0xe7
	vprorq ymm31, ymm31, 0xd6
	vprorq zmm24, zmm29, 0xe5
%endif
