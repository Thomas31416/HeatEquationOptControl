#!/usr/bin/env python3

import sys

if len(sys.argv) != 3:
    print('Error: Wrong number of arguments.', file=sys.stderr)
    sys.exit(2)

for (l1, l2) in zip(open(sys.argv[1]), open(sys.argv[2])):
    f1 = [float(x) for x in l1.strip().split()]
    f2 = [float(x) for x in l2.strip().split()]

    if len(f1) == 0 and len(f2) == 0:
        print('')
    elif len(f1) == 4 and len(f2) == 4:
        print(f1[0], f1[1], f1[2], f1[3] - f2[3])
    else:
        print('Error: Wrong input.', file=sys.stderr)
        sys.exit(1)
