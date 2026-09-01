	vpminub xmm6, xmm6, xmm2
	vpminub xmm6, xmm2
	vpminub xmm5, xmm2, xmm1
	vpminub ymm0, ymm0, ymm0
	vpminub ymm0, ymm0
	vpminub ymm7, ymm2, ymm5
	vpminub xmm7, xmm7, xmm5
	vpminub xmm7, xmm5
	vpminub xmm3, xmm1, xmm2
	vpminub ymm3, ymm0, ymm4
	vpminub ymm3, ymm4
	vpminub ymm7, ymm0, yword [0x198]

%ifdef ERROR
	vpminub xmm11, xmm9, xmm9
	vpminub ymm9, ymm9, ymm12
	vpminub xmm8, xmm9, xmm11
	vpminub ymm13, ymm15, ymm11
	vpminub xmm20, xmm17, xmm25
	vpminub ymm31, ymm19, ymm25
	vpminub xmm21, xmm16, xmm30
	vpminub ymm26, ymm27, ymm16
%endif
