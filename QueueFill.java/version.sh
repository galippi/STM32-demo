# bash
VERSION=`git rev-parse --short --verify HEAD 2>/dev/null`
# VERSION=`git rev-parse --short --verify HEAD 2>/dev/null 1|tr -d '\n\r' `
#VERSION=`git rev-parse --short --verify HEAD |tr -d '\n\r' `
#echo VERSION=$VERSION mmm >_haha
#echo $VERSION | tr -d '\n\r '>_haha2
#VERSION=`cat _haha2`
#echo VERSION=$VERSION mmm >_haha3
rm -rf src/Version/VersionInfo.java
mkdir src/Version
sed s/git-revision/$VERSION/ <version.template > src/Version/VersionInfo.java
