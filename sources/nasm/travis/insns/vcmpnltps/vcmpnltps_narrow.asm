	vcmpnltps xmm4, xmm2, oword [0xc31]
	vcmpnltps xmm4, oword [0xc31]
	vcmpnltps xmm3, xmm5, xmm1
	vcmpnltps ymm7, ymm3, yword [0x7a4]
	vcmpnltps ymm7, yword [0x7a4]
	vcmpnltps ymm4, ymm4, yword [0xaf7]
	vcmpnltps k7, xmm4, xmm2
	vcmpnltps k5, xmm0, xmm3
	vcmpnltps k1, ymm6, yword [0xa90]
	vcmpnltps k1, ymm1, yword [0xdd0]

%ifdef ERROR
	vcmpnltps xmm8, xmm14, xmm12
	vcmpnltps ymm14, ymm9, ymm15
	vcmpnltps k6, xmm12, xmm9
	vcmpnltps k3, ymm11, ymm9
%endif
