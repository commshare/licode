#!/usr/bin/env sh

echo "copy from installMacDeps.sh to install gulp"

set -e

SCRIPT=`pwd`/$0
FILENAME=`basename $SCRIPT`
PATHNAME=`dirname $SCRIPT`
ROOT=$PATHNAME/..
BUILD_DIR=$ROOT/build
CURRENT_DIR=`pwd`
NVM_CHECK="$PATHNAME"/checkNvm.sh
FAST_MAKE=''

LIB_DIR=$BUILD_DIR/libdeps
PREFIX_DIR=$LIB_DIR/build/

###暂时还是重新安装一遍
install_nvm_node() {
  if [ -d $LIB_DIR ]; then
    ##export nvm path to use nvm binary
    export NVM_DIR=$(greadlink -f "$LIB_DIR/nvm")
    echo "NVM_DIR IS $NVM_DIR"
    if [ ! -s "$NVM_DIR/nvm.sh" ]; then
      git clone https://github.com/creationix/nvm.git "$NVM_DIR"
      cd "$NVM_DIR"
      git checkout `git describe --abbrev=0 --tags --match "v[0-9]*" origin`
      cd "$CURRENT_DIR"
    fi
    . $NVM_CHECK
    nvm install
  else
    mkdir -p $LIB_DIR
    install_nvm_node
  fi
}


install_brew_deps(){
 # brew install glib pkg-config boost cmake yasm log4cxx gettext coreutils conan
 install_nvm_node
 #export NVM_DIR=/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm
     echo "NVM_DIR IS $NVM_DIR"

  nvm use
  npm install
  npm install -g node-gyp
  npm install gulp@3.9.1 gulp-eslint@3 run-sequence@2.2.1 webpack-stream@4.0.0 google-closure-compiler-js@20170521.0.0 del@3.0.0 gulp-sourcemaps@2.6.4 script-loader@0.7.2 expose-loader@0.7.5
  #if [ "$DISABLE_SERVICES" != "true" ]; then
    #brew install rabbitmq mongodb
  #fi
}

install_brew_deps

##这样 node_modules/.bin/下会有 gulp这个文件
echo "====done==="