#!/bin/bash
# Copyright (c) 2019, BLAZINGSQL.
###########################################
# BlazingDB CPU conda build script for CI #
###########################################
set -e

# Logger function for build status output
function logger() {
  echo -e "\n>>>> $@\n"
}

# Set home to the job's workspace
export HOME=$WORKSPACE

# Switch to project root; also root of repo checkout
cd $WORKSPACE

# Get latest tag and number of commits since tag
export GIT_DESCRIBE_TAG=`git describe --abbrev=0 --tags`
export GIT_DESCRIBE_NUMBER=`git rev-list ${GIT_DESCRIBE_TAG}..HEAD --count`



#export CUDACXX=/usr/local/cuda/bin/nvcc
#
#CONDA_CH=""
#if [ ! -z "$CONDA_BUILD" ]; then
#    IFS=', ' read -r -a array <<< "$CONDA_BUILD"
#    for item in "${array[@]}"
#    do
#        if [ $item == "blazingsql" ]; then
#            CONDA_CH=$CONDA_CH" -c blazingsql/label/main"
#        else
#            CONDA_CH=$CONDA_CH" -c "$item
#        fi
#    done
#fi
#export CONDA_CH
#
#if [ -z "$CONDA_UPLOAD" ]; then
#    CONDA_UPLOAD="blazingsql"
#fi
#export CONDA_UPLOAD

################################################################################
# SETUP - Check environment
################################################################################

logger "Get env..."
env

conda create python=3.7 -y -n gdf
logger "Activate conda env..."
source activate gdf
conda install conda-build anaconda-client -y

logger "Check versions..."
python --version
gcc --version
g++ --version
conda list

# FIX Added to deal with Anancoda SSL verification issues during conda builds
conda config --set ssl_verify False

################################################################################
# BUILD - Conda package builds
################################################################################

logger "Build conda pkg for blazingsql..."
source ci/cpu/blazingsql/build_blazingsql.sh

logger "Upload conda pkg for blazingsql..."
source ci/cpu/upload_anaconda.sh
