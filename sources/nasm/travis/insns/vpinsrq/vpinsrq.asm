default rel
	vpinsrq xmm3, xmm5, qword [0xa5c], 0xb1
	vpinsrq xmm3, qword [0xa5c], 0xb1
	vpinsrq xmm7, xmm0, qword [0x898], 0xc4
	vpinsrq xmm7, xmm5, rdx, 0xcc
	vpinsrq xmm7, rdx, 0xcc
	vpinsrq xmm4, xmm3, rsi, 0x10
	vpinsrq xmm12, xmm13, qword [0x21c], 0x57
	vpinsrq xmm14, xmm13, r9, 0xaa
	vpinsrq xmm19, xmm20, qword [0x131], 0x4d
	vpinsrq xmm19, xmm23, r31, 0xdb
	vpinsrq xmm4, xmm4, qword [eax+1], 0x7a
	vpinsrq xmm7, xmm0, qword [eax+64], 0x6e
	vpinsrq xmm7, xmm4, qword [eax+1], 0x8c
	vpinsrq xmm5, xmm2, qword [eax+64], 0x7f
	vpinsrq xmm2, xmm5, [0xa0c], 0xb
	vpinsrq xmm6, xmm1, [0x697], 0x9d
