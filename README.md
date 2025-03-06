# README

## Overview
This project processes image data to track spherical markers appearing as bright blobs in camera images. The provided data consists of lines of bright pixels extracted from image rows, sorted by their Y and X coordinates. The objective is to group these lines into unique blobs such that all lines belonging to the same blob receive the same index.

## Algorithm Explanation
The algorithm follows these steps:
1. **Load Data**: Reads the input in the cpp file containing extracted bright pixel lines.
2. **Blob Clustering**: Iterates through the sorted lines and assigns blob indices based on touching conditions.
3. **Output Generation**: print the results on the IDE



