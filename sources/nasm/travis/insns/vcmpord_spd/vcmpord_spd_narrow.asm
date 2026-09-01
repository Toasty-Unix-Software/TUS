	vcmpord_spd xmm5, xmm0, oword [0x1a2]
	vcmpord_spd xmm5, oword [0x1a2]
	vcmpord_spd xmm2, xmm2, xmm4
	vcmpord_spd ymm0, ymm2, ymm0
	vcmpord_spd ymm0, ymm0
	vcmpord_spd ymm6, ymm6, ymm0
	vcmpord_spd k6, xmm6, xmm3
	vcmpord_spd k6, xmm4, xmm4
	vcmpord_spd k3, ymm4, yword [0x74a]
	vcmpord_spd k1, ymm2, ymm3

%ifdef ERROR
	vcmpord_spd xmm10, xmm10, xmm10
	vcmpord_spd ymm9, ymm9, ymm8
	vcmpord_spd k6, xmm8, xmm11
	vcmpord_spd k4, ymm11, ymm11
%endif
