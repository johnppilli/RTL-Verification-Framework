// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vfifo__Syms.h"


void Vfifo___024root__trace_chg_0_sub_0(Vfifo___024root* vlSelf, VerilatedVcd::Buffer* bufp);

void Vfifo___024root__trace_chg_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfifo___024root__trace_chg_0\n"); );
    // Init
    Vfifo___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vfifo___024root*>(voidSelf);
    Vfifo__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    // Body
    Vfifo___024root__trace_chg_0_sub_0((&vlSymsp->TOP), bufp);
}

void Vfifo___024root__trace_chg_0_sub_0(Vfifo___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfifo___024root__trace_chg_0_sub_0\n"); );
    Vfifo__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode + 1);
    // Body
    if (VL_UNLIKELY((vlSelfRef.__Vm_traceActivity[1U]))) {
        bufp->chgCData(oldp+0,(vlSelfRef.fifo__DOT__memory[0]),8);
        bufp->chgCData(oldp+1,(vlSelfRef.fifo__DOT__memory[1]),8);
        bufp->chgCData(oldp+2,(vlSelfRef.fifo__DOT__memory[2]),8);
        bufp->chgCData(oldp+3,(vlSelfRef.fifo__DOT__memory[3]),8);
        bufp->chgCData(oldp+4,(vlSelfRef.fifo__DOT__memory[4]),8);
        bufp->chgCData(oldp+5,(vlSelfRef.fifo__DOT__memory[5]),8);
        bufp->chgCData(oldp+6,(vlSelfRef.fifo__DOT__memory[6]),8);
        bufp->chgCData(oldp+7,(vlSelfRef.fifo__DOT__memory[7]),8);
        bufp->chgCData(oldp+8,(vlSelfRef.fifo__DOT__wr_ptr),3);
        bufp->chgCData(oldp+9,(vlSelfRef.fifo__DOT__rd_ptr),3);
        bufp->chgCData(oldp+10,(vlSelfRef.fifo__DOT__count_reg),4);
    }
    bufp->chgBit(oldp+11,(vlSelfRef.clk));
    bufp->chgBit(oldp+12,(vlSelfRef.rst_n));
    bufp->chgBit(oldp+13,(vlSelfRef.wr_en));
    bufp->chgCData(oldp+14,(vlSelfRef.data_in),8);
    bufp->chgBit(oldp+15,(vlSelfRef.rd_en));
    bufp->chgCData(oldp+16,(vlSelfRef.data_out),8);
    bufp->chgBit(oldp+17,(vlSelfRef.full));
    bufp->chgBit(oldp+18,(vlSelfRef.empty));
    bufp->chgCData(oldp+19,(vlSelfRef.count),4);
    bufp->chgBit(oldp+20,(vlSelfRef.fifo__DOT__do_write));
    bufp->chgBit(oldp+21,(vlSelfRef.fifo__DOT__do_read));
}

void Vfifo___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfifo___024root__trace_cleanup\n"); );
    // Init
    Vfifo___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vfifo___024root*>(voidSelf);
    Vfifo__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    vlSymsp->__Vm_activity = false;
    vlSymsp->TOP.__Vm_traceActivity[0U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[1U] = 0U;
}
