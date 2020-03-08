#!/usr/bin/env bash

set -ex

if [[ "$(uname -s)" == 'Linux' ]]; then
    sudo apt-get -y update
    sudo apt-get -y install libboost-test-dev
fi
