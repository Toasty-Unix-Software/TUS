	vpmovsxwq xmm6, xmm3
	vpmovsxwq xmm4, xmm7
	vpmovsxwq ymm0, qword [0x124]
	vpmovsxwq ymm5, qword [0xacc]
	vpmovsxwq ymm5, xmm2
	vpmovsxwq ymm2, xmm6
	vpmovsxwq xmm5, dword [0x4bb]
	vpmovsxwq xmm5, xmm4

%ifdef ERROR
	vpmovsxwq xmm12, xmm13
	vpmovsxwq ymm11, qword [0xe51]
	vpmovsxwq ymm10, xmm14
	vpmovsxwq xmm8, xmm14
	vpmovsxwq xmm26, xmm16
	vpmovsxwq ymm18, qword [0x6a4]
	vpmovsxwq ymm26, xmm30
	vpmovsxwq xmm30, xmm26
%endif
