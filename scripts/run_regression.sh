#!/usr/bin/env bash
#
# Runs every testbench across several seeds and prints one summary table.
#
#   scripts/run_regression.sh                 # default DUTs and seeds
#   SEEDS="1 2 3 4 5" scripts/run_regression.sh
#   DUTS="fifo alu" CYCLES=20000 scripts/run_regression.sh
#
# Exits non-zero if any run fails, which is what CI keys off.

set -uo pipefail
cd "$(dirname "$0")/.."

DUTS=${DUTS:-"counter fifo arbiter skid_buffer alu"}
SEEDS=${SEEDS:-"1 7 1337 90210"}
CYCLES=${CYCLES:-2000}
LOGDIR=${LOGDIR:-build/logs}

mkdir -p "$LOGDIR"

fail=0
runs=0
failed_runs=()

printf '%-14s %-8s %-8s %-9s %-9s %s\n' DUT SEED RESULT CHECKS COVERAGE LOG
printf -- '---------------------------------------------------------------------\n'

for dut in $DUTS; do
  bin="build/$dut/V$dut"
  if [ ! -x "$bin" ]; then
    echo "missing $bin; run 'make build' first" >&2
    exit 1
  fi

  for seed in $SEEDS; do
    log="$LOGDIR/${dut}_${seed}.log"
    "$bin" --seed "$seed" --cycles "$CYCLES" >"$log" 2>&1
    rc=$?
    runs=$((runs + 1))

    line=$(grep '^VLIB_RESULT' "$log" || true)
    checks=$(grep -oE '[0-9]+ checks' "$log" | tail -1 | cut -d' ' -f1)
    cov=$(sed -n 's/.*coverage=\([0-9.]*\).*/\1/p' <<<"$line")

    if [ $rc -eq 0 ]; then
      result=pass
    else
      result=FAIL
      fail=1
      failed_runs+=("$dut seed=$seed -> $log")
    fi

    printf '%-14s %-8s %-8s %-9s %-9s %s\n' \
      "$dut" "$seed" "$result" "${checks:-?}" "${cov:-?}%" "$log"
  done
done

printf -- '---------------------------------------------------------------------\n'
if [ $fail -eq 0 ]; then
  echo "$runs runs, all passed"
else
  echo "$runs runs, ${#failed_runs[@]} failed:"
  printf '  %s\n' "${failed_runs[@]}"
fi

exit $fail
