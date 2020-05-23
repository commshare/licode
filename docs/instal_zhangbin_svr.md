#

## common里依赖了zlib
- macos 安装
```cpp
 ✘ zhangbin@zhangbin-mbp  ~/tet/licodelllcode/licode   branch-pre-v8.4 ●  brew install zlib

Updating Homebrew...
==> Auto-updated Homebrew!
Updated 2 taps (homebrew/core and homebrew/cask).
==> New Formulae
kind                       liblouis                   sentencepiece
==> Updated Formulae
amazon-ecs-cli  fabio           gradle          open-zwave      terraform
byteman         firebase-cli    imagemagick@6   qpdf            tunnel
docker-compose  fonttools       nats-server     qstat           virtuoso
doctl           gatsby-cli      newman          skaffold        whois
erlang@20       git-revise      opa             stellar-core
==> Deleted Formulae
erlang@19                                ruby@2.0

==> Downloading https://homebrew.bintray.com/bottles/zlib-1.2.11.catalina.bottle
######################################################################## 100.0%
==> Pouring zlib-1.2.11.catalina.bottle.tar.gz
==> Caveats
zlib is keg-only, which means it was not symlinked into /usr/local,
because macOS already provides this software and installing another version in
parallel can cause all kinds of trouble.

For compilers to find zlib you may need to set:
  export LDFLAGS="-L/usr/local/opt/zlib/lib"
  export CPPFLAGS="-I/usr/local/opt/zlib/include"

For pkg-config to find zlib you may need to set:
  export PKG_CONFIG_PATH="/usr/local/opt/zlib/lib/pkgconfig"

==> Summary
🍺  /usr/local/Cellar/zlib/1.2.11: 12 files, 376.4KB
 zhangbin@zhangbin-mbp  ~/tet/licodelllcode/licode   branch-pre-v8.4 ●  

```