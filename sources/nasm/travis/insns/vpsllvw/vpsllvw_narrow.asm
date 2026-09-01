	vpsllvw xmm0, xmm4, xmm6
	vpsllvw xmm0, xmm6
	vpsllvw xmm3, xmm7, xmm4
	vpsllvw ymm4, ymm2, ymm1
	vpsllvw ymm4, ymm1
	vpsllvw ymm6, ymm3, ymm7
	vpsllvw zmm5, zmm1, zmm5
	vpsllvw zmm5, zmm5
	vpsllvw zmm0, zmm3, zmm6

%ifdef ERROR
	vpsllvw xmm13, xmm11, xmm10
	vpsllvw ymm8, ymm14, ymm12
	vpsllvw zmm8, zmm13, zmm11
	vpsllvw xmm31, xmm25, xmm24
	vpsllvw ymm22, ymm25, ymm26
	vpsllvw zmm19, zmm23, zmm24
%endif
