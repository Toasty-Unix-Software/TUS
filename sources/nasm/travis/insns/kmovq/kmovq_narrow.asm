	kmovq k1, k2
	kmovq k2, k3
	kmovq qword [0x132], k1
	kmovq qword [0x112], k5

%ifdef ERROR
	kmovq k5, rdi
	kmovq k2, rdx
	kmovq rbp, k7
	kmovq rcx, k2
	kmovq k4, r11
	kmovq r10, k3
	kmovq k1, r31
	kmovq r28, k6
%endif
