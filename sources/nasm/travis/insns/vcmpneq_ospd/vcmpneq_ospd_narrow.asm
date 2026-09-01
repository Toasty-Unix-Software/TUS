	vcmpneq_ospd xmm6, xmm2, oword [0x25b]
	vcmpneq_ospd xmm6, oword [0x25b]
	vcmpneq_ospd xmm7, xmm2, xmm7
	vcmpneq_ospd ymm7, ymm2, yword [0xc9c]
	vcmpneq_ospd ymm7, yword [0xc9c]
	vcmpneq_ospd ymm7, ymm0, ymm6
	vcmpneq_ospd k4, xmm2, xmm0
	vcmpneq_ospd k3, xmm2, xmm1
	vcmpneq_ospd k1, ymm3, yword [0xb17]
	vcmpneq_ospd k1, ymm0, ymm4

%ifdef ERROR
	vcmpneq_ospd xmm11, xmm11, xmm10
	vcmpneq_ospd ymm11, ymm10, ymm8
	vcmpneq_ospd k5, xmm15, xmm15
	vcmpneq_ospd k5, ymm9, ymm14
%endif
