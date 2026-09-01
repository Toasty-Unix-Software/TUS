	vplzcntd xmm0, xmm1
	vplzcntd xmm6, xmm2
	vplzcntd ymm7, ymm7
	vplzcntd ymm7, ymm0
	vplzcntd zmm4, zword [0x2af]
	vplzcntd zmm0, zmm1

%ifdef ERROR
	vplzcntd xmm14, xmm8
	vplzcntd ymm14, ymm15
	vplzcntd zmm12, zmm14
	vplzcntd xmm17, xmm21
	vplzcntd ymm22, ymm31
	vplzcntd zmm24, zmm31
%endif
