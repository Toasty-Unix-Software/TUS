	sal al, 1
	sal cl, 1
	sal bx, 1
	sal dx, 1
	sal ecx, 1
	sal ebp, 1
	sal rdi, 1
	sal rbp, 1

%ifdef ERROR
	sal r12b, 1
	sal r11w, 1
	sal r15d, 1
	sal r11, 1
	sal r18b, 1
	sal r21w, 1
	sal r22d, 1
	sal r23, 1
%endif
