	vcmpordps xmm0, xmm7, oword [0x1e8]
	vcmpordps xmm0, oword [0x1e8]
	vcmpordps xmm7, xmm6, xmm7
	vcmpordps ymm1, ymm5, ymm2
	vcmpordps ymm1, ymm2
	vcmpordps ymm1, ymm1, ymm1
	vcmpordps k4, xmm2, xmm3
	vcmpordps k5, xmm4, oword [0xe5f]
	vcmpordps k4, ymm3, ymm5
	vcmpordps k6, ymm5, yword [0xab2]

%ifdef ERROR
	vcmpordps xmm11, xmm9, xmm11
	vcmpordps ymm8, ymm14, ymm10
	vcmpordps k6, xmm12, xmm10
	vcmpordps k6, ymm9, ymm12
%endif
