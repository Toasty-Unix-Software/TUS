default rel
	vpbroadcastd xmm5, dword [0xc73]
	vpbroadcastd xmm4, dword [0xb7e]
	vpbroadcastd xmm3, xmm1
	vpbroadcastd xmm2, xmm0
	vpbroadcastd ymm7, dword [0xf53]
	vpbroadcastd ymm1, dword [0x536]
	vpbroadcastd ymm5, xmm7
	vpbroadcastd ymm6, xmm7
	vpbroadcastd xmm8, dword [0x80a]
	vpbroadcastd xmm8, xmm10
	vpbroadcastd ymm14, dword [0xadd]
	vpbroadcastd ymm12, xmm8
	vpbroadcastd xmm18, dword [0x8c7]
	vpbroadcastd xmm30, xmm23
	vpbroadcastd ymm18, dword [0x1d0]
	vpbroadcastd ymm25, xmm22
	vpbroadcastd xmm1{k4}, dword [0x8a2]
	vpbroadcastd ymm1{k6}, dword [0xd79]
	vpbroadcastd zmm7{k5}, dword [0x28a]
	vpbroadcastd xmm5{k1}, xmm7
	vpbroadcastd ymm7{k1}, xmm4
	vpbroadcastd zmm5{k1}, xmm4
	vpbroadcastd xmm3{k4}, ecx
	vpbroadcastd ymm7{k3}, ebp
	vpbroadcastd zmm0{k3}, eax
	vpbroadcastd xmm3{k5}{z}, dword [0xb59]
	vpbroadcastd ymm0{k7}{z}, dword [0x69c]
	vpbroadcastd zmm4{k7}{z}, dword [0x97d]
	vpbroadcastd xmm5{k3}{z}, xmm3
	vpbroadcastd ymm7{k2}{z}, xmm3
	vpbroadcastd zmm6{k1}{z}, xmm4
	vpbroadcastd xmm2{k4}{z}, edi
	vpbroadcastd ymm1{k4}{z}, ebx
	vpbroadcastd zmm6{k5}{z}, edi
	vpbroadcastd xmm3, dword [eax+1]
	vpbroadcastd xmm5, dword [eax+64]
	vpbroadcastd ymm7, dword [eax+1]
	vpbroadcastd ymm5, dword [eax+64]
	vpbroadcastd xmm1, dword [eax+1]
	vpbroadcastd xmm4, dword [eax+64]
	vpbroadcastd ymm0, dword [eax+1]
	vpbroadcastd ymm4, dword [eax+64]
	vpbroadcastd zmm1, dword [eax+1]
	vpbroadcastd zmm2, dword [eax+64]
	vpbroadcastd xmm3, [0xf12]
	vpbroadcastd ymm1, [0xebd]
	vpbroadcastd xmm6, [0x2d6]
	vpbroadcastd ymm0, [0x463]
	vpbroadcastd zmm1, [0x98a]
