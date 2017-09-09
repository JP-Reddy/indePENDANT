#!/bin/bash -f
# Soft environment setup

export PROJ_ROOT=`dirname $0`

export PATH=/cygdrive/c/csdtk/cygwin/bin:/usr/bin:/crosscompiler/bin:/cygdrive/c/csdtk/cooltools:/usr/local/bin:/cygdrive/c/Program\ Files/Xoreax/IncrediBuild:

export USE_GCC_4=1
export ENABLE_GC_SECTIONS=1

cd  ${PROJ_ROOT}
export PROJ_WORKDIR=`pwd`

# Soft directory location
export SOFT_WORKDIR=$PROJ_WORKDIR
echo ${SOFT_WORKDIR}

cd $SOFT_WORKDIR
bash
#echo Project Switched to "${SOFT_WORKDIR}"
#make
