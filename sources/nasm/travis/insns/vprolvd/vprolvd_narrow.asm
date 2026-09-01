	vprolvd xmm7, xmm5, xmm3
	vprolvd xmm7, xmm3
	vprolvd xmm6, xmm0, xmm3
	vprolvd ymm7, ymm4, ymm4
	vprolvd ymm7, ymm4
	vprolvd ymm4, ymm5, ymm3
	vprolvd zmm0, zmm3, zmm5
	vprolvd zmm0, zmm5
	vprolvd zmm5, zmm0, zmm2

%ifdef ERROR
	vprolvd xmm10, xmm12, xmm10
	vprolvd ymm10, ymm10, ymm10
	vprolvd zmm9, zmm13, zmm10
	vprolvd xmm30, xmm31, xmm24
	vprolvd ymm26, ymm28, ymm28
	vprolvd zmm22, zmm22, zmm23
%endif
