	kmovb k4, k3
	kmovb k1, k2
	kmovb byte [0xb98], k4
	kmovb byte [0x60c], k7
	kmovb k6, ebp
	kmovb k2, edx
	kmovb k4, bl
	kmovb k3, cl

%ifdef ERROR
	kmovb k6, r11d
	kmovb k7, r11b
	kmovb k5, r21d
	kmovb k3, r24b
%endif
