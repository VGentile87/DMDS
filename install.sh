#!/bin/bash
# Installation script for the fedra-package   #
# 14.11.03                                    #

installdir=`pwd`
if [[ ${installdir##*/} == "src" ]] ; then
 installdir=`cd ..; pwd`
fi
export installdir=$installdir

# making directories, if not already existing
if [[ ! -a $installdir/bin ]] ; then
  mkdir $installdir/bin
fi
if [[ ! -a $installdir/include ]] ; then
  mkdir $installdir/include
fi
if [[ ! -a $installdir/lib ]] ; then
  mkdir $installdir/lib
fi

# Define links from include-dir to src-files; former "setlinks.sh"
PROJECT_INC=$installdir/include
PROJECT_SRC=$installdir/src

LIBS="libDMRoot"

for lib in ${LIBS} ; do
    echo "set links for ${lib} ..."
    listLib=`ls $installdir/src/${lib}/*.h`
    for hname in ${listLib} ; do
	ln -fs $PROJECT_SRC/${lib}/${hname##*/} $PROJECT_INC/${hname##*/}
    done
done

# create ProjectDef.mk in ./config-directory
ProjectDef="$installdir/src/config/ProjectDef.mk"

echo "PROJECT_ROOT=$installdir"  > $ProjectDef
echo >> $ProjectDef
echo 'BIN_DIR = $(PROJECT_ROOT)/bin' >> $ProjectDef
echo 'LIB_DIR = $(PROJECT_ROOT)/lib' >> $ProjectDef
echo 'INC_DIR = $(PROJECT_ROOT)/include' >> $ProjectDef
echo >> $ProjectDef
#echo 'PROJECT_LIBS = -L$(LIB_DIR)' >> $ProjectDef

# create RootDef.mk and TargetsDef.mk links
configdir="$installdir/src/config"
ln -fs $configdir/RootDef.linux.mk $configdir/RootDef.mk 
ln -fs $configdir/TargetsDef.linux.mk $configdir/TargetsDef.mk 

# create setup_new.sh, depending on default-shell used
usedshell=`echo $SHELL`
if [[ (${usedshell##*/} == "tcsh") ||  (${usedshell##*/} == "csh") ]] ; then 
 echo 'setenv  DM_ROOT '"$installdir" > $installdir/setup_new.sh
 echo 'setenv  LD_LIBRARY_PATH $DM_ROOT/lib:${LD_LIBRARY_PATH}' >> $installdir/setup_new.sh
 echo 'setenv  PATH ${PATH}:$DM_ROOT/bin' >> $installdir/setup_new.sh
fi

if [[ (${usedshell##*/} == "bash") ||  ${usedshell##*/} == "sh" || ${usedshell##*/} == "ksh" ]] ; then
 echo 'export DM_ROOT='"$installdir" > $installdir/setup_new.sh
 echo 'export LD_LIBRARY_PATH=$DM_ROOT/lib:${LD_LIBRARY_PATH}' >> $installdir/setup_new.sh
 echo 'export PATH=${PATH}:$DM_ROOT/bin' >> $installdir/setup_new.sh
fi
export DM_ROOT="$installdir"
export LD_LIBRARY_PATH=$DM_ROOT/lib:${LD_LIBRARY_PATH}
export PATH=${PATH}:$DM_ROOT/bin

# compilation of libraries
#echo "Do you want to compile the libraries: [y/n]"
#read yesno
#if [[ $yesno == 'y' ]] ; then
  chmod u+x $installdir/src/makeall.sh
  cd $installdir/src
  $installdir/src/makeall.sh
#fi

# install fedra macros
#cp -rf $PROJECT_SRC/appl/macros $installdir/macros
cd $installdir



