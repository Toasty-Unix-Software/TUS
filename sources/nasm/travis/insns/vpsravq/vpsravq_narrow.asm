	vpsravq xmm4, xmm5, xmm7
	vpsravq xmm4, xmm7
	vpsravq xmm4, xmm0, xmm0
	vpsravq ymm3, ymm2, ymm5
	vpsravq ymm3, ymm5
	vpsravq ymm2, ymm5, ymm5
	vpsravq zmm3, zmm4, zmm0
	vpsravq zmm3, zmm0
	vpsravq zmm5, zmm7, zword [0x9be]

%ifdef ERROR
	vpsravq xmm13, xmm15, xmm9
	vpsravq ymm9, ymm14, ymm11
	vpsravq zmm15, zmm12, zmm12
	vpsravq xmm25, xmm29, xmm25
	vpsravq ymm28, ymm22, ymm31
	vpsravq zmm30, zmm23, zmm31
%endif
