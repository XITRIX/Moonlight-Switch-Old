#! /bin/bash
make -j NIGHTLY_BUILD=`git rev-parse --short "$GITHUB_SHA"`