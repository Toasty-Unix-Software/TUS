	vcmpunord_spd xmm4, xmm1, oword [0x187]
	vcmpunord_spd xmm4, oword [0x187]
	vcmpunord_spd xmm3, xmm6, xmm7
	vcmpunord_spd ymm1, ymm7, yword [0x4d2]
	vcmpunord_spd ymm1, yword [0x4d2]
	vcmpunord_spd ymm7, ymm0, yword [0xb04]
	vcmpunord_spd k6, xmm5, xmm0
	vcmpunord_spd k4, xmm1, xmm6
	vcmpunord_spd k2, ymm5, yword [0xa3a]
	vcmpunord_spd k4, ymm5, ymm4

%ifdef ERROR
	vcmpunord_spd xmm15, xmm8, xmm12
	vcmpunord_spd ymm10, ymm13, ymm12
	vcmpunord_spd k4, xmm9, xmm14
	vcmpunord_spd k5, ymm13, ymm10
%endif
