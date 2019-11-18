
#cmake没找到我的boost 1.7.1

##
```
find /usr/local/lib -name '*[Bb]oost*'
```
- /usr/local/Cellar/boost/

```
 ✘ zhangbin@zhangbin-mbp  ~/tet/licodelllcode/licode   branch-pre-v8.4 ●  brew search boost
==> Formulae
boost ✔             boost-mpi           boost-python@1.59   boost@1.59
boost-bcp           boost-python        boost@1.55          boost@1.60
boost-build         boost-python3       boost@1.57          xgboost

==> Casks
boostnote                  nosqlbooster-for-mongodb   turbo-boost-switcher
focus-booster              soundboosterlite

```

## cmake
- find_package
- https://github.com/Kitware/CMake/blob/3a77329c40803b02fe33ffc86197c1baf291aaff/Modules/FindBoost.cmake#L827

```
message(STATUS "---------------------")
message(STATUS "Boost_FOUND: ${Boost_FOUND}")
message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
message(STATUS "---------------------")
```

- cmake支持查boost么？
 - http://www.yeolar.com/note/2014/12/16/cmake-how-to-find-libraries/
```

cmake --help-module FindBoost

```

## 卸载1.71.0
```$xslt
brew uninstall boost


Uninstalling /usr/local/Cellar/boost/1.71.0... (14,377 files, 608.4MB)

```
##安装1.69 ，不好弄啊ß
```$xslt
 ✘ zhangbin@zhangbin-mbp  ~/tet/licodelllcode/licode   branch-pre-v8.4 ●  brew install boost@1.69.0
Error: No available formula with the name "boost@1.69.0" 
==> Searching for a previously deleted formula (in the last month)...
Warning: homebrew/core is shallow clone. To get complete history run:
  git -C "$(brew --repo homebrew/core)" fetch --unshallow

Error: No previously deleted formula found.
==> Searching for similarly named formulae...
Error: No similarly named formulae found.
==> Searching taps...
==> Searching taps on GitHub...
Error: No formulae found in taps.
 ✘ zhangbin@zhangbin-mbp  ~/tet/licodelllcode/licode   branch-pre-v8.4 ●  

```