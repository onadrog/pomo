#!/usr/bin/env bash

set -xe

cc -Wall -Wextra -O3 -lraylib -o pomo pomo.c
