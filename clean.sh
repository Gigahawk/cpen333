#!/bin/bash

find . -type d -name .vs -exec rm -rf {} +
find . -type f -name *.exe -delete
find . -type f -name *.ilk -delete
find . -type f -name *.pdb -delete



