	vcmple_oqps xmm3, xmm0, xmm2
	vcmple_oqps xmm3, xmm2
	vcmple_oqps xmm3, xmm4, oword [0x861]
	vcmple_oqps ymm0, ymm5, ymm6
	vcmple_oqps ymm0, ymm6
	vcmple_oqps ymm6, ymm7, yword [0x8a0]
	vcmple_oqps k3, xmm3, xmm1
	vcmple_oqps k7, xmm7, xmm5
	vcmple_oqps k4, ymm2, ymm5
	vcmple_oqps k6, ymm0, ymm6

%ifdef ERROR
	vcmple_oqps xmm8, xmm12, xmm14
	vcmple_oqps ymm9, ymm8, ymm10
	vcmple_oqps k3, xmm8, xmm10
	vcmple_oqps k3, ymm8, ymm15
%endif
