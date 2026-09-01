default rel
	vpbroadcastw xmm0, word [0x686]
	vpbroadcastw xmm0, word [0x8eb]
	vpbroadcastw xmm3, xmm1
	vpbroadcastw xmm6, xmm6
	vpbroadcastw ymm1, word [0x508]
	vpbroadcastw ymm0, word [0x8fd]
	vpbroadcastw ymm1, xmm7
	vpbroadcastw ymm7, xmm0
	vpbroadcastw xmm15, word [0x7be]
	vpbroadcastw xmm14, xmm9
	vpbroadcastw ymm13, word [0x5db]
	vpbroadcastw ymm8, xmm10
	vpbroadcastw xmm31, word [0x249]
	vpbroadcastw xmm20, xmm31
	vpbroadcastw ymm26, word [0x34e]
	vpbroadcastw ymm22, xmm18
	vpbroadcastw xmm5{k2}, word [0x34d]
	vpbroadcastw ymm7{k2}, xmm5
	vpbroadcastw zmm3{k1}, xmm3
	vpbroadcastw xmm3{k1}, bp
	vpbroadcastw xmm6{k5}, edi
	vpbroadcastw xmm1{k3}, rcx
	vpbroadcastw ymm0{k2}, bp
	vpbroadcastw ymm1{k4}, ebx
	vpbroadcastw ymm0{k3}, rax
	vpbroadcastw zmm4{k6}, bx
	vpbroadcastw zmm5{k2}, ebx
	vpbroadcastw zmm5{k4}, rsi
	vpbroadcastw xmm7{k4}{z}, xmm2
	vpbroadcastw ymm6{k1}{z}, xmm0
	vpbroadcastw zmm4{k3}{z}, word [0xad0]
	vpbroadcastw xmm5{k7}{z}, ax
	vpbroadcastw xmm2{k4}{z}, esi
	vpbroadcastw xmm3{k2}{z}, rbx
	vpbroadcastw ymm5{k6}{z}, bx
	vpbroadcastw ymm0{k7}{z}, ebp
	vpbroadcastw ymm1{k6}{z}, rbx
	vpbroadcastw zmm3{k1}{z}, bp
	vpbroadcastw zmm2{k5}{z}, edx
	vpbroadcastw zmm7{k6}{z}, rbx
	vpbroadcastw xmm6, word [eax+1]
	vpbroadcastw xmm3, word [eax+64]
	vpbroadcastw ymm2, word [eax+1]
	vpbroadcastw ymm7, word [eax+64]
	vpbroadcastw xmm7, word [eax+1]
	vpbroadcastw xmm1, word [eax+64]
	vpbroadcastw ymm0, word [eax+1]
	vpbroadcastw ymm2, word [eax+64]
	vpbroadcastw zmm7, word [eax+1]
	vpbroadcastw zmm2, word [eax+64]
	vpbroadcastw xmm4, [0x7bf]
	vpbroadcastw ymm0, [0x18c]
	vpbroadcastw xmm6, [0x335]
	vpbroadcastw ymm5, [0x7f1]
	vpbroadcastw zmm2, [0xeb5]
