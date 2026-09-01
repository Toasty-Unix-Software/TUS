	vcmpltps xmm3, xmm2, oword [0xf2c]
	vcmpltps xmm3, oword [0xf2c]
	vcmpltps xmm6, xmm4, xmm0
	vcmpltps ymm3, ymm1, ymm7
	vcmpltps ymm3, ymm7
	vcmpltps ymm2, ymm0, yword [0x9fe]
	vcmpltps k4, xmm2, xmm0
	vcmpltps k4, xmm4, xmm5
	vcmpltps k2, ymm2, ymm5
	vcmpltps k6, ymm0, ymm2

%ifdef ERROR
	vcmpltps xmm10, xmm13, xmm15
	vcmpltps ymm11, ymm9, ymm15
	vcmpltps k4, xmm15, xmm9
	vcmpltps k4, ymm13, ymm15
%endif
