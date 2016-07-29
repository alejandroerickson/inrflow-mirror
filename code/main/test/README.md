### Test directory

Pass a file of `inrflow` arguments (as an argument) to one of the test scripts,
`./gnu_parallel.sh`, `.\parallel_test.sh`, or `serial_test.sh`, listed in order of
preference.  The scripts will ask for confirmation to continue, and gives the
option not to compile with debug flags.  If called without arguments, the scripts
use the output of `gen_test_args.sh`.  Test scripts will not work outside this
directory.

### Example call:

    ./gnu_parallel_test.sh swcube/small_swcube_test.args

### File names and directory hierarchy

All tests output to a unique directory `inrflow.pid.d`, where `pid` is the pid of
the test (test exits if `inrflow.pid.d` exists).  Within that directory, 

* `./gnu_parallel_test.sh` pipes meta output to test.log.
* `./parallel_test.sh` pipes meta output to `n.meta`.
* `./serial_test.sh` prints direct to `stdout`.

Files with arguments for `inrflow` (one per line) have the suffix `.args`, and
various topology-specific tests are stored in each directory `topo/`.

### Installing GNU Parallel

GNU Parallel is the best choice for use on a distributed system, and it can be
installed without administrator rights.  This script will install it on most *nix
systems in about 10 seconds, attempting first to install system-wide, and so on.

    (wget -O - pi.dk/3 || curl pi.dk/3/ || fetch -o - http://pi.dk/3) | bash
