	vcmpgt_oqps xmm4, xmm1, oword [0x71c]
	vcmpgt_oqps xmm4, oword [0x71c]
	vcmpgt_oqps xmm4, xmm5, xmm4
	vcmpgt_oqps ymm1, ymm5, ymm5
	vcmpgt_oqps ymm1, ymm5
	vcmpgt_oqps ymm2, ymm0, yword [0x588]
	vcmpgt_oqps k2, xmm2, xmm7
	vcmpgt_oqps k2, xmm7, oword [0x40d]
	vcmpgt_oqps k4, ymm6, ymm2
	vcmpgt_oqps k1, ymm3, yword [0x231]

%ifdef ERROR
	vcmpgt_oqps xmm12, xmm9, xmm9
	vcmpgt_oqps ymm12, ymm8, ymm8
	vcmpgt_oqps k5, xmm9, xmm10
	vcmpgt_oqps k2, ymm12, ymm14
%endif
