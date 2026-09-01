	vpternlogq xmm7, xmm2, xmm5, 0x55
	vpternlogq xmm7, xmm7, xmm1, 0xd1
	vpternlogq ymm4, ymm2, yword [0x290], 0x7d
	vpternlogq ymm6, ymm4, ymm7, 0x66
	vpternlogq zmm0, zmm6, zmm2, 0x25
	vpternlogq zmm6, zmm6, zmm1, 0x3

%ifdef ERROR
	vpternlogq xmm9, xmm9, xmm13, 0x1a
	vpternlogq ymm12, ymm12, ymm14, 0xd7
	vpternlogq zmm10, zmm11, zmm9, 0x4d
	vpternlogq xmm19, xmm29, xmm28, 0x2a
	vpternlogq ymm29, ymm16, ymm18, 0xa4
	vpternlogq zmm18, zmm20, zmm21, 0x89
%endif
