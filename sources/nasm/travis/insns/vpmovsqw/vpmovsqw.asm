default rel
	vpmovsqw xmm1, xmm4
	vpmovsqw xmm2, xmm4
	vpmovsqw xmm2, ymm7
	vpmovsqw xmm3, ymm0
	vpmovsqw xmm2, zmm4
	vpmovsqw xmm4, zmm4
	vpmovsqw dword [0x4cb], xmm7
	vpmovsqw dword [0xce9], xmm6
	vpmovsqw xmm15, xmm14
	vpmovsqw xmm13, ymm10
	vpmovsqw xmm8, zmm10
	vpmovsqw dword [0x64f], xmm15
	vpmovsqw xmm20, xmm25
	vpmovsqw xmm30, ymm22
	vpmovsqw xmm25, zmm28
	vpmovsqw dword [0x180], xmm30
	vpmovsqw xmm3{k4}, xmm1
	vpmovsqw xmm0{k3}, ymm3
	vpmovsqw xmm7{k2}, zmm2
	vpmovsqw dword [0x351]{k1}, xmm7
	vpmovsqw qword [0x57c]{k3}, ymm5
	vpmovsqw oword [0x988]{k6}, zmm6
	vpmovsqw dword [eax+1], xmm2
	vpmovsqw dword [eax+64], xmm7
	vpmovsqw qword [eax+1], ymm7
	vpmovsqw qword [eax+64], ymm2
	vpmovsqw oword [eax+1], zmm0
	vpmovsqw oword [eax+64], zmm4
	vpmovsqw [0x9c5], xmm3
	vpmovsqw [0x3c4], ymm6
	vpmovsqw [0xebe], zmm7
