	vpmovsdb xmm4, xmm4
	vpmovsdb xmm4, xmm3
	vpmovsdb xmm6, ymm3
	vpmovsdb xmm4, ymm5
	vpmovsdb xmm6, zmm5
	vpmovsdb xmm4, zmm6
	vpmovsdb dword [0x2ad], xmm7
	vpmovsdb dword [0x281], xmm4

%ifdef ERROR
	vpmovsdb xmm9, xmm11
	vpmovsdb xmm11, ymm13
	vpmovsdb xmm10, zmm13
	vpmovsdb dword [0x1b5], xmm15
	vpmovsdb xmm19, xmm26
	vpmovsdb xmm25, ymm16
	vpmovsdb xmm31, zmm30
	vpmovsdb dword [0x4f6], xmm23
%endif
