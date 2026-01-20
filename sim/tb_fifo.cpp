#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <queue>
#include "Vfifo.h"
#include "verilated.h"

// Zig reference model functions
extern "C" {
    void fifo_init();
    void fifo_tick();
    void fifo_set_reset(bool rst_n);
    void fifo_set_wr_en(bool wr_en);
    void fifo_set_rd_en(bool rd_en);
    void fifo_set_data_in(unsigned char data);
    unsigned char fifo_get_data_out();
    bool fifo_get_full();
    bool fifo_get_empty();
    size_t fifo_get_count();
}

// =============================================================================
// Latency Checker - Tracks write-to-read latency for data through the FIFO
// =============================================================================
class LatencyChecker {
private:
    struct Transaction {
        unsigned char data;
        int write_cycle;
    };
    std::queue<Transaction> pending;
    int total_transactions;
    int total_latency;
    int min_latency;
    int max_latency;
    int latency_violations;
    int max_allowed_latency;

public:
    LatencyChecker(int max_latency_cycles) :
        total_transactions(0),
        total_latency(0),
        min_latency(999999),
        max_latency(0),
        latency_violations(0),
        max_allowed_latency(max_latency_cycles) {}

    void record_write(unsigned char data, int cycle) {
        Transaction t = {data, cycle};
        pending.push(t);
    }

    bool check_read(unsigned char data, int cycle) {
        if (pending.empty()) {
            printf("  [LATENCY ERROR] Read with no pending write!\n");
            return false;
        }

        Transaction t = pending.front();
        pending.pop();

        if (t.data != data) {
            printf("  [LATENCY ERROR] Data mismatch: expected %d, got %d\n", t.data, data);
            return false;
        }

        int latency = cycle - t.write_cycle;
        total_transactions++;
        total_latency += latency;

        if (latency < min_latency) min_latency = latency;
        if (latency > max_latency) max_latency = latency;

        if (latency > max_allowed_latency) {
            printf("  [LATENCY VIOLATION] Data %d took %d cycles (max: %d)\n",
                   data, latency, max_allowed_latency);
            latency_violations++;
            return false;
        }

        return true;
    }

    void print_report() {
        printf("\n========== Latency Report ==========\n");
        printf("Total transactions: %d\n", total_transactions);
        if (total_transactions > 0) {
            printf("Min latency: %d cycles\n", min_latency);
            printf("Max latency: %d cycles\n", max_latency);
            printf("Avg latency: %.2f cycles\n", (float)total_latency / total_transactions);
        }
        printf("Latency violations: %d\n", latency_violations);
        printf("Max allowed latency: %d cycles\n", max_allowed_latency);
    }

    int get_violations() { return latency_violations; }
};

// =============================================================================
// Functional Coverage Tracker
// =============================================================================
class CoverageTracker {
private:
    bool seen_empty;
    bool seen_full;
    bool seen_write_when_full;
    bool seen_read_when_empty;
    bool seen_simultaneous_rw;
    bool seen_rollover;
    int  count_bins[9];  // 0-8 for depth=8

public:
    CoverageTracker() :
        seen_empty(false), seen_full(false),
        seen_write_when_full(false), seen_read_when_empty(false),
        seen_simultaneous_rw(false), seen_rollover(false) {
        for (int i = 0; i < 9; i++) count_bins[i] = 0;
    }

    void sample(bool empty, bool full, int count, bool wr_en, bool rd_en) {
        if (empty) seen_empty = true;
        if (full) seen_full = true;
        if (wr_en && full) seen_write_when_full = true;
        if (rd_en && empty) seen_read_when_empty = true;
        if (wr_en && rd_en) seen_simultaneous_rw = true;
        if (count >= 0 && count <= 8) count_bins[count]++;
    }

    void record_rollover() { seen_rollover = true; }

    void print_report() {
        printf("\n========== Coverage Report ==========\n");
        printf("Empty state:           %s\n", seen_empty ? "HIT" : "MISS");
        printf("Full state:            %s\n", seen_full ? "HIT" : "MISS");
        printf("Write when full:       %s\n", seen_write_when_full ? "HIT" : "MISS");
        printf("Read when empty:       %s\n", seen_read_when_empty ? "HIT" : "MISS");
        printf("Simultaneous R/W:      %s\n", seen_simultaneous_rw ? "HIT" : "MISS");
        printf("Pointer rollover:      %s\n", seen_rollover ? "HIT" : "MISS");
        printf("\nCount distribution:\n");
        for (int i = 0; i <= 8; i++) {
            printf("  count=%d: %d samples\n", i, count_bins[i]);
        }

        int hits = (seen_empty ? 1 : 0) + (seen_full ? 1 : 0) +
                   (seen_write_when_full ? 1 : 0) + (seen_read_when_empty ? 1 : 0) +
                   (seen_simultaneous_rw ? 1 : 0) + (seen_rollover ? 1 : 0);
        printf("\nCoverage: %d/6 bins hit (%.1f%%)\n", hits, hits * 100.0 / 6);
    }

    int get_coverage_percent() {
        int hits = (seen_empty ? 1 : 0) + (seen_full ? 1 : 0) +
                   (seen_write_when_full ? 1 : 0) + (seen_read_when_empty ? 1 : 0) +
                   (seen_simultaneous_rw ? 1 : 0) + (seen_rollover ? 1 : 0);
        return hits * 100 / 6;
    }
};

// =============================================================================
// Clock cycle helper
// =============================================================================
void tick(Vfifo* dut, int& cycle) {
    dut->clk = 1;
    dut->eval();
    fifo_tick();
    dut->clk = 0;
    dut->eval();
    cycle++;
}

// =============================================================================
// Compare RTL vs Reference Model
// =============================================================================
int compare_outputs(Vfifo* dut, int cycle) {
    int errors = 0;

    unsigned char rtl_data_out = dut->data_out;
    unsigned char ref_data_out = fifo_get_data_out();
    bool rtl_full = dut->full;
    bool ref_full = fifo_get_full();
    bool rtl_empty = dut->empty;
    bool ref_empty = fifo_get_empty();
    int rtl_count = dut->count;
    size_t ref_count = fifo_get_count();

    if (rtl_full != ref_full) {
        printf("  [MISMATCH] Cycle %d: full - RTL=%d, REF=%d\n", cycle, rtl_full, ref_full);
        errors++;
    }
    if (rtl_empty != ref_empty) {
        printf("  [MISMATCH] Cycle %d: empty - RTL=%d, REF=%d\n", cycle, rtl_empty, ref_empty);
        errors++;
    }
    if (rtl_count != (int)ref_count) {
        printf("  [MISMATCH] Cycle %d: count - RTL=%d, REF=%zu\n", cycle, rtl_count, ref_count);
        errors++;
    }
    // Only compare data_out when not empty
    if (!rtl_empty && rtl_data_out != ref_data_out) {
        printf("  [MISMATCH] Cycle %d: data_out - RTL=%d, REF=%d\n", cycle, rtl_data_out, ref_data_out);
        errors++;
    }

    return errors;
}

// =============================================================================
// Main Testbench
// =============================================================================
int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    srand(time(NULL));

    Vfifo* dut = new Vfifo;
    fifo_init();

    LatencyChecker latency(20);  // Max 20 cycles latency allowed
    CoverageTracker coverage;

    int cycle = 0;
    int total_errors = 0;
    int writes_completed = 0;
    int reads_completed = 0;

    printf("==============================================\n");
    printf("  FIFO Verification with Latency Checking\n");
    printf("==============================================\n\n");

    // -------------------------------------------------------------------------
    // Reset Sequence
    // -------------------------------------------------------------------------
    printf("[TEST] Reset sequence...\n");
    dut->clk = 0;
    dut->rst_n = 0;
    dut->wr_en = 0;
    dut->rd_en = 0;
    dut->data_in = 0;
    dut->eval();

    fifo_set_reset(false);
    fifo_set_wr_en(false);
    fifo_set_rd_en(false);
    fifo_set_data_in(0);

    for (int i = 0; i < 5; i++) {
        tick(dut, cycle);
    }

    // Release reset
    dut->rst_n = 1;
    fifo_set_reset(true);
    dut->eval();

    total_errors += compare_outputs(dut, cycle);
    printf("  Reset complete. FIFO should be empty.\n");
    printf("  RTL: empty=%d, full=%d, count=%d\n", dut->empty, dut->full, dut->count);

    // -------------------------------------------------------------------------
    // Test 1: Basic Write/Read
    // -------------------------------------------------------------------------
    printf("\n[TEST] Basic write/read sequence...\n");

    // Write 4 items
    for (int i = 0; i < 4; i++) {
        unsigned char data = i + 1;
        dut->wr_en = 1;
        dut->data_in = data;
        fifo_set_wr_en(true);
        fifo_set_data_in(data);

        tick(dut, cycle);
        latency.record_write(data, cycle);
        writes_completed++;

        dut->wr_en = 0;
        fifo_set_wr_en(false);

        total_errors += compare_outputs(dut, cycle);
        coverage.sample(dut->empty, dut->full, dut->count, true, false);
        printf("  Write %d: count=%d\n", data, dut->count);
    }

    // Read 4 items
    for (int i = 0; i < 4; i++) {
        dut->rd_en = 1;
        fifo_set_rd_en(true);

        unsigned char data_before_read = dut->data_out;
        tick(dut, cycle);
        latency.check_read(data_before_read, cycle);
        reads_completed++;

        dut->rd_en = 0;
        fifo_set_rd_en(false);

        total_errors += compare_outputs(dut, cycle);
        coverage.sample(dut->empty, dut->full, dut->count, false, true);
        printf("  Read %d: count=%d\n", data_before_read, dut->count);
    }

    // -------------------------------------------------------------------------
    // Test 2: Fill to Full
    // -------------------------------------------------------------------------
    printf("\n[TEST] Fill FIFO to full...\n");

    for (int i = 0; i < 8; i++) {
        unsigned char data = 100 + i;
        dut->wr_en = 1;
        dut->data_in = data;
        fifo_set_wr_en(true);
        fifo_set_data_in(data);

        tick(dut, cycle);
        if (!dut->full) {
            latency.record_write(data, cycle);
            writes_completed++;
        }

        total_errors += compare_outputs(dut, cycle);
        coverage.sample(dut->empty, dut->full, dut->count, true, false);
    }

    dut->wr_en = 0;
    fifo_set_wr_en(false);
    printf("  FIFO full=%d, count=%d\n", dut->full, dut->count);

    // Try to write when full
    printf("\n[TEST] Write when full (should be ignored)...\n");
    dut->wr_en = 1;
    dut->data_in = 0xFF;
    fifo_set_wr_en(true);
    fifo_set_data_in(0xFF);
    tick(dut, cycle);
    coverage.sample(dut->empty, dut->full, dut->count, true, false);
    total_errors += compare_outputs(dut, cycle);
    printf("  After write attempt: count=%d (should still be 8)\n", dut->count);
    dut->wr_en = 0;
    fifo_set_wr_en(false);

    // -------------------------------------------------------------------------
    // Test 3: Drain to Empty
    // -------------------------------------------------------------------------
    printf("\n[TEST] Drain FIFO to empty...\n");

    while (!dut->empty) {
        dut->rd_en = 1;
        fifo_set_rd_en(true);

        unsigned char data_out = dut->data_out;
        tick(dut, cycle);
        latency.check_read(data_out, cycle);
        reads_completed++;

        total_errors += compare_outputs(dut, cycle);
        coverage.sample(dut->empty, dut->full, dut->count, false, true);
    }

    dut->rd_en = 0;
    fifo_set_rd_en(false);
    printf("  FIFO empty=%d, count=%d\n", dut->empty, dut->count);

    // Try to read when empty
    printf("\n[TEST] Read when empty (should be ignored)...\n");
    dut->rd_en = 1;
    fifo_set_rd_en(true);
    tick(dut, cycle);
    coverage.sample(dut->empty, dut->full, dut->count, false, true);
    total_errors += compare_outputs(dut, cycle);
    printf("  After read attempt: empty=%d (should still be 1)\n", dut->empty);
    dut->rd_en = 0;
    fifo_set_rd_en(false);

    // -------------------------------------------------------------------------
    // Test 4: Simultaneous Read/Write
    // -------------------------------------------------------------------------
    printf("\n[TEST] Simultaneous read/write...\n");

    // First put some data in
    for (int i = 0; i < 4; i++) {
        unsigned char data = 50 + i;
        dut->wr_en = 1;
        dut->data_in = data;
        fifo_set_wr_en(true);
        fifo_set_data_in(data);
        tick(dut, cycle);
        latency.record_write(data, cycle);
        writes_completed++;
        total_errors += compare_outputs(dut, cycle);
    }
    dut->wr_en = 0;
    fifo_set_wr_en(false);

    // Now do simultaneous R/W
    printf("  Before: count=%d\n", dut->count);
    for (int i = 0; i < 4; i++) {
        unsigned char new_data = 60 + i;
        unsigned char read_data = dut->data_out;

        dut->wr_en = 1;
        dut->rd_en = 1;
        dut->data_in = new_data;
        fifo_set_wr_en(true);
        fifo_set_rd_en(true);
        fifo_set_data_in(new_data);

        tick(dut, cycle);
        latency.record_write(new_data, cycle);
        latency.check_read(read_data, cycle);
        writes_completed++;
        reads_completed++;

        total_errors += compare_outputs(dut, cycle);
        coverage.sample(dut->empty, dut->full, dut->count, true, true);
        printf("  Simultaneous R/W: wrote %d, read %d, count=%d\n", new_data, read_data, dut->count);
    }
    dut->wr_en = 0;
    dut->rd_en = 0;
    fifo_set_wr_en(false);
    fifo_set_rd_en(false);

    // -------------------------------------------------------------------------
    // Test 5: Randomized Stress Test
    // -------------------------------------------------------------------------
    printf("\n[TEST] Randomized stress test (100 cycles)...\n");

    int rand_errors = 0;
    for (int i = 0; i < 100; i++) {
        bool do_write = (rand() % 2) && !dut->full;
        bool do_read = (rand() % 2) && !dut->empty;
        unsigned char data = rand() % 256;

        unsigned char read_data = dut->data_out;

        dut->wr_en = do_write;
        dut->rd_en = do_read;
        dut->data_in = data;
        fifo_set_wr_en(do_write);
        fifo_set_rd_en(do_read);
        fifo_set_data_in(data);

        tick(dut, cycle);

        if (do_write) {
            latency.record_write(data, cycle);
            writes_completed++;
        }
        if (do_read) {
            latency.check_read(read_data, cycle);
            reads_completed++;
        }

        rand_errors += compare_outputs(dut, cycle);
        coverage.sample(dut->empty, dut->full, dut->count, do_write, do_read);
    }

    dut->wr_en = 0;
    dut->rd_en = 0;
    fifo_set_wr_en(false);
    fifo_set_rd_en(false);

    total_errors += rand_errors;
    printf("  Random test errors: %d\n", rand_errors);

    // -------------------------------------------------------------------------
    // Test 6: Pointer Rollover Test
    // -------------------------------------------------------------------------
    printf("\n[TEST] Pointer rollover test...\n");

    // Drain any remaining data
    while (!dut->empty) {
        dut->rd_en = 1;
        fifo_set_rd_en(true);
        unsigned char d = dut->data_out;
        tick(dut, cycle);
        latency.check_read(d, cycle);
        reads_completed++;
    }
    dut->rd_en = 0;
    fifo_set_rd_en(false);

    // Write and read 20 items to force pointer wraparound
    for (int i = 0; i < 20; i++) {
        unsigned char data = i;

        // Write
        dut->wr_en = 1;
        dut->data_in = data;
        fifo_set_wr_en(true);
        fifo_set_data_in(data);
        tick(dut, cycle);
        latency.record_write(data, cycle);
        writes_completed++;
        dut->wr_en = 0;
        fifo_set_wr_en(false);

        // Read
        dut->rd_en = 1;
        fifo_set_rd_en(true);
        unsigned char read_data = dut->data_out;
        tick(dut, cycle);
        latency.check_read(read_data, cycle);
        reads_completed++;
        dut->rd_en = 0;
        fifo_set_rd_en(false);

        total_errors += compare_outputs(dut, cycle);

        if (read_data != data) {
            printf("  [ERROR] Rollover mismatch: wrote %d, read %d\n", data, read_data);
        }
    }
    coverage.record_rollover();
    printf("  Pointer rollover test complete\n");

    // -------------------------------------------------------------------------
    // Final Reports
    // -------------------------------------------------------------------------
    printf("\n==============================================\n");
    printf("           VERIFICATION COMPLETE\n");
    printf("==============================================\n");
    printf("\nTotal cycles: %d\n", cycle);
    printf("Writes completed: %d\n", writes_completed);
    printf("Reads completed: %d\n", reads_completed);
    printf("RTL vs Reference mismatches: %d\n", total_errors);

    latency.print_report();
    coverage.print_report();

    printf("\n========== Final Result ==========\n");
    int latency_errors = latency.get_violations();
    if (total_errors == 0 && latency_errors == 0) {
        printf("PASSED - All tests passed!\n");
    } else {
        printf("FAILED - %d mismatches, %d latency violations\n", total_errors, latency_errors);
    }

    delete dut;
    return (total_errors + latency_errors) > 0 ? 1 : 0;
}
