default rel
	vpmovsqb xmm3, xmm4
	vpmovsqb xmm4, xmm3
	vpmovsqb xmm1, ymm1
	vpmovsqb xmm1, ymm0
	vpmovsqb xmm1, zmm7
	vpmovsqb xmm1, zmm5
	vpmovsqb word [0xa2b], xmm6
	vpmovsqb word [0x770], xmm7
	vpmovsqb xmm14, xmm8
	vpmovsqb xmm8, ymm8
	vpmovsqb xmm9, zmm15
	vpmovsqb word [0xfdf], xmm12
	vpmovsqb xmm23, xmm19
	vpmovsqb xmm19, ymm26
	vpmovsqb xmm23, zmm23
	vpmovsqb word [0x326], xmm31
	vpmovsqb xmm6{k1}, xmm2
	vpmovsqb xmm2{k2}, ymm4
	vpmovsqb xmm1{k6}, zmm6
	vpmovsqb word [0x1f9]{k4}, xmm5
	vpmovsqb dword [0x315]{k4}, ymm2
	vpmovsqb qword [0x39e]{k2}, zmm2
	vpmovsqb word [eax+1], xmm2
	vpmovsqb word [eax+64], xmm1
	vpmovsqb dword [eax+1], ymm5
	vpmovsqb dword [eax+64], ymm3
	vpmovsqb qword [eax+1], zmm1
	vpmovsqb qword [eax+64], zmm6
	vpmovsqb [0x1de], xmm7
	vpmovsqb [0x530], ymm7
	vpmovsqb [0x7ee], zmm0
