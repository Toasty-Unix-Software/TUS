	vprorvq xmm6, xmm0, xmm4
	vprorvq xmm6, xmm4
	vprorvq xmm0, xmm6, oword [0x547]
	vprorvq ymm1, ymm6, yword [0x688]
	vprorvq ymm1, yword [0x688]
	vprorvq ymm0, ymm5, ymm6
	vprorvq zmm0, zmm6, zmm2
	vprorvq zmm0, zmm2
	vprorvq zmm4, zmm6, zmm2

%ifdef ERROR
	vprorvq xmm11, xmm15, xmm14
	vprorvq ymm14, ymm10, ymm12
	vprorvq zmm8, zmm12, zmm8
	vprorvq xmm16, xmm20, xmm22
	vprorvq ymm28, ymm22, ymm28
	vprorvq zmm20, zmm23, zmm22
%endif
