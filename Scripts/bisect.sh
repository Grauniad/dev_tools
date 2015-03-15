#!/bin/bash

scriptsDir=$(dirname $0)

git checkout master

git log

echo -n "Last Good Commit: " 
read good

git bisect start
git bisect bad
git bisect good $good

git bisect run $scriptsDir/cleanBuild
