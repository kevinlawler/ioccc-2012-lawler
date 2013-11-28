#assumes ${ENTRY}.c is present in the directory
ENTRY=lawler

DATA=test
LOCAL_FLAGS=

$(shell cp ${ENTRY}.c ${ENTRY}_tmp.c)
OS := $(shell uname -s | tr "[:upper:]" "[:lower:]")

ifeq (freebsd,$(OS))
  $(shell sed "s/con/*i=512;con/g" ${ENTRY}.c > ${ENTRY}_tmp.c)
endif
ifeq (sunos,$(OS))
  # On Solaris tar/find must point to GNU tar/find
  LOCAL_FLAGS = -lsocket
  $(shell sed "s/2, 1/2, 2/g" ${ENTRY}.c > ${ENTRY}_tmp.c)
endif

#compile the test suite
$(shell ${CC} ${DATA}.c -o ${DATA})
#run the test suite
FAILED := $(shell ./${DATA}; echo $$?)

#if the tests fail, use a different source
ifneq ($(FAILED),0)
  $(shell echo "#include <unistd.h>\nint main(){write(1,\"Be sure to buy or emulate a vanilla POSIX machine\\\n\",50);}" > ${ENTRY}_tmp.c)
endif


build: ${ENTRY} 

clean: 
	-${RM} ${ENTRY} ${ENTRY}_tmp.c ${DATA}


clobber: clean


data:


${ENTRY}: ${ENTRY}_tmp.c
	${CC} ${LOCAL_FLAGS} ${ENTRY}_tmp.c -o ${ENTRY}

