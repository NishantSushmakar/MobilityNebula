# Conversions needed from NebulaStream to MEOS

- Nebulastream produces a milliseconds timestamp with type long. This is not accepted by MEOS. MEOS expects the timestamp format "%Y-%m-%d %H:%M:%S”(or some variations of this) as a C string.

- MEOS assumes the trajectory (sequence of points) is in order, i.e., all points have a temporal increasing logic. Points with a decreasing logic will cause a set fault on the MEOS side. Nebulastream accepts late points (out-of-order) when using watermarks. Therefore, it is necessary to ensure that the string array sent from Nebulastream to MEOS is ordered. This is a concern for aggregation functions.