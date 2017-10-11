# usage:
#       ./makeall.sh         - build  all libraries
#       ./makeall.sh  clean  - clean  all directories
#       ./makeall.sh  depend - create all dependencies
#

LIBS="libDMRoot"

for lib in ${LIBS} ; do
    if [ "$1" != "check" ]; then 
       echo 
       echo "make $1 in ${lib} ............."
    fi
    cd ${lib}
    make -j5 $1
    cd ..
done

APPLS="appl/examples"

for appl in ${APPLS} ; do
    if [ "$1" != "check" ]; then
       echo
       echo "make $1 in ${appl} ............"
    fi
    cd ${appl}
    make -j5 $1
    cd ../..
done

if [ "$1" == "" ] ; then
  echo ------------------------------------
  ./makeall.sh check
fi
