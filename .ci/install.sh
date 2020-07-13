#!/usr/bin/env bash

set -ex

if [[ "$(uname -s)" == 'Linux' ]]; then
    sudo apt-get -y update
    sudo apt-get -y install libboost-test-dev python3-pip python3-setuptools
fi
python3 -m pip install --user pyyaml
