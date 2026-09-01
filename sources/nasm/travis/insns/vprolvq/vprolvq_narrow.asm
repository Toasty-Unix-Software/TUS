	vprolvq xmm6, xmm5, oword [0x550]
	vprolvq xmm6, oword [0x550]
	vprolvq xmm7, xmm2, oword [0x219]
	vprolvq ymm0, ymm2, yword [0x3a7]
	vprolvq ymm0, yword [0x3a7]
	vprolvq ymm4, ymm5, yword [0xe4b]
	vprolvq zmm7, zmm4, zmm7
	vprolvq zmm7, zmm7
	vprolvq zmm6, zmm6, zword [0xb76]

%ifdef ERROR
	vprolvq xmm13, xmm15, xmm13
	vprolvq ymm9, ymm9, ymm11
	vprolvq zmm14, zmm10, zmm14
	vprolvq xmm31, xmm26, xmm22
	vprolvq ymm28, ymm30, ymm22
	vprolvq zmm23, zmm18, zmm24
%endif
