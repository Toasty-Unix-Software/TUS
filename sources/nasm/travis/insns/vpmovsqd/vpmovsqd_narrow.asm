	vpmovsqd xmm4, xmm0
	vpmovsqd xmm1, xmm6
	vpmovsqd xmm5, ymm5
	vpmovsqd xmm6, ymm5
	vpmovsqd ymm6, zmm2
	vpmovsqd ymm1, zmm0
	vpmovsqd qword [0xe33], xmm2
	vpmovsqd qword [0xd78], xmm5

%ifdef ERROR
	vpmovsqd xmm10, xmm13
	vpmovsqd xmm11, ymm10
	vpmovsqd ymm10, zmm8
	vpmovsqd qword [0x8a9], xmm9
	vpmovsqd xmm30, xmm21
	vpmovsqd xmm31, ymm22
	vpmovsqd ymm19, zmm16
	vpmovsqd qword [0x1b0], xmm26
%endif
