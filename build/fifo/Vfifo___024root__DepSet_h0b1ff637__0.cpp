// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vfifo.h for the primary calling header

#include "Vfifo__pch.h"
#include "Vfifo___024root.h"

void Vfifo___024root___ico_sequent__TOP__0(Vfifo___024root* vlSelf);

void Vfifo___024root___eval_ico(Vfifo___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfifo___024root___eval_ico\n"); );
    Vfifo__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VicoTriggered.word(0U))) {
        Vfifo___024root___ico_sequent__TOP__0(vlSelf);
    }
}

VL_INLINE_OPT void Vfifo___024root___ico_sequent__TOP__0(Vfifo___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfifo___024root___ico_sequent__TOP__0\n"); );
    Vfifo__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.fifo__DOT__do_write = ((~ (IData)(vlSelfRef.full)) 
                                     & (IData)(vlSelfRef.wr_en));
    vlSelfRef.fifo__DOT__do_read = ((~ (IData)(vlSelfRef.empty)) 
                                    & (IData)(vlSelfRef.rd_en));
}

void Vfifo___024root___eval_triggers__ico(Vfifo___024root* vlSelf);

bool Vfifo___024root___eval_phase__ico(Vfifo___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfifo___024root___eval_phase__ico\n"); );
    Vfifo__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ __VicoExecute;
    // Body
    Vfifo___024root___eval_triggers__ico(vlSelf);
    __VicoExecute = vlSelfRef.__VicoTriggered.any();
    if (__VicoExecute) {
        Vfifo___024root___eval_ico(vlSelf);
    }
    return (__VicoExecute);
}

void Vfifo___024root___eval_act(Vfifo___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfifo___024root___eval_act\n"); );
    Vfifo__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

void Vfifo___024root___nba_sequent__TOP__0(Vfifo___024root* vlSelf);
void Vfifo___024root___nba_sequent__TOP__1(Vfifo___024root* vlSelf);

void Vfifo___024root___eval_nba(Vfifo___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfifo___024root___eval_nba\n"); );
    Vfifo__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vfifo___024root___nba_sequent__TOP__0(vlSelf);
    }
    if ((3ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vfifo___024root___nba_sequent__TOP__1(vlSelf);
        vlSelfRef.__Vm_traceActivity[1U] = 1U;
    }
}

VL_INLINE_OPT void Vfifo___024root___nba_sequent__TOP__1(Vfifo___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfifo___024root___nba_sequent__TOP__1\n"); );
    Vfifo__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*7:0*/ __VdlyVal__fifo__DOT__memory__v0;
    __VdlyVal__fifo__DOT__memory__v0 = 0;
    CData/*2:0*/ __VdlyDim0__fifo__DOT__memory__v0;
    __VdlyDim0__fifo__DOT__memory__v0 = 0;
    CData/*0:0*/ __VdlySet__fifo__DOT__memory__v0;
    __VdlySet__fifo__DOT__memory__v0 = 0;
    // Body
    __VdlySet__fifo__DOT__memory__v0 = 0U;
    if (vlSelfRef.rst_n) {
        if (vlSelfRef.fifo__DOT__do_read) {
            vlSelfRef.fifo__DOT__rd_ptr = ((7U == (IData)(vlSelfRef.fifo__DOT__rd_ptr))
                                            ? 0U : 
                                           (7U & ((IData)(1U) 
                                                  + (IData)(vlSelfRef.fifo__DOT__rd_ptr))));
        }
        if (vlSelfRef.fifo__DOT__do_write) {
            __VdlyVal__fifo__DOT__memory__v0 = vlSelfRef.data_in;
            __VdlyDim0__fifo__DOT__memory__v0 = vlSelfRef.fifo__DOT__wr_ptr;
            __VdlySet__fifo__DOT__memory__v0 = 1U;
            vlSelfRef.fifo__DOT__wr_ptr = ((7U == (IData)(vlSelfRef.fifo__DOT__wr_ptr))
                                            ? 0U : 
                                           (7U & ((IData)(1U) 
                                                  + (IData)(vlSelfRef.fifo__DOT__wr_ptr))));
        }
        vlSelfRef.fifo__DOT__count_reg = (0xfU & ((2U 
                                                   == 
                                                   (((IData)(vlSelfRef.fifo__DOT__do_write) 
                                                     << 1U) 
                                                    | (IData)(vlSelfRef.fifo__DOT__do_read)))
                                                   ? 
                                                  ((IData)(1U) 
                                                   + (IData)(vlSelfRef.fifo__DOT__count_reg))
                                                   : 
                                                  ((1U 
                                                    == 
                                                    (((IData)(vlSelfRef.fifo__DOT__do_write) 
                                                      << 1U) 
                                                     | (IData)(vlSelfRef.fifo__DOT__do_read)))
                                                    ? 
                                                   ((IData)(vlSelfRef.fifo__DOT__count_reg) 
                                                    - (IData)(1U))
                                                    : (IData)(vlSelfRef.fifo__DOT__count_reg))));
    } else {
        vlSelfRef.fifo__DOT__rd_ptr = 0U;
        vlSelfRef.fifo__DOT__wr_ptr = 0U;
        vlSelfRef.fifo__DOT__count_reg = 0U;
    }
    if (__VdlySet__fifo__DOT__memory__v0) {
        vlSelfRef.fifo__DOT__memory[__VdlyDim0__fifo__DOT__memory__v0] 
            = __VdlyVal__fifo__DOT__memory__v0;
    }
    vlSelfRef.data_out = vlSelfRef.fifo__DOT__memory
        [vlSelfRef.fifo__DOT__rd_ptr];
    vlSelfRef.count = vlSelfRef.fifo__DOT__count_reg;
    vlSelfRef.full = (8U == (IData)(vlSelfRef.fifo__DOT__count_reg));
    vlSelfRef.empty = (0U == (IData)(vlSelfRef.fifo__DOT__count_reg));
    vlSelfRef.fifo__DOT__do_write = ((~ (IData)(vlSelfRef.full)) 
                                     & (IData)(vlSelfRef.wr_en));
    vlSelfRef.fifo__DOT__do_read = ((~ (IData)(vlSelfRef.empty)) 
                                    & (IData)(vlSelfRef.rd_en));
}

void Vfifo___024root___eval_triggers__act(Vfifo___024root* vlSelf);

bool Vfifo___024root___eval_phase__act(Vfifo___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfifo___024root___eval_phase__act\n"); );
    Vfifo__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    VlTriggerVec<2> __VpreTriggered;
    CData/*0:0*/ __VactExecute;
    // Body
    Vfifo___024root___eval_triggers__act(vlSelf);
    __VactExecute = vlSelfRef.__VactTriggered.any();
    if (__VactExecute) {
        __VpreTriggered.andNot(vlSelfRef.__VactTriggered, vlSelfRef.__VnbaTriggered);
        vlSelfRef.__VnbaTriggered.thisOr(vlSelfRef.__VactTriggered);
        Vfifo___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

bool Vfifo___024root___eval_phase__nba(Vfifo___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfifo___024root___eval_phase__nba\n"); );
    Vfifo__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = vlSelfRef.__VnbaTriggered.any();
    if (__VnbaExecute) {
        Vfifo___024root___eval_nba(vlSelf);
        vlSelfRef.__VnbaTriggered.clear();
    }
    return (__VnbaExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vfifo___024root___dump_triggers__ico(Vfifo___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vfifo___024root___dump_triggers__nba(Vfifo___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vfifo___024root___dump_triggers__act(Vfifo___024root* vlSelf);
#endif  // VL_DEBUG

void Vfifo___024root___eval(Vfifo___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfifo___024root___eval\n"); );
    Vfifo__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    vlSelfRef.__Vsampled_TOP__rst_n = vlSelfRef.rst_n;
    vlSelfRef.__Vsampled_TOP__fifo__DOT__count_reg 
        = vlSelfRef.fifo__DOT__count_reg;
    vlSelfRef.__Vsampled_TOP__full = vlSelfRef.full;
    vlSelfRef.__Vsampled_TOP__empty = vlSelfRef.empty;
    vlSelfRef.__Vsampled_TOP__fifo__DOT___Vpast_0_0 
        = vlSelfRef.fifo__DOT___Vpast_0_0;
    vlSelfRef.__Vsampled_TOP__fifo__DOT___Vpast_1_0 
        = vlSelfRef.fifo__DOT___Vpast_1_0;
    vlSelfRef.__Vsampled_TOP__fifo__DOT__do_write = vlSelfRef.fifo__DOT__do_write;
    vlSelfRef.__Vsampled_TOP__fifo__DOT__do_read = vlSelfRef.fifo__DOT__do_read;
    IData/*31:0*/ __VicoIterCount;
    CData/*0:0*/ __VicoContinue;
    IData/*31:0*/ __VnbaIterCount;
    CData/*0:0*/ __VnbaContinue;
    // Body
    __VicoIterCount = 0U;
    vlSelfRef.__VicoFirstIteration = 1U;
    __VicoContinue = 1U;
    while (__VicoContinue) {
        if (VL_UNLIKELY(((0x64U < __VicoIterCount)))) {
#ifdef VL_DEBUG
            Vfifo___024root___dump_triggers__ico(vlSelf);
#endif
            VL_FATAL_MT("rtl/fifo.sv", 8, "", "Input combinational region did not converge.");
        }
        __VicoIterCount = ((IData)(1U) + __VicoIterCount);
        __VicoContinue = 0U;
        if (Vfifo___024root___eval_phase__ico(vlSelf)) {
            __VicoContinue = 1U;
        }
        vlSelfRef.__VicoFirstIteration = 0U;
    }
    __VnbaIterCount = 0U;
    __VnbaContinue = 1U;
    while (__VnbaContinue) {
        if (VL_UNLIKELY(((0x64U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            Vfifo___024root___dump_triggers__nba(vlSelf);
#endif
            VL_FATAL_MT("rtl/fifo.sv", 8, "", "NBA region did not converge.");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        __VnbaContinue = 0U;
        vlSelfRef.__VactIterCount = 0U;
        vlSelfRef.__VactContinue = 1U;
        while (vlSelfRef.__VactContinue) {
            if (VL_UNLIKELY(((0x64U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                Vfifo___024root___dump_triggers__act(vlSelf);
#endif
                VL_FATAL_MT("rtl/fifo.sv", 8, "", "Active region did not converge.");
            }
            vlSelfRef.__VactIterCount = ((IData)(1U) 
                                         + vlSelfRef.__VactIterCount);
            vlSelfRef.__VactContinue = 0U;
            if (Vfifo___024root___eval_phase__act(vlSelf)) {
                vlSelfRef.__VactContinue = 1U;
            }
        }
        if (Vfifo___024root___eval_phase__nba(vlSelf)) {
            __VnbaContinue = 1U;
        }
    }
}

#ifdef VL_DEBUG
void Vfifo___024root___eval_debug_assertions(Vfifo___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfifo___024root___eval_debug_assertions\n"); );
    Vfifo__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (VL_UNLIKELY(((vlSelfRef.clk & 0xfeU)))) {
        Verilated::overWidthError("clk");}
    if (VL_UNLIKELY(((vlSelfRef.rst_n & 0xfeU)))) {
        Verilated::overWidthError("rst_n");}
    if (VL_UNLIKELY(((vlSelfRef.wr_en & 0xfeU)))) {
        Verilated::overWidthError("wr_en");}
    if (VL_UNLIKELY(((vlSelfRef.rd_en & 0xfeU)))) {
        Verilated::overWidthError("rd_en");}
}
#endif  // VL_DEBUG
