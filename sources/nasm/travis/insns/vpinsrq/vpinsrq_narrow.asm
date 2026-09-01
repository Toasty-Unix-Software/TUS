	vpinsrq xmm3, xmm5, qword [0xa5c], 0xb1
	vpinsrq xmm3, qword [0xa5c], 0xb1
	vpinsrq xmm7, xmm0, qword [0x898], 0xc4
	vpinsrq xmm7, xmm5, rdx, 0xcc
	vpinsrq xmm7, rdx, 0xcc
	vpinsrq xmm4, xmm3, rsi, 0x10

%ifdef ERROR
	vpinsrq xmm12, xmm13, qword [0x21c], 0x57
	vpinsrq xmm14, xmm13, r9, 0xaa
	vpinsrq xmm19, xmm20, qword [0x131], 0x4d
	vpinsrq xmm19, xmm23, r31, 0xdb
%endif
