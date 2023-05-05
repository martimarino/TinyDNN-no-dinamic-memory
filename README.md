# Machine Learning test on the EPI processor

This repository contains Machine Learning testing codes for
the new EPI processor.

In particular, it contains the C++ source code for 
**deep neural networks** (DNNs).

We have started from the freely available library **tinyDNN**,
than we have adapted to compile on ARM and to exploit ARM SVE2 feature.

We are able to both train and query a DNN, load it from disk or save
it to disk.

Being this machine learning application very heavy as time complexity,
we believe it is a good benchmark for the EPI processor.

In the future we might add other **machine learning** algorithms
(both C++ source code, datasets and documentation).
