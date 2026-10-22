"""Bounded process checks; no claim of exhaustive race detection."""
import subprocess
from collections import Counter

for scheduler in ("fifo", "edf"):
    for n in (0, -1):
        r = subprocess.run(["./codexion", str(n), "1000", "2", "2",
                            "2", "3", "1", scheduler], capture_output=True,
                           timeout=5)
        assert r.returncode != 0
    for n in (1, 2, 4, 5):
        for _ in range(10):
            r = subprocess.run(["./codexion", str(n), "200", "2", "2",
                                "2", "5", "1", scheduler], text=True,
                               capture_output=True, timeout=5, check=True)
            if n == 1:
                assert "burned out" in r.stdout
            else:
                assert "burned out" not in r.stdout, r.stdout
                counts = Counter(int(s.split()[1]) for s in r.stdout.splitlines()
                                 if "is compiling" in s)
                assert len(counts) == n and min(counts.values()) >= 5
    r = subprocess.run(["./codexion", "5", "1", "2", "2", "2", "5",
                        "1", scheduler], capture_output=True, timeout=5)
    assert r.returncode == 0
print("queue/stop unit checks and 86 bounded CLI runs passed")
