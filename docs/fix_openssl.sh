#!/bin/sh
#https://gist.github.com/llbbl/c54f44d028d014514d5d837f64e60bac
echo 'copy two openssl lib files'

cd /usr/local/Cellar/openssl/1.0.2t/lib

sudo cp libssl.1.0.0.dylib libcrypto.1.0.0.dylib /usr/local/lib/

cd /usr/local/lib

echo 'backup existing lib files, if they exist'

mv libssl.dylib libssl_bak.dylib

mv libcrypto.dylib libcrypto_bak.dylib

echo 'add symlink to missing openssl libs'

sudo ln -s libssl.1.0.0.dylib libssl.dylib

sudo ln -s libcrypto.1.0.0.dylib libcrypto.dylib
