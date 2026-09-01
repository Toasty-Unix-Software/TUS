	vcmpeq_osps xmm1, xmm2, xmm6
	vcmpeq_osps xmm1, xmm6
	vcmpeq_osps xmm4, xmm4, xmm4
	vcmpeq_osps ymm5, ymm4, ymm1
	vcmpeq_osps ymm5, ymm1
	vcmpeq_osps ymm3, ymm2, yword [0xdc7]
	vcmpeq_osps k7, xmm3, xmm5
	vcmpeq_osps k5, xmm7, xmm2
	vcmpeq_osps k7, ymm1, ymm4
	vcmpeq_osps k4, ymm1, ymm2

%ifdef ERROR
	vcmpeq_osps xmm12, xmm13, xmm13
	vcmpeq_osps ymm9, ymm8, ymm12
	vcmpeq_osps k1, xmm11, xmm13
	vcmpeq_osps k5, ymm14, ymm14
%endif
