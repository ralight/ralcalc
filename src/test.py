#!/usr/bin/env python3

import os
import subprocess
import tempfile

class RalcalcTest:
    def __init__(self, inputs, expected_stdout=None, expected_stderr=None, expected_rc=0, stdin=False):
        self.inputs = inputs
        self.expected_stdout = expected_stdout
        self.expected_stderr = expected_stderr
        self.expected_rc = expected_rc
        self.stdin = stdin

    def report(self, proc, test_type):
        if self.expected_stdout is not None and proc.stdout != self.expected_stdout + "\n":
            raise ValueError(f"Bad {test_type} output {proc.stdout} for {self.inputs}")
        if self.expected_stderr is not None and proc.stderr != self.expected_stderr + "\n":
            raise ValueError(f"Bad {test_type} output {proc.stderr} for {self.inputs}")
        if proc.returncode != self.expected_rc:
            if self.expected_stdout is None:
                print(f"stdout: {proc.stdout}")
            if self.expected_stderr is None:
                print(f"stderr: {proc.stderr}")
            raise ValueError(f"Bad args return code {proc.returncode} for {self.inputs}")

    def run_args(self):
        cmd = ['./ralcalc'] + self.inputs
        proc = subprocess.run(cmd, capture_output=True, text=True, env=self.env)
        self.report(proc, "args")

    def run_stdin(self):
        cmd = ['./ralcalc', '-i']
        stdin = " ".join(self.inputs) + "\n"
        proc = subprocess.run(cmd, input=stdin, capture_output=True, text=True, env=self.env)
        self.report(proc, "stdin")

    def run(self, datadir):
        self.env = os.environ.copy()
        self.env['XDG_CONFIG_HOME'] = datadir
        self.env['XDG_DATA_HOME'] = datadir
        self.env['HOME'] = datadir
        if self.stdin == False:
            self.run_args()
        else:
            self.run_stdin()

tests = [
    RalcalcTest(["-h"], None, stdin=False),
    RalcalcTest(["-a"], None, stdin=False),

    RalcalcTest(["2xpi"], "2xpi = 6.28319"),
    RalcalcTest(["1e33"], "1e33 = 1000Q"),
    RalcalcTest(["1e32"], "1e32 = 100Q"),
    RalcalcTest(["1e31"], "1e31 = 10Q"),
    RalcalcTest(["1e30"], "1e30 = 1Q"),
    RalcalcTest(["1e29"], "1e29 = 100R"),
    RalcalcTest(["1e28"], "1e28 = 10R"),
    RalcalcTest(["1e27"], "1e27 = 1R"),
    RalcalcTest(["1e26"], "1e26 = 100Y"),
    RalcalcTest(["1e25"], "1e25 = 10Y"),
    RalcalcTest(["1e24"], "1e24 = 1Y"),
    RalcalcTest(["1e23"], "1e23 = 100Z"),
    RalcalcTest(["1e22"], "1e22 = 10Z"),
    RalcalcTest(["1e21"], "1e21 = 1Z"),
    RalcalcTest(["1e20"], "1e20 = 100E"),
    RalcalcTest(["1e19"], "1e19 = 10E"),
    RalcalcTest(["1e18"], "1e18 = 1E"),
    RalcalcTest(["1e17"], "1e17 = 100P"),
    RalcalcTest(["1e16"], "1e16 = 10P"),
    RalcalcTest(["1e15"], "1e15 = 1P"),
    RalcalcTest(["1e14"], "1e14 = 100T"),
    RalcalcTest(["1e13"], "1e13 = 10T"),
    RalcalcTest(["1e12"], "1e12 = 1T"),
    RalcalcTest(["1e11"], "1e11 = 100G"),
    RalcalcTest(["1e10"], "1e10 = 10G"),
    RalcalcTest(["1e9"], "1e9 = 1G"),
    RalcalcTest(["1e8"], "1e8 = 100M"),
    RalcalcTest(["1e7"], "1e7 = 10M"),
    RalcalcTest(["1e6"], "1e6 = 1M"),
    RalcalcTest(["1e5"], "1e5 = 100k"),
    RalcalcTest(["1e4"], "1e4 = 10k"),
    RalcalcTest(["1e3"], "1e3 = 1k"),
    RalcalcTest(["1e2"], "1e2 = 100"),
    RalcalcTest(["1e1"], "1e1 = 10"),
    RalcalcTest(["1e0"], "1e0 = 1"),
    RalcalcTest(["1e-1"], "1e-1 = 0.1"),
    RalcalcTest(["1e-2"], "1e-2 = 10m"),
    RalcalcTest(["1e-3"], "1e-3 = 1m"),
    RalcalcTest(["1e-4"], "1e-4 = 100u"),
    RalcalcTest(["1e-5"], "1e-5 = 10u"),
    RalcalcTest(["1e-6"], "1e-6 = 1u"),
    RalcalcTest(["1e-7"], "1e-7 = 100n"),
    RalcalcTest(["1e-8"], "1e-8 = 10n"),
    RalcalcTest(["1e-9"], "1e-9 = 1n"),
    RalcalcTest(["1e-10"], "1e-10 = 100p"),
    RalcalcTest(["1e-11"], "1e-11 = 10p"),
    RalcalcTest(["1e-12"], "1e-12 = 1p"),
    RalcalcTest(["1e-13"], "1e-13 = 100f"),
    RalcalcTest(["1e-14"], "1e-14 = 10f"),
    RalcalcTest(["1e-15"], "1e-15 = 1f"),
    RalcalcTest(["1e-16"], "1e-16 = 100a"),
    RalcalcTest(["1e-17"], "1e-17 = 10a"),
    RalcalcTest(["1e-18"], "1e-18 = 1a"),
    RalcalcTest(["1e-19"], "1e-19 = 100z"),
    RalcalcTest(["1e-20"], "1e-20 = 10z"),
    RalcalcTest(["1e-21"], "1e-21 = 1z"),
    RalcalcTest(["1e-22"], "1e-22 = 100y"),
    RalcalcTest(["1e-23"], "1e-23 = 10y"),
    RalcalcTest(["1e-24"], "1e-24 = 1y"),
    RalcalcTest(["1e-25"], "1e-25 = 100r"),
    RalcalcTest(["1e-26"], "1e-26 = 10r"),
    RalcalcTest(["1e-27"], "1e-27 = 1r"),
    RalcalcTest(["1e-28"], "1e-28 = 100q"),
    RalcalcTest(["1e-29"], "1e-29 = 10q"),
    RalcalcTest(["1e-30"], "1e-30 = 1q"),
    RalcalcTest(["1e-31"], "1e-31 = 0.1q"),
    RalcalcTest(["1e-32"], "1e-32 = 0.01q"),
    RalcalcTest(["1e-33"], "1e-33 = 0.001q"),
    RalcalcTest(["1e-34"], "1e-34 = 0.0001q"),

    RalcalcTest(["1Qx1q"], "1Qx1q = 1"),
    RalcalcTest(["1Rx1r"], "1Rx1r = 1"),
    RalcalcTest(["1Yx1y"], "1Yx1y = 1"),
    RalcalcTest(["1Zx1z"], "1Zx1z = 1"),
    RalcalcTest(["1Ex1a"], "1Ex1a = 1"),
    RalcalcTest(["1Px1f"], "1Px1f = 1"),
    RalcalcTest(["1Tx1p"], "1Tx1p = 1"),
    RalcalcTest(["1Gx1n"], "1Gx1n = 1"),
    RalcalcTest(["1Mx1u"], "1Mx1u = 1"),
    RalcalcTest(["1kx1m"], "1kx1m = 1"),

    RalcalcTest(["-s", "k", "1"], "1 = 0.001k"),

    RalcalcTest(["exp"], "exp = 2.71828"),
    RalcalcTest(["pi"], "pi = 3.14159"),

    RalcalcTest(["-r", "sin0"], "sin0 = 0.000000"),
    RalcalcTest(["-r", "cos0"], "cos0 = 1.000000"),
    RalcalcTest(["-r", "tan0"], "tan0 = 0.000000"),

    RalcalcTest(["-r", "asin0"], "asin0 = 0.000000"),
    RalcalcTest(["-r", "acos1"], "acos1 = 0.000000"),
    RalcalcTest(["-r", "atan0"], "atan0 = 0.000000"),

    RalcalcTest(["-r", "log0.5"], "log0.5 = -0.301030"),
    RalcalcTest(["-r", "ln0.5"], "ln0.5 = -0.693147"),

    RalcalcTest(["-r", "sqrt2"], "sqrt2 = 1.414214"),
    RalcalcTest(["-r", "2^2"], "2^2 = 4.000000"),

    # Exponent output
    RalcalcTest(["-e", "1n"], "1n = 1e-09"),
    RalcalcTest(["-e", "-p", "2", "pi/100000"], "pi/100000 = 3.1e-05"),

    # Raw output
    RalcalcTest(["-r", "-p", "5", "pi/100000"], "pi/100000 = 0.00003"),

    # Implicit tokens
    RalcalcTest(["-(1+2)"], "-(1+2) = -3"),
    RalcalcTest(["-[1+2]"], "-[1+2] = -3"),
    RalcalcTest(["-[1-2]"], "-[1-2] = 1"),
    RalcalcTest(["[1+2][3+4]"], "[1+2][3+4] = 21"),
    RalcalcTest(["[1+2]log2"], "[1+2]log2 = 0.90309"),


    # Precision
    RalcalcTest(["-p", "1", "pi"], "pi = 3"),
    RalcalcTest(["-p", "2", "pi"], "pi = 3.1"),
    RalcalcTest(["-p", "3", "pi"], "pi = 3.14"),
    RalcalcTest(["-p", "4", "pi"], "pi = 3.142"),
    RalcalcTest(["-p", "5", "pi"], "pi = 3.1416"),
    RalcalcTest(["-p", "6", "pi"], "pi = 3.14159"),
    RalcalcTest(["-p", "7", "pi"], "pi = 3.141593"),
    RalcalcTest(["-p", "8", "pi"], "pi = 3.1415927"),
    RalcalcTest(["-p", "9", "pi"], "pi = 3.14159265"),
    RalcalcTest(["-p", "10", "pi"], "pi = 3.141592654"),
    RalcalcTest(["-p", "11", "pi"], "pi = 3.1415926536"),
    RalcalcTest(["-p"], None),
    RalcalcTest(["-f"], None),

    # Division
    RalcalcTest(["3/1.5"], "3/1.5 = 2"),

    # Negation
    RalcalcTest(["-0.1"], "-0.1 = -0.1"),

    # Mod
    RalcalcTest(["5%1.3"], "5%1.3 = 1.1"),

    # Quiet
    RalcalcTest(["-q", "5%1.3"], "1.1"),

    # File input
    RalcalcTest(["-f", "test.data"], "2x2 = 4\n1/sqrt2 = 0.707107"),

    # Stdin
    RalcalcTest(["sqrt3"], "sqrt3 = 1.73205", stdin=True),

    # Errors
    RalcalcTest(["h"], expected_stderr="\nError: h\n       ^ unknown token", expected_rc=1),
    RalcalcTest(["e"], expected_stderr="\nError: e\n       ^ bad number", expected_rc=1),
    RalcalcTest(["2pi"], expected_stderr="\nError: 2pi\n          ^ duplicate number", expected_rc=1),
    RalcalcTest(["[2+]"], expected_stderr="\nError: [2+]\n          ^ invalid bracket", expected_rc=1),
    RalcalcTest(["1+2/"], expected_stderr="\nError: 1+2/\n          ^ invalid operator", expected_rc=1),
    RalcalcTest(["[1"], expected_stderr="\nError: [1\n         ^ mismatched brackets", expected_rc=1),
    RalcalcTest(["-s", "QR", "2"], expected_stderr="Error: Invalid SI prefix 'QR' for '-s' option.", expected_rc = 1),
    RalcalcTest(["-s", "5", "2"], expected_stderr="Error: Invalid SI prefix '5' for '-s' option.", expected_rc = 1),
    RalcalcTest(["-s"], None),
    RalcalcTest(["-f", "a/b/c/d/e/g/h"], expected_stderr="Error: Unable to open file \"a/b/c/d/e/g/h\"", expected_rc = 1),
    RalcalcTest(["-p", "-1"], expected_stderr="Error: Precision must be a positive integer.", expected_rc = 1),
    RalcalcTest(["[1]]"], expected_stderr="\nError: [1]]\n          ^ invalid bracket\n\nError: [1]]\n           ^ mismatched brackets", expected_rc=1),
    RalcalcTest(["lo"], expected_stderr="\nError: lo\n       ^ unknown token\n\nError: lo\n        ^ unknown token", expected_rc=1),

    # Last value
    RalcalcTest(["1", "/", "sqrt2"], "1 / sqrt2 = 0.707107"),
    RalcalcTest(["_x2"], "_x2 = 1.41421"),

]


with tempfile.TemporaryDirectory() as cfgdir:
    for test in tests:
        test.run(cfgdir)

    with open(f"{cfgdir}/ralcalcrc", "w") as f:
        f.write("-e\n")
    test = RalcalcTest(["1u"], "1u = 1e-06")
    test.run(cfgdir)

    with open(f"{cfgdir}/ralcalcrc", "w") as f:
        f.write("-r\n")
    test = RalcalcTest(["1u"], "1u = 0.000001")
    test.run(cfgdir)

    with open(f"{cfgdir}/ralcalcrc", "w") as f:
        f.write("-s n\n")
    test = RalcalcTest(["1u"], "1u = 1000n")
    test.run(cfgdir)
