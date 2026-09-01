	vpblendd xmm6, xmm6, oword [0xd08], 0x41
	vpblendd xmm6, oword [0xd08], 0x41
	vpblendd xmm0, xmm1, oword [0x1f1], 0x1
	vpblendd ymm3, ymm2, ymm3, 0x1f
	vpblendd ymm3, ymm3, 0x1f
	vpblendd ymm3, ymm6, ymm6, 0x8d

%ifdef ERROR
	vpblendd xmm13, xmm10, xmm14, 0xcd
	vpblendd ymm9, ymm8, ymm9, 0x9f
%endif
