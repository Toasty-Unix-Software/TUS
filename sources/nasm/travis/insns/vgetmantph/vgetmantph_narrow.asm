	vgetmantph xmm1, xmm4, 0x47
	vgetmantph xmm1, xmm5, 0xb3
	vgetmantph ymm1, yword [0xccb], 0xe1
	vgetmantph ymm1, ymm6, 0x97
	vgetmantph zmm4, zmm2, 0xb5
	vgetmantph zmm1, zmm2, 0x4e

%ifdef ERROR
	vgetmantph xmm10, xmm12, 0x5c
	vgetmantph ymm13, ymm12, 0x49
	vgetmantph zmm13, zmm12, 0x32
	vgetmantph xmm18, xmm25, 0xc7
	vgetmantph ymm22, ymm17, 0xca
	vgetmantph zmm26, zmm31, 0xad
%endif
