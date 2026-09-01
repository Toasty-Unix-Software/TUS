	vpminsd xmm1, xmm2, xmm6
	vpminsd xmm1, xmm6
	vpminsd xmm4, xmm4, xmm5
	vpminsd ymm3, ymm2, ymm5
	vpminsd ymm3, ymm5
	vpminsd ymm1, ymm0, yword [0x9bf]
	vpminsd xmm3, xmm1, oword [0x29c]
	vpminsd xmm3, oword [0x29c]
	vpminsd xmm0, xmm1, xmm6
	vpminsd ymm5, ymm5, ymm2
	vpminsd ymm5, ymm2
	vpminsd ymm6, ymm5, ymm5

%ifdef ERROR
	vpminsd xmm13, xmm13, xmm15
	vpminsd ymm15, ymm9, ymm10
	vpminsd xmm12, xmm13, xmm13
	vpminsd ymm11, ymm12, ymm11
	vpminsd xmm27, xmm17, xmm16
	vpminsd ymm21, ymm21, ymm25
	vpminsd xmm17, xmm30, xmm19
	vpminsd ymm18, ymm25, ymm24
%endif
