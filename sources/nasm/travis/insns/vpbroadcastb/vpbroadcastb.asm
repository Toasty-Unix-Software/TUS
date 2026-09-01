default rel
	vpbroadcastb xmm2, byte [0x5c9]
	vpbroadcastb xmm6, byte [0x4e1]
	vpbroadcastb xmm7, xmm1
	vpbroadcastb xmm6, xmm5
	vpbroadcastb ymm6, byte [0x650]
	vpbroadcastb ymm0, byte [0x64f]
	vpbroadcastb ymm0, xmm4
	vpbroadcastb ymm3, xmm0
	vpbroadcastb xmm8, byte [0x51c]
	vpbroadcastb xmm9, xmm8
	vpbroadcastb ymm15, byte [0xe73]
	vpbroadcastb ymm11, xmm11
	vpbroadcastb xmm17, byte [0x9f7]
	vpbroadcastb xmm24, xmm31
	vpbroadcastb ymm26, byte [0x73a]
	vpbroadcastb ymm25, xmm21
	vpbroadcastb xmm3{k1}, xmm4
	vpbroadcastb ymm1{k2}, xmm1
	vpbroadcastb zmm4{k6}, xmm2
	vpbroadcastb xmm3{k2}, bl
	vpbroadcastb xmm4{k6}, dx
	vpbroadcastb xmm2{k1}, eax
	vpbroadcastb xmm5{k7}, rcx
	vpbroadcastb ymm1{k2}, al
	vpbroadcastb ymm3{k6}, di
	vpbroadcastb ymm7{k1}, esi
	vpbroadcastb ymm3{k6}, rax
	vpbroadcastb zmm2{k7}, cl
	vpbroadcastb zmm2{k2}, dx
	vpbroadcastb zmm6{k6}, ebx
	vpbroadcastb zmm3{k4}, rcx
	vpbroadcastb xmm2{k4}{z}, byte [0x407]
	vpbroadcastb ymm1{k6}{z}, xmm1
	vpbroadcastb zmm5{k7}{z}, byte [0xd9a]
	vpbroadcastb xmm4{k7}{z}, dl
	vpbroadcastb xmm6{k1}{z}, bx
	vpbroadcastb xmm6{k4}{z}, esi
	vpbroadcastb xmm5{k3}{z}, rdx
	vpbroadcastb ymm1{k2}{z}, dl
	vpbroadcastb ymm5{k7}{z}, bx
	vpbroadcastb ymm2{k6}{z}, ebp
	vpbroadcastb ymm7{k1}{z}, rsi
	vpbroadcastb zmm0{k7}{z}, bl
	vpbroadcastb zmm7{k1}{z}, bx
	vpbroadcastb zmm2{k2}{z}, edx
	vpbroadcastb zmm4{k5}{z}, rdx
	vpbroadcastb xmm7, byte [eax+1]
	vpbroadcastb xmm1, byte [eax+64]
	vpbroadcastb ymm6, byte [eax+1]
	vpbroadcastb ymm0, byte [eax+64]
	vpbroadcastb xmm1, byte [eax+1]
	vpbroadcastb xmm5, byte [eax+64]
	vpbroadcastb ymm3, byte [eax+1]
	vpbroadcastb ymm2, byte [eax+64]
	vpbroadcastb zmm4, byte [eax+1]
	vpbroadcastb zmm4, byte [eax+64]
	vpbroadcastb xmm7, [0xa73]
	vpbroadcastb ymm5, [0xc3e]
	vpbroadcastb xmm6, [0x447]
	vpbroadcastb ymm3, [0x1d0]
	vpbroadcastb zmm3, [0x555]
