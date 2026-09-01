	vcmple_oqpd xmm5, xmm6, xmm0
	vcmple_oqpd xmm5, xmm0
	vcmple_oqpd xmm4, xmm0, oword [0x8c8]
	vcmple_oqpd ymm6, ymm1, ymm5
	vcmple_oqpd ymm6, ymm5
	vcmple_oqpd ymm2, ymm2, yword [0x76a]
	vcmple_oqpd k7, xmm3, xmm4
	vcmple_oqpd k7, xmm2, oword [0x83b]
	vcmple_oqpd k6, ymm5, yword [0x6d9]
	vcmple_oqpd k7, ymm6, yword [0x92e]

%ifdef ERROR
	vcmple_oqpd xmm12, xmm15, xmm12
	vcmple_oqpd ymm10, ymm13, ymm10
	vcmple_oqpd k6, xmm10, xmm8
	vcmple_oqpd k7, ymm14, ymm9
%endif
