ifeq (${KERNELRELEASE},)
	KERNEL_SOURCE := /lib/modules/$(shell uname -r)/build/
	PWD := $(shell pwd)
default:
	${MAKE} -C ${KERNEL_SOURCE} M=${PWD} modules

install:
	${MAKE} -C ${KERNEL_SOURCE} M=${PWD} modules_install
	depmod

clean:
	${MAKE} -C ${KERNEL_SOURCE} M=${PWD} clean

else
	obj-m := customdevice.o
endif

prog:
	gcc userspace.c -o userspace
