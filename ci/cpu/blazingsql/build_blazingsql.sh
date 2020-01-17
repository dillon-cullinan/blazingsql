#!/bin/bash
set -e

echo "CMD: conda build  -c blazingsql-nightly -c rapidsai-nightly -c nvidia -c conda-forge -c defaults conda/recipes/blazingsql --python=$PYTHON"
conda build  -c blazingsql-nightly -c rapidsai-nightly -c nvidia -c conda-forge -c defaults conda/recipes/blazingsql --python=$PYTHON

