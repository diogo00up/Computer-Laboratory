# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/devices
# source code files to be compiled
SRCS = proj.c utils.c game.c graphicscard.c keyboard.c mouse.c timer.c

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic -D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>>
