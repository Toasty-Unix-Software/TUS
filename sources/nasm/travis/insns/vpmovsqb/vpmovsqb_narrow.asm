	vpmovsqb xmm3, xmm4
	vpmovsqb xmm4, xmm3
	vpmovsqb xmm1, ymm1
	vpmovsqb xmm1, ymm0
	vpmovsqb xmm1, zmm7
	vpmovsqb xmm1, zmm5
	vpmovsqb word [0xa2b], xmm6
	vpmovsqb word [0x770], xmm7

%ifdef ERROR
	vpmovsqb xmm14, xmm8
	vpmovsqb xmm8, ymm8
	vpmovsqb xmm9, zmm15
	vpmovsqb word [0xfdf], xmm12
	vpmovsqb xmm23, xmm19
	vpmovsqb xmm19, ymm26
	vpmovsqb xmm23, zmm23
	vpmovsqb word [0x326], xmm31
%endif
